/*
  ==============================================================================

    MultibandProcessor.h
    Created: 26 Mar 2024 9:55:06pm
    Author:  Zachary Miller

  ==============================================================================
*/

#pragma once
#include <vector>
#include "Biquad.h"
#include "PeakCompressor.h"
#include <JuceHeader.h>

class MultibandProcessor : public Biquad, public PeakCompressor {

public:
    
    void setInputGain(float inputGain) {
        this->inGain = pow(20,inputGain/10);
    }
    
    void setOutputGain(float outputGain) {
        this->outGain = pow(20,outputGain/10);
    }
    
    void setResistorValue(float drive) {
        this->R = drive; // sets resistor value [1e-5 to 1e3 to inf]
    }
    
    void setMakeupValue(float makeupGain) {
        this->makeup = makeupGain;
    }
    
    void setClipState(bool state) {
        this->clip = state;
    }
    
    float processSample_SoftClip(float x) {
            
        // Diode pair emulation
        
        float fVd = 2*Is*sinh(Vd/etaVt) + Vd/R - x/R;
        
        float count = 0;
        while ((abs(fVd) > TOL) && (count<5)) {
            
            float der = 2*Is/etaVt * cosh(Vd/etaVt) + 1/R;
            Vd = Vd - fVd/der; // update guess
            fVd = 2*Is*sinh(Vd/etaVt) + Vd/R - x/R;
            count += 1;
            
        }
        
        float Vout = Vd;
        float y = Vout;
        
        return y * makeup;
        
    }
    
    void setParallel_1(float parallel) {
        this->parallelBlend1 = parallel;
    }
    
    void setParallel_2(float parallel) {
        this->parallelBlend2 = parallel;
    }
    
    void setParallel_3(float parallel) {
        this->parallelBlend3 = parallel;
    }
    
    void setThreshold_band1(float threshold) {
        T_band1 = threshold;
    }
    
    void setThreshold_band2(float threshold) {
        T_band2 = threshold;
    }
    
    void setThreshold_band3(float threshold) {
        T_band3 = threshold;
    }
    
    void setRatio_band1(float ratio) {
        R_band1 = ratio;
    }
    
    void setRatio_band2(float ratio) {
        R_band2 = ratio;
    }
    
    void setRatio_band3(float ratio) {
        R_band3 = ratio;
    }
    
    void setKnee_band1(float knee) {
        W_band1 = knee;
    }
    
    void setKnee_band2(float knee) {
        W_band2 = knee;
    }
    
    void setKnee_band3(float knee) {
        W_band3 = knee;
    }
    
    void setAttack_band1(float attackTime) {
        alphaA_band1 = exp(-log10(9)/(Fs * attackTime));
    }
    
    void setAttack_band2(float attackTime) {
        alphaA_band2 = exp(-log10(9)/(Fs * attackTime));
    }
    
    void setAttack_band3(float attackTime) {
        alphaA_band3 = exp(-log10(9)/(Fs * attackTime));
    }
    
    void setRelease_band1(float releaseTime) {
        alphaR_band1 = exp(-log10(9)/(Fs * releaseTime));
    }
    
    void setRelease_band2(float releaseTime) {
        alphaR_band2 = exp(-log10(9)/(Fs * releaseTime));
    }
    
    void setRelease_band3(float releaseTime) {
        alphaR_band3 = exp(-log10(9)/(Fs * releaseTime));
    }
                
    void setLowCutoffFrequency(float lowCutoff) {
        LPF1.setFreq(lowCutoff);
        LPF2.setFreq(lowCutoff);
        HPF1.setFreq(lowCutoff);
        HPF2.setFreq(lowCutoff);
    }
    
    void setHighCutoffFrequency(float highCutoff) {
        APF1.setFreq(highCutoff);
        APF2.setFreq(highCutoff);
        LPF3.setFreq(highCutoff);
        LPF4.setFreq(highCutoff);
        HPF3.setFreq(highCutoff);
        HPF4.setFreq(highCutoff);
    }
        
    float processSampleBand1(float x, int channel) {
        float a = LPF1.processSample(x, channel);
        float b = LPF2.processSample(a, channel);
//        float c = APF1.processSample(b, channel);
//        float d = APF2.processSample(c, channel);
        return b;
    }
    
    float processSampleBand2(float x, int channel) {
        float e = HPF1.processSample(x, channel);
        float f = HPF2.processSample(e, channel);
        float g = LPF3.processSample(f, channel);
        float h = LPF4.processSample(g, channel);
        return h;
    }
    
    float processSampleBand3(float x, int channel) {
//        float k = HPF1.processSample(x, channel);
//        float l = HPF2.processSample(k, channel);
        float m = HPF3.processSample(x, channel);
        float n = HPF4.processSample(m, channel);
        return n;
    }
                               
    float process1CompSample(float x, int channel, float T_band1, float R_band1, float W_band1, float alphaA_band1, float alphaR_band1) {
        
        // Convert to unipolar
        float x_dB = 20*log10(abs(x));
        
        if (x_dB < -96.f) {
            x_dB = -96.f; // ensure no values of -inf
        }
        
        // Band 1 compression
        if (x_dB > T_band1 + (W_band1/2)) {
            // Above knee curve, compress
            gainSC_band1 = T_band1 + (x_dB - T_band1)/R_band1;
        } else if (x_dB > (T_band1 - W_band1/2)) {
            // Within knee curve, compress
            gainSC_band1 = x_dB + ((1/R_band1 - 1) * pow((x_dB - T_band1 + W_band1/2),2))/(2*W_band1);
        } else {
            // Do not compress
            gainSC_band1 = x_dB;
        }
        
        auto gainChange_band1 = gainSC_band1 - x_dB; // store gain change at "n" sample pos
        
        if (gainChange_band1 < gainSmoothPrev_band1) {
            // Attack mode
            gainSmooth_band1 = ((1-alphaA_band1) * gainChange_band1) + (alphaA_band1 * gainSmoothPrev_band1);
        } else {
            // Release mode
            gainSmooth_band1 = ((1-alphaR_band1) * gainChange_band1) + (alphaR_band1 * gainSmoothPrev_band1);
        }
        
        float y = pow(10,(gainSmooth_band1/20)) * x;
        
        gainSmoothPrev_band1 = gainSmooth_band1; // update gainSmooth parameter
        
        return y; // returns output on linear scale
        
    }
    
    float process2CompSample(float x, int channel, float T_band2, float R_band2, float W_band2, float alphaA_band2, float alphaR_band2) {
        
        // Convert to unipolar
        float x_dB = 20*log10(abs(x));
        
        if (x_dB < -96.f) {
            x_dB = -96.f; // ensure no values of -inf
        }
        
        // Band 2 compression
        if (x_dB > T_band2 + (W_band1/2)) {
            // Above knee curve, compress
            gainSC_band2 = T_band2 + (x_dB - T_band2)/R_band2;
        } else if (x_dB > (T_band2 - W_band2/2)) {
            // Within knee curve, compress
            gainSC_band2 = x_dB + ((1/R_band2 - 1) * pow((x_dB - T_band2 + W_band2/2),2))/(2*W_band2);
        } else {
            // Do not compress
            gainSC_band2 = x_dB;
        }
        
        auto gainChange_band2 = gainSC_band2 - x_dB; // store gain change at "n" sample pos
        
        if (gainChange_band2 < gainSmoothPrev_band2) {
            // Attack mode
            gainSmooth_band2 = ((1-alphaA_band2) * gainChange_band2) + (alphaA_band2 * gainSmoothPrev_band2);
        } else {
            // Release mode
            gainSmooth_band2 = ((1-alphaR_band2) * gainChange_band2) + (alphaR_band2 * gainSmoothPrev_band2);
        }
        
        float y = pow(10,(gainSmooth_band2/20)) * x;
        
        gainSmoothPrev_band2 = gainSmooth_band2; // update gainSmooth parameter
        
        return y; // returns output on linear scale
    
    }
        
    float process3CompSample(float x, int channel, float T_band3, float R_band3, float W_band3, float alphaA_band3, float alphaR_band3) {
        
        // Convert to unipolar
        float x_dB = 20*log10(abs(x));
        
        if (x_dB < -96.f) {
            x_dB = -96.f; // ensure no values of -inf
        }
        
        // Band 3 compression
        if (x_dB > T_band3 + (W_band3/2)) {
            // Above knee curve, compress
            gainSC_band3 = T_band3 + (x_dB - T_band3)/R_band3;
        } else if (x_dB > (T_band3 - W_band3/2)) {
            // Within knee curve, compress
            gainSC_band3 = x_dB + ((1/R_band3 - 1) * pow((x_dB - T_band3 + W_band3/2),2))/(2*W_band3);
        } else {
            // Do not compress
            gainSC_band3 = x_dB;
        }
        
        auto gainChange_band3 = gainSC_band3 - x_dB; // store gain change at "n" sample pos
        
        if (gainChange_band3 < gainSmoothPrev_band3) {
            // Attack mode
            gainSmooth_band3 = ((1-alphaA_band3) * gainChange_band3) + (alphaA_band3 * gainSmoothPrev_band3);
        } else {
            // Release mode
            gainSmooth_band3 = ((1-alphaR_band3) * gainChange_band3) + (alphaR_band3 * gainSmoothPrev_band3);
        }
        
        float y = pow(10,(gainSmooth_band3/20)) * x;
        
        gainSmoothPrev_band3 = gainSmooth_band3; // update gainSmooth parameter
        
        return y; // returns output on linear scale
    
    }
    
    void processBuffer(float * samples, const int numSamples, const int channel)
    {
        updateCoefficients();
        for (int n = 0; n < numSamples ; n++){
            
            float x = samples[n] * inGain;
            
            float Linkwitz1Buffer = processSampleBand1(x, channel); // low band
            float Linkwitz3Buffer = processSampleBand3(x, channel); // high band
            float Linkwitz2Buffer = x - (Linkwitz1Buffer + Linkwitz3Buffer); // mid band
                        
            float comp1Band = process1CompSample(Linkwitz1Buffer, channel, T_band1, R_band1, W_band1, alphaA_band1, alphaR_band1);
            float comp2Band = process2CompSample(Linkwitz2Buffer, channel, T_band2, R_band2, W_band2, alphaA_band2, alphaR_band2);
            float comp3Band = process3CompSample(Linkwitz3Buffer, channel, T_band3, R_band3, W_band3, alphaA_band3, alphaR_band3);
            
            float sumBands = x + (comp1Band * parallelBlend1) + (comp2Band * parallelBlend2) + (comp3Band * parallelBlend3);
            float sumBandsClip = processSample_SoftClip(sumBands);
            
            if (clip == true) {
                samples[n] = sumBandsClip * outGain;
            } else {
                samples[n] = sumBands * outGain;
            }
        
        }
    }

    
bool clip = false;

private:
        
    // Diode pair soft clip parameters
    const float Is = 10e-9;
    const float Vt = 0.026;
    const float eta = 2;
    const float etaVt = eta*Vt;
    float R = 1e-7;
    float Vd = 0;
    
    float TOL = 1e-18; // diode guess tolerance
    
    float makeup = 1.f;

    float Fs = 48000.f;
    float Q = 0.7071;
    
    float inGain = 0.f;
    float outGain = 0.f;
        
    Biquad LPF1 {Biquad::FilterType::LPF, Q},
           LPF2 {Biquad::FilterType::LPF, Q},
           LPF3 {Biquad::FilterType::LPF, Q},
           LPF4 {Biquad::FilterType::LPF, Q},
           APF1 {Biquad::FilterType::APF, Q},
           APF2 {Biquad::FilterType::APF, Q},
           HPF1 {Biquad::FilterType::HPF, Q},
           HPF2 {Biquad::FilterType::HPF, Q},
           HPF3 {Biquad::FilterType::HPF, Q},
           HPF4 {Biquad::FilterType::HPF, Q};
                         
    /* Compressor parameters for individual bands */
    
    float parallelBlend1 = 0.f;
    float parallelBlend2 = 0.f;
    float parallelBlend3 = 0.f;
    
    float T_band1 = 0.f;
    float T_band2 = 0.f;
    float T_band3 = 0.f;
                               
    float R_band1 = 0.f;
    float R_band2 = 0.f;
    float R_band3 = 0.f;

    float W_band1 = 0.f;
    float W_band2 = 0.f;
    float W_band3 = 0.f;
                               
    float alphaA_band1 = 0.f;
    float alphaA_band2 = 0.f;
    float alphaA_band3 = 0.f;
                               
    float alphaR_band1 = 0.f;
    float alphaR_band2 = 0.f;
    float alphaR_band3 = 0.f;
                               
    float gainSC_band1 = 0.f;
    float gainSC_band2 = 0.f;
    float gainSC_band3 = 0.f;
                               
    float gainSmooth_band1 = 0.f;
    float gainSmooth_band2 = 0.f;
    float gainSmooth_band3 = 0.f;
                               
    float gainSmoothPrev_band1 = 0.f;
    float gainSmoothPrev_band2 = 0.f;
    float gainSmoothPrev_band3 = 0.f;
                        
};

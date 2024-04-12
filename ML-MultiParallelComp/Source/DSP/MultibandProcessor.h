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
    
    void setThreshold_band1(float threshold) {
        T_band1 = threshold;
    }
    
    void setThreshold_band2(float threshold) {
        T_band2 = threshold;
    }
    
    void setRatio_band1(float ratio) {
        R_band1 = ratio;
    }
    
    void setRatio_band2(float ratio) {
        R_band2 = ratio;
    }
    
    void setKnee_band1(float knee) {
        W_band1 = knee;
    }
    
    void setKnee_band2(float knee) {
        W_band2 = knee;
    }
    
    void setAttack_band1(float attackTime) {
        alphaA_band1 = exp(-log10(9)/(Fs * attackTime));
    }
    
    void setAttack_band2(float attackTime) {
        alphaA_band2 = exp(-log10(9)/(Fs * attackTime));
    }
    
    void setRelease_band1(float releaseTime) {
        alphaR_band1 = exp(-log10(9)/(Fs * releaseTime));
    }
    
    void setRelease_band2(float releaseTime) {
        alphaR_band2 = exp(-log10(9)/(Fs * releaseTime));
    }
                
    void setCutoffFrequency(float cutoff) {
        LPF1.setFreq(cutoff);
        LPF2.setFreq(cutoff);
        HPF1.setFreq(cutoff);
        HPF2.setFreq(cutoff);
    }
        
    float processSampleLPF(float x, int channel) {
        float a = LPF1.processSample(x, channel);
        float b = LPF2.processSample(a, channel);
        return b;
    }
    
    float processSampleHPF(float x, int channel) {
        float c = HPF1.processSample(x, channel);
        float d = HPF2.processSample(c, channel);
        return d;
    }
                               
    float processLCompSample(float x, int channel, float T_band1, float R_band1, float W_band1, float alphaA_band1, float alphaR_band1) {
        
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
    
    float processHCompSample(float x, int channel, float T_band2, float R_band2, float W_band2, float alphaA_band2, float alphaR_band2) {
        
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
    
    void processBuffer(float * samples, const int numSamples, const int channel)
    {
        updateCoefficients();
        for (int n = 0; n < numSamples ; n++){
            
            float x = samples[n];
            
            float LinkwitzLPFBuffer = processSampleLPF(x, channel);
            float LinkwitzHPFBuffer = processSampleHPF(x, channel);
            
            float compLowBand = processLCompSample(LinkwitzLPFBuffer, channel, T_band1, R_band1, W_band1, alphaA_band1, alphaR_band1);
            float compHighBand = processHCompSample(LinkwitzHPFBuffer, channel, T_band2, R_band2, W_band2, alphaA_band2, alphaR_band2);
            
//            samples[n] = LinkwitzLPFBuffer // bypass high band
//            samples[n] = LinkwitzHPFBuffer // bypass low band
//            samples[n] = LinkwitzLPFBuffer + LinkwitzHPFBuffer; // bypass compression
            samples[n] = compLowBand + compHighBand;
            
        }
    }
    
private:
    
    float Fs = 48000.f;
    
    Biquad LPF1 {Biquad::FilterType::LPF, 0.7071},
           LPF2 {Biquad::FilterType::LPF, 0.7071},
           HPF1 {Biquad::FilterType::HPF, 0.7071},
           HPF2 {Biquad::FilterType::HPF, 0.7071};
                         
    /* Compressor parameters for individual bands */
    
    float T_band1 = 0.f;
    float T_band2 = 0.f;
                               
    float R_band1 = 0.f;
    float R_band2 = 0.f;
                               
    float W_band1 = 0.f;
    float W_band2 = 0.f;
                               
    float alphaA_band1 = 0.f;
    float alphaA_band2 = 0.f;
                               
    float alphaR_band1 = 0.f;
    float alphaR_band2 = 0.f;
                               
    float gainSC_band1 = 0.f;
    float gainSC_band2 = 0.f;
                               
    float gainSmooth_band1 = 0.f;
    float gainSmooth_band2 = 0.f;
                               
    float gainSmoothPrev_band1 = 0.f;
    float gainSmoothPrev_band2 = 0.f;
                        
    
};
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //public:
//
//    /* COMPRESSOR PARAMETERS */
//    
//    // Low band
//    
//    void setThreshold_band1(float threshold_band1) {
//        T_band1 = threshold_band1;
//    }
//    
//    void setRatio_band1(float ratio_band1) {
//        R_band1 = ratio_band1;
//    }
//    
//    void setKnee_band1(float knee_band1) {
//        W_band1 = knee_band1;
//    }
//    
//    void setAttack_band1(float attackTime_band1) {
//        alphaA_band1 = exp(-log10(9)/(Fs * attackTime_band1));
//    }
//    
//    void setRelease_band1(float releaseTime_band1) {
//        alphaR_band1 = exp(-log10(9)/(Fs * releaseTime_band1));
//    }
//    
//    // High band
//    
//    void setThreshold_band2(float threshold_band2) {
//        T_band2 = threshold_band2;
//    }
//
//    void setRatio_band2(float ratio_band2) {
//        R_band2 = ratio_band2;
//    }
//
//    void setKnee_band2(float knee_band2) {
//        W_band2 = knee_band2;
//    }
//    
//    void setAttack_band2(float attackTime_band2) {
//        alphaA_band2 = exp(-log10(9)/(Fs * attackTime_band2));
//    }
//    
//    void setRelease_band2(float releaseTime_band2) {
//        alphaR_band2 = exp(-log10(9)/(Fs * releaseTime_band2));
//    }
//    
//    /* --------------------- */
//    
//    float processSample_peak_band1(float x, int channel, float T_band1, float R_band1, float W_band1, float alphaA_band1, float alphaR_band1) {
//        
//        // Convert to unipolar scale
//        x = 20*log10(abs(x));
//                                       
//        if (x < -96.f) {
//            x = -96.f; // ensure no values of -inf
//        }
//        
//        if (x > T_band1 + (W_band1/2)) {
//            // Above knee curve, compress
//            gainSC_band1 = T_band1 + (x - T_band1)/R_band1;
//        } else if (x > (T_band1 - W_band1/2)) {
//            // Within knee curve, compress
//            gainSC_band1 = x + ((1/R_band1 - 1) * pow((x - T_band1 + W_band1/2),2))/(2*W_band1);
//        } else {
//            // Do not compress
//            gainSC_band1 = x;
//        }
//        
//        auto gainChange_band1 = gainSC_band1 - x; // store gain change at "n" sample pos
//
//        if (gainChange_band1 < gainSmoothPrev_band1) {
//            // Attack mode
//            gainSmooth_band1 = ((1-alphaA_band1) * gainChange_band1) + (alphaA_band1 * gainSmoothPrev_band1);
//        } else {
//            // Release mode
//            gainSmooth_band1 = ((1-alphaR_band1) * gainChange_band1) + (alphaR_band1 * gainSmoothPrev_band1);
//        }
//                                       
//        float y = pow(10,(gainSmooth_band1/20)) * x;
//                                       
//        gainSmoothPrev_band1 = gainSmooth_band1; // update gainSmooth parameter
//
//        return y; // returns output on linear scale
//        
//    }
//    
//    float processSample_peak_band2(float x, int channel, float T_band2, float R_band2, float W_band2, float alphaA_band2, float alphaR_band2) {
//        
//        // Convert to unipolar scale
//        x = 20*log10(abs(x));
//                                       
//        if (x < -96.f) {
//            x = -96.f; // ensure no values of -inf
//        }
//        
//        if (x > T_band2 + (W_band2/2)) {
//            // Above knee curve, compress
//            gainSC_band2 = T_band2 + (x - T_band2)/R_band2;
//        } else if (x > (T_band2 - W_band2/2)) {
//            // Within knee curve, compress
//            gainSC_band2 = x + ((1/R_band2 - 1) * pow((x - T_band2 + W_band2/2),2))/(2*W_band2);
//        } else {
//            // Do not compress
//            gainSC_band2 = x;
//        }
//        
//        auto gainChange_band2 = gainSC_band2 - x; // store gain change at "n" sample pos
//
//        if (gainChange_band2 < gainSmoothPrev_band2) {
//            // Attack mode
//            gainSmooth_band2 = ((1-alphaA_band2) * gainChange_band2) + (alphaA_band2 * gainSmoothPrev_band2);
//        } else {
//            // Release mode
//            gainSmooth_band2 = ((1-alphaR_band2) * gainChange_band2) + (alphaR_band2 * gainSmoothPrev_band2);
//        }
//                                       
//        float y = pow(10,(gainSmooth_band2/20)) * x;
//                                       
//        gainSmoothPrev_band2 = gainSmooth_band2; // update gainSmooth parameter
//
//        return y; // returns output on linear scale
//    
//    }
//    
//private:
//    
//    
//    float Fs = 48000.f;
//    float userFreq = 1000.f;
//    
//    /* COMPRESSION */
//    
//    float T_band1 = 0.f; // threshold, low band
//    float T_band2 = 0.f; // threshold, high band
//    
//    float R_band1 = 0.f; // ratio, low band
//    float R_band2 = 0.f; // ratio, high band
//    
//    float W_band1 = 0.f; // knee, low band
//    float W_band2 = 0.f; // knee, high band
//
//    float alphaA_band1 = 0.f; // attack coefficient, low band
//    float alphaA_band2 = 0.f; // attack coefficient, high band
//
//    float alphaR_band1 = 0.f; // release coefficient, low band
//    float alphaR_band2 = 0.f; // release coefficient, high band
//    
//    float gainSC_band1 = 0.f; // output of static characteristics
//    float gainSC_band2 = 0.f;
//    
//    float gainSmooth_band1 = 0.f;
//    float gainSmooth_band2 = 0.f;
//    
//    float gainSmoothPrev_band1 = 0.f;
//    float gainSmoothPrev_band2 = 0.f;
//    
//};

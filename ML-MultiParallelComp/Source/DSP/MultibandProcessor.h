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

class MultibandProcessor : public Biquad {
public:
        
    void prepare(double sampleRate) {
        Fs = sampleRate;
    }
    
    void setUserFreq(float userFreq) {
        userFreq = userFreq;
    }
    
    float getUserFreq() {
        return userFreq;
    }

//    IMPLEMENTING FILTERBANKING
    
//    There should be the same number of unique processSample() instances per number
//    of filter instances. For a two-band filterbank, two biquad LPFs and two biquad HPFs
//    should be cascaded. This computation occurs in parallel resulting in a sum at the
//    end to maintain the integrity of the input signal.
    
//    Model each subfunction after the following processSample() function. Originally by
//    Dr. Tarr and derived from the Audio EQ Cookbook.
    
    ///    float Biquad::processSample(float x, int channel)
    ///    {
    ///        // Output, processed sample (Direct Form 1)
    ///        float y = (b0 / a0) * x + (b1 / a0) * x1[channel] + (b2 / a0) * x2[channel]
    ///                + (-a1 / a0) * y1[channel] + (-a2 / a0) * y2[channel];
    ///
    ///        x2[channel] = x1[channel]; // store delay samples for next process step
    ///        x1[channel] = x;
    ///        y2[channel] = y1[channel];
    ///        y1[channel] = y;
    ///
    ///        return y;
    ///    };
    
    float processSample_filt1(float x, int channel) {
        
        float y = (b0 / a0) * x + (b1 / a0) * x1[channel] + (b2 / a0) * x2[channel] + (-a1 / a0) * y1[channel] + (-a2 / a0) * y2[channel];
        
        x2[channel] = x1[channel];
        x1[channel] = x;
        y2[channel] = y1[channel];
        y1[channel] = y;
        
        float a = y;
        
        return a;
    }
    
    float processSample_filt2(float a, int channel) {
        
        float y = (b0 / a0) * a + (b1 / a0) * x1[channel] + (b2 / a0) * x2[channel] + (-a1 / a0) * y1[channel] + (-a2 / a0) * y2[channel];
        
        x2[channel] = x1[channel];
        x1[channel] = a;
        y2[channel] = y1[channel];
        y1[channel] = y;
        
        float b = y;
        
        return b;
    }
    
    float processSample_filt3(float b, int channel) {
        
        float y = (b0 / a0) * b + (b1 / a0) * x1[channel] + (b2 / a0) * x2[channel] + (-a1 / a0) * y1[channel] + (-a2 / a0) * y2[channel];
        
        x2[channel] = x1[channel];
        x1[channel] = b;
        y2[channel] = y1[channel];
        y1[channel] = y;
        
        float c = y;
        
        return c;
    }
    
    float processSample_filt4(float c, int channel) {
        
        float y = (b0 / a0) * c + (b1 / a0) * x1[channel] + (b2 / a0) * x2[channel] + (-a1 / a0) * y1[channel] + (-a2 / a0) * y2[channel];
        
        x2[channel] = x1[channel];
        x1[channel] = c;
        y2[channel] = y1[channel];
        y1[channel] = y;
        
        float d = y;
        
        return d;
    }
    
    void processBuffer(float * samples, const int numSamples, const int channel) {
        updateCoefficients();
        // Perform the processing
        for (int n = 0; n < numSamples ; n++){
            float x = samples[n];
            
            // Two biquad LPFs
            setFilterType(Biquad::LPF);
            setFreq(userFreq);
            
            float a = processSample_filt1(x,channel);
            float b = processSample_filt2(a,channel);
            
            // Two biquad HPFs
            setFilterType(Biquad::HPF);
            setFreq(userFreq);
            
            float c = processSample_filt3(b,channel);
            float d = processSample_filt4(c,channel);
            
            samples[n] = (1/numFilterStages) * (a + b + c + d); // sum bands together

        }
    }
    
private:
    
    float Fs = 48000.f;
    float userFreq = 1000.f;
    const int numFilterStages = 4;
    
    // Variables for User to Modify Filter
    float freq = 20.0f; // frequency in Hz
    float Q = 0.707f; // Q => [0.1 - 10]
    float ampdB = 0.0f; // Amplitude on dB scale

    // Intermediate Variables
    float w0 = (float) (2.0 * M_PI) * freq / Fs; // Normalize frequency
    float alpha = std::sin(w0) / (2.0f * Q); // Bandwidth/slope/resonance parameter
    float A = std::pow(10.0f, ampdB / 40.0f); // Linear amplitude

    // Variables for Biquad Implementation
    // 2 channels - L,R : Up to 2nd Order
    float x1[2] = { 0.0f }; // 1 sample of delay feedforward
    float x2[2] = { 0.0f }; // 2 samples of delay feedforward
    float y1[2] = { 0.0f }; // 1 sample of delay feedback
    float y2[2] = { 0.0f }; // 2 samples of delay feedback

    // Filter coefficients
    float b0 = 1.0f; // initialized to pass signal
    float b1 = 0.0f; // without filtering
    float b2 = 0.0f;
    float a0 = 1.0f;
    float a1 = 0.0f;
    float a2 = 0.0f;
    
};

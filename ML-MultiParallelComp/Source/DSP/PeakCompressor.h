/*
  ==============================================================================

    PeakCompressor.h
    Created: 12 Mar 2024 12:15:19pm
    Author:  Zachary Miller

  ==============================================================================
*/

#pragma once

class PeakCompressor {
public:
    
    void prepareToPlay(double sampleRate);
    float processSample(float x, int channel, float T, float R, float W, float alphaA, float alphaR);
    void process(float *buffer, int numSamples, int channel);
        
    void setThreshold(float threshold); // 0dB set [-infdB - 6dB]
    void setRatio(float ratio); // 4:1 set [1:1 - inf:1]
    void setKnee(float knee); // 6dB set [0dB - 18dB]
    void setAttack(float attackTime); // 1ms set [.01ms - 1s]
    void setRelease(float releaseTime); // 30ms set [1ms - 3s]
    
private:
    
    float Fs = -1.f;
    
    // Compressor parameters
    
    float T = 0.f; // threshold
    float R = 0.f; // ratio
    float W = 0.f; // knee
    float alphaA = 0.f; // attack coefficient
    float alphaR = 0.f; // release coefficient
    
    float gainSC = 0.f; // output of static characteristics
    float gainSmooth = 0.f;
    float gainSmoothPrev = 0.f;
        
};

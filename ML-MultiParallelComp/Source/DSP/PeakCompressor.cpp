/*
  ==============================================================================

    PeakCompressor.cpp
    Created: 12 Mar 2024 12:15:19pm
    Author:  Zachary Miller

  ==============================================================================
*/

#include <cmath>
#include "PeakCompressor.h"

void PeakCompressor::prepareToPlay(double sampleRate) {
    if (sampleRate != Fs) {
        Fs = (float) sampleRate;
    }
    smoothAlpha = std::exp(-std::log(9.f)/(sampleRate*respTime));
}

void PeakCompressor::setThreshold(float threshold) {
    T = threshold;
}

void PeakCompressor::setRatio(float ratio) {
    R = ratio;
}

void PeakCompressor::setKnee(float knee) {
    W = knee;
}

void PeakCompressor::setAttack(float attackTime) {
    alphaA = exp(-log10(9)/(Fs * attackTime));
}

void PeakCompressor::setRelease(float releaseTime) {
    alphaR = exp(-log10(9)/(Fs * releaseTime));
}

float PeakCompressor::processSample(float x, int channel, float T, float R, float W, float alphaA, float alphaR) {
        
    if (x > T + (W/2)) {
        // Above knee curve, compress
        gainSC = T + (x - T)/R;
    } else if (x > (T - W/2)) {
        // Within knee curve, compress
        gainSC = x + ((1/R - 1) * pow((x - T + W/2),2))/(2*W);
    } else {
        // Do not compress
        gainSC = x;
    }
    
    auto gainChange = gainSC - x; // store gain change at "n" sample pos
    
    if (gainChange < gainSmoothPrev) {
        // Attack mode
        gainSmooth = ((1-alphaA) * gainChange) + (alphaA * gainSmoothPrev);
    } else {
        // Release mode
        gainSmooth = ((1-alphaR) * gainChange) + (alphaR * gainSmoothPrev);
    }
    
    float y = pow(10,(gainSmooth/20)) * x;
    
    gainSmoothPrev = gainSmooth; // update gainSmooth parameter
    
    return y; // returns output on linear scale
}

void PeakCompressor::process(float *buffer, int numSamples, int channel) {
    for (int n = 0; n < numSamples; n++) {
        
        // Convert unipolar scale to dB scale
        float x = 20*log10(abs(buffer[n]));
        
        if (x < -96.f) {
            x = -96.f; // ensure no values of -inf
        }
        
        buffer[n] = processSample(x,channel,T,R,W,alphaA,alphaR);
    }
}

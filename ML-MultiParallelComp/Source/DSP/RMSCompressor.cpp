/*
  ==============================================================================

    RMSCompressor.cpp
    Created: 12 Mar 2024 2:50:38pm
    Author:  Zachary Miller

  ==============================================================================
*/

#include <cmath>
#include "RMSCompressor.h"

void RMSCompressor::prepareToPlay(double sampleRate) {
    if (sampleRate != Fs) {
        Fs = (float) sampleRate;
    }
}

void RMSCompressor::setThreshold(float threshold) {
    T = threshold;
}

void RMSCompressor::setRatio(float ratio) {
    R = ratio;
}

void RMSCompressor::setKnee(float knee) {
    W = knee;
}

void RMSCompressor::setAttack(float attackTime) {
    alphaA = exp(-log10(9)/(Fs * attackTime));
}

void RMSCompressor::setRelease(float releaseTime) {
    alphaR = exp(-log10(9)/(Fs * releaseTime));
}

float RMSCompressor::processSample(float x, int channel, float T, float R, float W, float alphaA, float alphaR) {
        
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
        gainSmooth = -sqrt(((1-alphaA)*pow(gainChange,2)) + (alphaA*pow(gainSmoothPrev,2)));
    } else {
        // Release mode
        gainSmooth = -sqrt(((1-alphaR)*pow(gainChange,2)) + (alphaR*pow(gainSmoothPrev,2)));
    }
    
    float y = pow(10,(gainSmooth/20)) * x;
    
    gainSmoothPrev = gainSmooth; // update gainSmooth parameter
    
    return y; // returns output on linear scale
}

void RMSCompressor::process(float *buffer, int numSamples, int channel) {
    for (int n = 0; n < numSamples; n++) {
        
        // Convert unipolar scale to dB scale
        float x = 20*log10(abs(buffer[n]));
        
        if (x < -96.f) {
            x = -96.f; // ensure no values of -inf
        }
        
        buffer[n] = processSample(x,channel,T,R,W,alphaA,alphaR);
    }
}

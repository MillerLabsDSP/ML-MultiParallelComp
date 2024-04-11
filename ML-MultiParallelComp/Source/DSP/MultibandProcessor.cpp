/*
  ==============================================================================

    MultibandProcessor.cpp
    Created: 26 Mar 2024 9:55:06pm
    Author:  Zachary Miller

  ==============================================================================
*/

#include "MultibandProcessor.h"

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



/*
  ==============================================================================

    SoftClip.cpp
    Created: 11 Apr 2024 10:55:37am
    Author:  Zachary Miller

  ==============================================================================
*/

#include "SoftClip.h"
#include <vector>

class SoftClip {
public:
    
    float processSample(float x, int channel, float drive) {
        float xGain = x * drive;
        float y = x - ( (1/3) * pow(xGain,3) ) + ( (1/5) * pow(xGain,5) );
        return y;
    }
    
private:
    
    float xGain = 1;
    
};

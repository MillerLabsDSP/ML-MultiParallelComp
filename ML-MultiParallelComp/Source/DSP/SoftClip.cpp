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
    
    void processSoftClip(std::vector<float> & input, std::vector<float> & output) {
        
        unsigned long int N = input.size();
        
        for(int n = 0; n < N; ++n) {
            float x = input[n];
            output[n] = x - ( (1/3) * pow(x,3) ) + ( (1/5) * pow(x,5) );
        }
    }
    
private:
    
    
    
};

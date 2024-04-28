/*
  ==============================================================================

    SharedImages.h
    Created: 28 Apr 2024 10:48:53am
    Author:  Zachary Miller

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SharedImages {
public:
    
    SharedImages() { loadImages(); };
    
    void loadImages();
    
    juce::Image& getBackgroundImage()   { return m_BackgroundImage; };
    
    juce::Image& getSmallKnobImage()    { return m_SmallKnob; };
    
    juce::Image& getMedKnobImage()      { return m_MedKnob; };
    
private:
    
    juce::Image m_BackgroundImage;
    juce::Image m_SmallKnob;
    juce::Image m_MedKnob;
    
};

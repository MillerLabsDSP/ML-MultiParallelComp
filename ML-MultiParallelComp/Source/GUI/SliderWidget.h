/*
  ==============================================================================

    SliderWidget.h
    Created: 28 Apr 2024 11:24:36am
    Author:  Zachary Miller

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../SharedImages.h"

class SmallKnob : public juce::Slider {
public:
    
    SmallKnob(SharedImages* i) : knobImage(i->getSmallKnobImage()) {
        
        setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        
    }
    
    ~SmallKnob() {};
    
    void paint(juce::Graphics& g) override {
        
        /* Normalizing the slider value */
        const double fractRotation = (getValue() - getMinimum()) / (getMaximum() - getMinimum());
        
        int frameIndex = floor(fractRotation * (numFrame - 1)); // -1 to start at 0 like MATLAB
        
        const int startY = frameIndex * frameHeight; // indexes down strip for correct frame
        
        g.drawImage(knobImage,0, 0, getWidth(), getHeight(), 0, startY, frameWidth, frameHeight);
    }
    
private:
    
    juce::Image& knobImage;
    
    int numFrame = 245;
    int frameHeight = 490;
    int frameWidth = 430;
    
    
};

class BigKnob : public juce::Slider {
public:
    
    BigKnob(SharedImages* i) : knobImage(i->getBigKnobImage()) {
        
        setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle (juce::Slider::TextBoxBelow, true, 50, 25);
        
    }
    
    ~BigKnob() {};
    
    void paint(juce::Graphics& g) override {
        
        /* Normalizing the slider value */
        const double fractRotation = (getValue() - getMinimum()) / (getMaximum() - getMinimum());
        
        int frameIndex = floor(fractRotation * (numFrame - 1)); // -1 to start at 0 like MATLAB
        
        const int startY = frameIndex * frameHeight; // indexes down strip for correct frame
        
        g.drawImage(knobImage,0, 0, getWidth(), getHeight(), 0, startY, frameWidth, frameHeight);
    }
    
private:
    
    juce::Image& knobImage;
    
    int numFrame = 256;
    int frameHeight = 490;
    int frameWidth = 430;
    
    
};

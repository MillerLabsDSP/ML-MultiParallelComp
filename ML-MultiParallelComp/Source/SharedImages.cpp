/*
  ==============================================================================

    SharedImages.cpp
    Created: 28 Apr 2024 10:48:53am
    Author:  Zachary Miller

  ==============================================================================
*/

#include "SharedImages.h"

void SharedImages::loadImages() {
    
    m_BackgroundImage = juce::ImageCache::getFromMemory(BinaryData::bg_5_png, BinaryData::bg_5_pngSize);
    
    m_SmallKnob = juce::ImageCache::getFromMemory(BinaryData::Knob_small_png, BinaryData::Knob_small_pngSize);
    
    m_MedKnob = juce::ImageCache::getFromMemory(BinaryData::Knob_middle_png, BinaryData::Knob_middle_pngSize);

}

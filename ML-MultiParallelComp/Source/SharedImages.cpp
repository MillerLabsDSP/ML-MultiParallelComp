/*
  ==============================================================================

    SharedImages.cpp
    Created: 28 Apr 2024 10:48:53am
    Author:  Zachary Miller

  ==============================================================================
*/

#include "SharedImages.h"

void SharedImages::loadImages() {
    
    m_BackgroundImage = juce::ImageCache::getFromMemory(BinaryData::bg_7_png, BinaryData::bg_7_pngSize);
    
    m_SmallKnob = juce::ImageCache::getFromMemory(BinaryData::Knob_small_png, BinaryData::Knob_small_pngSize);
    
    m_BigKnob = juce::ImageCache::getFromMemory(BinaryData::Knob_big_png, BinaryData::Knob_big_pngSize);

}

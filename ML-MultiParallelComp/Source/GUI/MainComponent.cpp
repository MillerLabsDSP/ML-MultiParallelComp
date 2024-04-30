/*
  ==============================================================================

    MainComponent.cpp
    Created: 28 Apr 2024 11:24:25am
    Author:  Zachary Miller

  ==============================================================================
*/

#include "MainComponent.h"
#include <JuceHeader.h>

//==============================================================================
MainComponent::MainComponent(MLMultiParallelCompAudioProcessor& p) :
    audioProcessor          (p),
    m_pSharedImages         (audioProcessor.getSharedImagesPtr()),
    smallKnob               (m_pSharedImages),
    bigKnob                 (m_pSharedImages) {
    
//    addAndMakeVisible(bigKnob);
    
}


MainComponent::~MainComponent()
{
}

void MainComponent::paint(juce::Graphics& g)
{
    
    int width = getWidth();
    int height = getHeight();
    
    g.drawImage(m_pSharedImages->getBackgroundImage(), 0, 0, width, height, 0, 0, 2809, 1443);
}

void MainComponent::resized()
{
    bigKnob.setBounds(100, 40, 90, 102.5581);
}

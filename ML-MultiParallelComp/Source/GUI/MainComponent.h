/*
  ==============================================================================

    MainComponent.h
    Created: 28 Apr 2024 11:24:25am
    Author:  Zachary Miller

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "../SharedImages.h"
#include "SliderWidget.h"

//==============================================================================
/*
*/
class MainComponent : public juce::Component {
public:
    MainComponent(MLMultiParallelCompAudioProcessor&);
    ~MainComponent() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
//    void mouseEnter(const juce::MouseEvent &event) override;
    
private:
    
    MLMultiParallelCompAudioProcessor&      audioProcessor;
    SharedImages*                           m_pSharedImages;
    SmallKnob                               smallKnob;
    BigKnob                                 bigKnob;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    
};

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
 
 */
class MLMultiParallelCompAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MLMultiParallelCompAudioProcessorEditor (MLMultiParallelCompAudioProcessor&);
    ~MLMultiParallelCompAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
//    void sliderValueChanged(juce::Slider * slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MLMultiParallelCompAudioProcessor& audioProcessor;
    
    juce::Slider threshold_band1;
    juce::Label  threshold_band1_label;
    juce::Slider threshold_band2;
    juce::Label  threshold_band2_label;
    
    juce::Slider ratio_band1;
    juce::Label  ratio_band1_label;
    juce::Slider ratio_band2;
    juce::Label  ratio_band2_label;
    
    juce::Slider knee_band1;
    juce::Label  knee_band1_label;
    juce::Slider knee_band2;
    juce::Label  knee_band2_label;
    
    juce::Slider attack_band1;
    juce::Label  attack_band1_label;
    juce::Slider attack_band2;
    juce::Label  attack_band2_label;
    
    juce::Slider release_band1;
    juce::Label  release_band1_label;
    juce::Slider release_band2;
    juce::Label  release_band2_label;
    
    juce::Slider cutoff;
    juce::Label  cutoff_label;
     
    juce::ToggleButton softClip;
    juce::Slider clipDrive;
    juce::Label  clipDrive_label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MLMultiParallelCompAudioProcessorEditor)
};

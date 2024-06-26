/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/MainComponent.h"

//==============================================================================
/**
 
 */
class MLMultiParallelCompAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    MLMultiParallelCompAudioProcessorEditor (MLMultiParallelCompAudioProcessor&);
    ~MLMultiParallelCompAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
        
    void updateToggleState(juce::Button* clip, juce::String clip_label) {
        auto state = clip->getToggleState();
                
        juce::String stateString    = state ? "ON" : "OFF";
        juce::String selectedString = state ? "Clipping..." : "Soft Clip";
        
        if (state == false) {
            clipDrive.setEnabled(false);
            clipDriveMakeup.setEnabled(false);
        }
        if (state == true){
            clipDrive.setEnabled(true);
            clipDriveMakeup.setEnabled(true);
        }
         
        clip->setButtonText (selectedString);

    }
    
private:

    MLMultiParallelCompAudioProcessor& audioProcessor;
    
    MainComponent mainComponent;
    
    juce::Slider inputGain;
    juce::Slider outputGain;
    
    juce::Slider threshold_band1;
    juce::Label  threshold_band1_label;
    juce::Slider threshold_band2;
    juce::Label  threshold_band2_label;
    juce::Slider threshold_band3;
    juce::Label  threshold_band3_label;
    
    juce::Slider ratio_band1;
    juce::Label  ratio_band1_label;
    juce::Slider ratio_band2;
    juce::Label  ratio_band2_label;
    juce::Slider ratio_band3;
    juce::Label  ratio_band3_label;
    
    juce::Slider knee_band1;
    juce::Label  knee_band1_label;
    juce::Slider knee_band2;
    juce::Label  knee_band2_label;
    juce::Slider knee_band3;
    juce::Label  knee_band3_label;
    
    juce::Slider attack_band1;
    juce::Label  attack_band1_label;
    juce::Slider attack_band2;
    juce::Label  attack_band2_label;
    juce::Slider attack_band3;
    juce::Label  attack_band3_label;
    
    juce::Slider release_band1;
    juce::Label  release_band1_label;
    juce::Slider release_band2;
    juce::Label  release_band2_label;
    juce::Slider release_band3;
    juce::Label  release_band3_label;
    
    juce::Slider parallel_band1;
    juce::Label  parallel_band1_label;
    juce::Slider parallel_band2;
    juce::Label  parallel_band2_label;
    juce::Slider parallel_band3;
    juce::Label  parallel_band3_label;
    
    juce::Slider lowCutoff;
    juce::Label  lowCutoff_label;
    juce::Slider highCutoff;
    juce::Label  highCutoff_label;
     
    juce::TextButton clip;
    juce::Label clip_label;
    
    juce::Slider clipDrive;
    juce::Slider clipDriveMakeup;
    
    std::vector<std::unique_ptr<SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr<ButtonAttachment>> buttonAttachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MLMultiParallelCompAudioProcessorEditor)
};

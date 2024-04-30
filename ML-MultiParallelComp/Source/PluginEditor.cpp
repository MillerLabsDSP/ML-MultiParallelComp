/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GUI/MainComponent.h"
#include <JuceHeader.h>
#include <cmath>

//==============================================================================
MLMultiParallelCompAudioProcessorEditor::MLMultiParallelCompAudioProcessorEditor (MLMultiParallelCompAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p), mainComponent(p)
{
    setSize(820, 420);
    addAndMakeVisible(mainComponent);
    
    const juce::Colour linearBarColour = juce::Colours::orange;
    const juce::Colour sliderColour = juce::Colours::orange;

    /* I/O GAIN */
    
    // Input gain slider
    inputGain.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    inputGain.setColour(juce::Slider::thumbColourId, sliderColour);
    inputGain.setBounds(25, 45, 50, 335);
    inputGain.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 30);
    inputGain.setTextValueSuffix(" dB");
    inputGain.onValueChange = [this](){
        audioProcessor.inputGainChanged(inputGain.getValue());
    };
    addAndMakeVisible(inputGain);
    
    
    // Output gain slider
    outputGain.setBounds(745, 45, 50, 335);
    outputGain.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    outputGain.setColour(juce::Slider::thumbColourId, sliderColour);
    outputGain.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 30);
    outputGain.setTextValueSuffix(" dB");
    outputGain.onValueChange = [this](){
        audioProcessor.outputGainChanged(outputGain.getValue());
    };
    addAndMakeVisible(outputGain);
    
    /* PARALLEL COMPRESSION - BAND 1 */
    
    // Threshold slider (band #1)
    threshold_band1.setBounds(80, 30, 120, 120);
    threshold_band1.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    threshold_band1.setColour(juce::Slider::thumbColourId, sliderColour);
    threshold_band1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    threshold_band1.setTextValueSuffix(" dB");
    threshold_band1.onValueChange = [this](){
        audioProcessor.thresholdBand1Changed(threshold_band1.getValue());
    };
    addAndMakeVisible(threshold_band1);
    
    // Ratio slider (band #1
    ratio_band1.setBounds(110, 170, 160, 25);
    ratio_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    ratio_band1.setColour(juce::Slider::trackColourId, linearBarColour);
    ratio_band1.setTextValueSuffix(":1");
    ratio_band1.onValueChange = [this](){
        audioProcessor.ratioBand1Changed(ratio_band1.getValue());
    };
    addAndMakeVisible(ratio_band1);

    // Knee slider (band #1)
    knee_band1.setBounds(110, 205, 160, 25);
    knee_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    knee_band1.setColour(juce::Slider::trackColourId, linearBarColour);
    knee_band1.setTextValueSuffix(" dB");
    knee_band1.onValueChange = [this](){
        audioProcessor.kneeBand1Changed(knee_band1.getValue());
    };
    addAndMakeVisible(knee_band1);
    
    // Attack slider (band #1)
    attack_band1.setBounds(110, 240, 160, 25);
    attack_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    attack_band1.setColour(juce::Slider::trackColourId, linearBarColour);
    attack_band1.setTextValueSuffix(" ms");
    attack_band1.onValueChange = [this](){
        audioProcessor.attackBand1Changed(attack_band1.getValue());
    };
    addAndMakeVisible(attack_band1);
    
    // Release slider (band #1)
    release_band1.setBounds(110, 275, 160, 25);
    release_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    release_band1.setColour(juce::Slider::trackColourId, linearBarColour);
    release_band1.setTextValueSuffix(" ms");
    release_band1.onValueChange = [this](){
        audioProcessor.releaseBand1Changed(release_band1.getValue());
    };
    addAndMakeVisible(release_band1);
    
    // Parallel blend slider (band #1)
    parallel_band1.setBounds(190, 50, 100, 100);
    parallel_band1.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    parallel_band1.setColour(juce::Slider::thumbColourId, sliderColour);
    parallel_band1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    parallel_band1.onValueChange = [this](){
        audioProcessor.parallelBand1Changed(parallel_band1.getValue());
    };
    addAndMakeVisible(parallel_band1);

    /* PARALLEL COMPRESSION - BAND 2 */
    
    // Threshold slider (band #2)
    threshold_band2.setBounds(300, 30, 120, 120);
    threshold_band2.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    threshold_band2.setColour(juce::Slider::thumbColourId, sliderColour);
    threshold_band2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    threshold_band2.setTextValueSuffix(" dB");
    threshold_band2.onValueChange = [this](){
        audioProcessor.thresholdBand2Changed(threshold_band2.getValue());
    };
    addAndMakeVisible(threshold_band2);
    
    // Ratio slider (band #2)
    ratio_band2.setBounds(330, 170, 160, 25);
    ratio_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    ratio_band2.setColour(juce::Slider::trackColourId, linearBarColour);
    ratio_band2.setTextValueSuffix(":1");
    ratio_band2.onValueChange = [this](){
        audioProcessor.ratioBand2Changed(ratio_band2.getValue());
    };
    addAndMakeVisible(ratio_band2);
    
    // Knee slider (band #2)
    knee_band2.setBounds(330, 205, 160, 25);
    knee_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    knee_band2.setColour(juce::Slider::trackColourId, linearBarColour);
    knee_band2.setTextValueSuffix(" dB");
    knee_band2.onValueChange = [this](){
        audioProcessor.kneeBand2Changed(knee_band2.getValue());
    };
    addAndMakeVisible(knee_band2);
    
    // Attack slider (band #2)
    attack_band2.setBounds(330, 240, 160, 25);
    attack_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    attack_band2.setColour(juce::Slider::trackColourId, linearBarColour);
    attack_band2.setTextValueSuffix(" ms");
    attack_band2.onValueChange = [this](){
        audioProcessor.attackBand2Changed(attack_band2.getValue());
    };
    addAndMakeVisible(attack_band2);

    // Release slider (band #2)
    release_band2.setBounds(330, 275, 160, 25);
    release_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    release_band2.setColour(juce::Slider::trackColourId, linearBarColour);
    release_band2.setTextValueSuffix(" ms");
    release_band2.onValueChange = [this](){
        audioProcessor.releaseBand2Changed(release_band2.getValue());
    };
    addAndMakeVisible(release_band2);
    
    // Parallel blend slider (band #2)
    parallel_band2.setBounds(410, 50, 100, 100);
    parallel_band2.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    parallel_band2.setColour(juce::Slider::thumbColourId, sliderColour);
    parallel_band2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    parallel_band2.onValueChange = [this](){
        audioProcessor.parallelBand2Changed(parallel_band2.getValue());
    };
    addAndMakeVisible(parallel_band2);
    
    /* PARALLEL COMPRESSION - BAND 3 */
    
    // Threshold slider (band #3)
    threshold_band3.setBounds(520, 30, 120, 120);
    threshold_band3.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    threshold_band3.setColour(juce::Slider::thumbColourId, sliderColour);
    threshold_band3.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    threshold_band3.setTextValueSuffix(" dB");
    threshold_band3.onValueChange = [this](){
        audioProcessor.thresholdBand3Changed(threshold_band3.getValue());
    };
    addAndMakeVisible(threshold_band3);
    
    // Ratio slider (band #3)
    ratio_band3.setBounds(550, 170, 160, 25);
    ratio_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    ratio_band3.setColour(juce::Slider::trackColourId, linearBarColour);
    ratio_band3.setTextValueSuffix(":1");
    ratio_band3.onValueChange = [this](){
        audioProcessor.ratioBand3Changed(ratio_band3.getValue());
    };
    addAndMakeVisible(ratio_band3);

    // Knee slider (band #3)
    knee_band3.setBounds(550, 205, 160, 25);
    knee_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    knee_band3.setColour(juce::Slider::trackColourId, linearBarColour);
    knee_band3.setTextValueSuffix(" dB");
    knee_band3.onValueChange = [this](){
        audioProcessor.kneeBand3Changed(knee_band3.getValue());
    };
    addAndMakeVisible(knee_band3);
    
    // Attack slider (band #3)
    attack_band3.setBounds(550, 240, 160, 25);
    attack_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    attack_band3.setColour(juce::Slider::trackColourId, linearBarColour);
    attack_band3.setTextValueSuffix(" ms");
    attack_band3.onValueChange = [this](){
        audioProcessor.attackBand3Changed(attack_band3.getValue());
    };
    addAndMakeVisible(attack_band3);
 
    // Release slider (band #3)
    release_band3.setBounds(550, 275, 160, 25);
    release_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    release_band3.setColour(juce::Slider::trackColourId, linearBarColour);
    release_band3.setTextValueSuffix(" ms");
    release_band3.onValueChange = [this](){
        audioProcessor.releaseBand3Changed(release_band3.getValue());
    };
    addAndMakeVisible(release_band3);
    
    // Parallel blend slider (band #3)
    parallel_band3.setBounds(630, 50, 100, 100);
    parallel_band3.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    parallel_band3.setColour(juce::Slider::thumbColourId, sliderColour);
    parallel_band3.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    parallel_band3.onValueChange = [this](){
        audioProcessor.parallelBand3Changed(parallel_band3.getValue());
    };
    addAndMakeVisible(parallel_band3);
    
    /* CUTOFFS */
        
    // Low cutoff slider
    lowCutoff.setBounds(110, 310, 295, 25);
    lowCutoff.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    lowCutoff.setColour(juce::Slider::trackColourId, linearBarColour);
    lowCutoff.setTextValueSuffix(" Hz");
    lowCutoff.onValueChange = [this](){
        audioProcessor.lowCutoffChanged(lowCutoff.getValue());
    };
    addAndMakeVisible(lowCutoff);
    
    // High cutoff slider
    highCutoff.setBounds(415, 310, 295, 25);
    highCutoff.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    highCutoff.setColour(juce::Slider::trackColourId, linearBarColour);
    highCutoff.setTextValueSuffix(" Hz");
    highCutoff.onValueChange = [this](){
        audioProcessor.highCutoffChanged(highCutoff.getValue());
    };
    addAndMakeVisible(highCutoff);
    
    /* CLIPPING */
    
    // Diode pair clipper button
    clip.setBounds(365, 365, 90, 25);
    clip.setButtonText("Soft Clip");
    clip.setToggleState(false, juce::dontSendNotification);
    clip.setClickingTogglesState(true);
    updateToggleState (&clip, "Soft Clip");
    clip.onClick = [this](){
        updateToggleState (&clip, "Soft Clip");
        audioProcessor.clipButtonToggled(clip.getToggleState());
    };
    addAndMakeVisible(clip);

    // Diode pair clipper slider
    clipDrive.setBounds(205, 340, 160, 75);
    clipDrive.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    clipDrive.setColour(juce::Slider::thumbColourId, sliderColour);
    clipDrive.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 50, 25);
    clipDrive.onValueChange = [this](){
        audioProcessor.clipDriveChanged(clipDrive.getValue());
    };
    addAndMakeVisible(clipDrive);
    
    // Diode pair clipper makeup gain slider
    clipDriveMakeup.setBounds(455, 340, 160, 75);
    clipDriveMakeup.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    clipDriveMakeup.setColour(juce::Slider::thumbColourId, sliderColour);
    clipDriveMakeup.setTextBoxStyle(juce::Slider::TextBoxRight, true, 50, 25);
    clipDriveMakeup.onValueChange = [this](){
        audioProcessor.clipMakeupChanged(clipDriveMakeup.getValue());
    };
    addAndMakeVisible(clipDriveMakeup);
    
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::INPUTGAIN, inputGain));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::OUTPUTGAIN, outputGain));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::THRESHOLD_BAND1, threshold_band1));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::RATIO_BAND1, ratio_band1));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::KNEE_BAND1, knee_band1));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::ATTACK_BAND1, attack_band1));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::RELEASE_BAND1, release_band1));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::PARALLEL_BAND1, parallel_band1));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::THRESHOLD_BAND2, threshold_band2));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::RATIO_BAND2, ratio_band2));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::KNEE_BAND2, knee_band2));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::ATTACK_BAND2, attack_band2));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::RELEASE_BAND2, release_band2));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::PARALLEL_BAND2, parallel_band2));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::THRESHOLD_BAND3, threshold_band3));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::RATIO_BAND3, ratio_band3));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::KNEE_BAND3, knee_band3));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::ATTACK_BAND3, attack_band3));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::RELEASE_BAND3, release_band3));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::PARALLEL_BAND3, parallel_band3));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::LINKWITZ1, lowCutoff));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::LINKWITZ2, highCutoff));
    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::CLIPTOGGLEBUTTON, clip));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::CLIPDRIVE, clipDrive));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,MLMultiParallelCompAudioProcessor::MAKEUP, clipDriveMakeup));
}

MLMultiParallelCompAudioProcessorEditor::~MLMultiParallelCompAudioProcessorEditor()
{
}

//==============================================================================
void MLMultiParallelCompAudioProcessorEditor::paint (juce::Graphics& g)
{

}

void MLMultiParallelCompAudioProcessorEditor::resized()
{
    mainComponent.setBounds(0, 0, getWidth(), getHeight());
}

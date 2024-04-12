/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MLMultiParallelCompAudioProcessorEditor::MLMultiParallelCompAudioProcessorEditor (MLMultiParallelCompAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(1020, 305);
    
    // Threshold slider (low band)
    threshold_band1.setBounds(205, 40, 275, 50);
    threshold_band1.setRange(-50, 0);
    threshold_band1.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    threshold_band1.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 75, 25);
    addAndMakeVisible(threshold_band1);
//    threshold_band1.addListener(this);
    
    addAndMakeVisible(threshold_band1_label);
    threshold_band1_label.setText("Threshold (dB)", juce::dontSendNotification);
    threshold_band1_label.attachToComponent(&threshold_band1, true);
    
    // Threshold slider (high band)
    threshold_band2.setBounds(595, 40, 275, 50);
    threshold_band2.setRange(-50, 0);
    threshold_band2.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    threshold_band2.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 75, 25);
    addAndMakeVisible(threshold_band2);
//    threshold_band2.addListener(this);
    
    addAndMakeVisible(threshold_band2_label);
    threshold_band2_label.setText("Threshold (dB)", juce::dontSendNotification);
    threshold_band2_label.attachToComponent(&threshold_band2, true);
    
    // Ratio slider (low band)
    ratio_band1.setBounds(205, 90, 275, 50);
    ratio_band1.setRange(1, 10);
    ratio_band1.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    ratio_band1.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 75, 25);
    addAndMakeVisible(ratio_band1);
//    ratio_band1.addListener(this);
    
    addAndMakeVisible(ratio_band1_label);
    ratio_band1_label.setText("Ratio", juce::dontSendNotification);
    ratio_band1_label.attachToComponent(&ratio_band1, true);
    
    // Ratio slider (high band)
    ratio_band2.setBounds(595, 90, 275, 50);
    ratio_band2.setRange(1, 10);
    ratio_band2.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    ratio_band2.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 75, 25);
    addAndMakeVisible(ratio_band2);
//    ratio_band2.addListener(this);
    
    addAndMakeVisible(ratio_band2_label);
    ratio_band2_label.setText("Ratio", juce::dontSendNotification);
    ratio_band2_label.attachToComponent(&ratio_band2, true);
    
    // Knee slider (low band)
    knee_band1.setBounds(205, 140, 275, 50);
    knee_band1.setRange(0, 6);
    knee_band1.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    knee_band1.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 75, 25);
    addAndMakeVisible(knee_band1);
//    knee_band1.addListener(this);
    
    addAndMakeVisible(knee_band1_label);
    knee_band1_label.setText("Knee", juce::dontSendNotification);
    knee_band1_label.attachToComponent(&knee_band1, true);
    
    // Knee slider (high band)
    knee_band2.setBounds(595, 140, 275, 50);
    knee_band2.setRange(0, 6);
    knee_band2.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    knee_band2.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 75, 25);
    addAndMakeVisible(knee_band2);
//    knee_band2.addListener(this);
    
    addAndMakeVisible(knee_band2_label);
    knee_band2_label.setText("Knee", juce::dontSendNotification);
    knee_band2_label.attachToComponent(&knee_band2, true);
    
    // Attack slider (low band)
    attack_band1.setBounds(205, 190, 275, 50);
    attack_band1.setRange(0, 6);
    attack_band1.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    attack_band1.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 75, 25);
    addAndMakeVisible(attack_band1);
//    attack_band1.addListener(this);
    
    addAndMakeVisible(attack_band1_label);
    attack_band1_label.setText("Attack", juce::dontSendNotification);
    attack_band1_label.attachToComponent(&attack_band1, true);
    
    // Attack slider (high band)
    attack_band2.setBounds(595, 190, 275, 50);
    attack_band2.setRange(0, 6);
    attack_band2.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    attack_band2.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 75, 25);
    addAndMakeVisible(attack_band2);
//    attack_band2.addListener(this);
    
    addAndMakeVisible(knee_band2_label);
    attack_band2_label.setText("Attack", juce::dontSendNotification);
    attack_band2_label.attachToComponent(&attack_band2, true);
    
    // Release slider (low band)
    release_band1.setBounds(205, 240, 275, 50);
    release_band1.setRange(0, 6);
    release_band1.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    release_band1.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 75, 25);
    addAndMakeVisible(release_band1);
//    attack_band1.addListener(this);
    
    addAndMakeVisible(release_band1_label);
    release_band1_label.setText("Release", juce::dontSendNotification);
    release_band1_label.attachToComponent(&release_band1, true);
    
    // Release slider (high band)
    release_band2.setBounds(595, 240, 275, 50);
    release_band2.setRange(0, 6);
    release_band2.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    release_band2.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 75, 25);
    addAndMakeVisible(release_band2);
//    release_band2.addListener(this);
    
    addAndMakeVisible(release_band2_label);
    release_band2_label.setText("Release", juce::dontSendNotification);
    release_band2_label.attachToComponent(&release_band2, true);
    
    // Cutoff slider
    cutoff.setBounds(30, 55, 50, 225);
    cutoff.setRange(20.0, 8000.0);
    cutoff.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    cutoff.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 150);
    addAndMakeVisible(cutoff);
//    cutoff.addListener(this);
    
    clipDrive_label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(cutoff_label);
    cutoff_label.setText("Cutoff", juce::dontSendNotification);
    cutoff_label.attachToComponent(&cutoff, false);
    
    
    // Soft-clip button
    softClip.setBounds(900, 180, 120, 120);
    softClip.setButtonText("Soft Clip");
    softClip.setToggleState(false,juce::dontSendNotification);
    addAndMakeVisible(softClip);
//    softClip.addListener(this);
    
    // Soft-clip drive slider
    clipDrive.setBounds(885, 90, 120, 120);
    clipDrive.setRange(0, 10);
    clipDrive.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    clipDrive.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 25);
    addAndMakeVisible(clipDrive);
//    clipDrive.addListener(this);
    
    clipDrive_label.setJustificationType(juce::Justification::centred);
    clipDrive_label.setText("Drive", juce::dontSendNotification);
    clipDrive_label.attachToComponent(&clipDrive, false);
    addAndMakeVisible(clipDrive_label);

    
}

MLMultiParallelCompAudioProcessorEditor::~MLMultiParallelCompAudioProcessorEditor()
{
}

//==============================================================================
void MLMultiParallelCompAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    
    g.fillAll (juce::Colour (41, 42, 47));

        g.setColour(juce::Colours::white);
        
        g.setFont(20);
        g.drawText ("Low Band", 250, 9, 100, 30, juce::Justification::left, false);
        
        g.setFont(20);
        g.drawText ("High Band", 630, 9, 100, 30, juce::Justification::centred, false);


}

void MLMultiParallelCompAudioProcessorEditor::resized()
{

}

void MLMultiParallelCompAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    
    if (slider == &cutoff) {
        audioProcessor.cutoffVal = cutoff.getValue();
    }
    
    if (slider == &threshold_band1) {
        audioProcessor.thresholdBand1 = threshold_band1.getValue();
    }
    
    if (slider == &threshold_band2) {
        audioProcessor.thresholdBand2 = threshold_band2.getValue();
    }
    
    if (slider == &ratio_band1) {
        audioProcessor.ratioBand1 = ratio_band1.getValue();
    }
    
    if (slider == &ratio_band2) {
        audioProcessor.ratioBand2 = ratio_band2.getValue();
    }
    
    if (slider == &knee_band1) {
        audioProcessor.kneeBand1 = knee_band1.getValue();
    }
    
    if (slider == &knee_band2) {
        audioProcessor.kneeBand2 = knee_band2.getValue();
    }
    
    if (slider == &attack_band1) {
        audioProcessor.attackBand1 = attack_band1.getValue();
    }
    
    if (slider == &attack_band2) {
        audioProcessor.attackBand2 = attack_band2.getValue();
    }
    
    if (slider == &release_band1) {
        audioProcessor.releaseBand1 = release_band1.getValue();
    }
    
    if (slider == &release_band2) {
        audioProcessor.releaseBand2 = release_band2.getValue();
    }
    
}

//void MLMultiParallelCompAudioProcessorEditor::sliderValueChanged(juce::Slider * slider) {
//    if (slider == &freqSlider) {
//        audioProcessor.filterFrequency = freqSlider.getValue();
//    }
//    
//    if (slider == &qSlider) {
//        audioProcessor.filterQ = qSlider.getValue();
//    }
//    
//    if (slider == &ampSlider) {
//        audioProcessor.filterQ = ampSlider.getValue();
//    }
//}

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
    setSize(1000, 325);
    
    
    
    /* ---------------------------- */
    // PARALLEL COMPRESSION BAND #1 //
    /* ---------------------------- */
    
    
    
    // Threshold slider (band #1)
    threshold_band1.setBounds(60, 30, 120, 120);
    threshold_band1.setRange(-50, 0);
    threshold_band1.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    threshold_band1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    threshold_band1.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(threshold_band1);
    threshold_band1.addListener(this);
    
//    addAndMakeVisible(threshold_band1_label);
//    threshold_band1_label.setText("Threshold (dB)", juce::dontSendNotification);
//    threshold_band1_label.attachToComponent(&threshold_band1, true);
    
    // Ratio slider (band #1)
    ratio_band1.setBounds(90, 170, 160, 25);
    ratio_band1.setRange(1, 10);
    ratio_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    ratio_band1.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(ratio_band1);
    ratio_band1.addListener(this);
    
//    addAndMakeVisible(ratio_band1_label);
//    ratio_band1_label.setText("Ratio", juce::dontSendNotification);
//    ratio_band1_label.attachToComponent(&ratio_band1, true);
    
    // Knee slider (band #1)
    knee_band1.setBounds(90, 205, 160, 25);
    knee_band1.setRange(0, 6);
    knee_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    knee_band1.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(knee_band1);
    knee_band1.addListener(this);
    
//    addAndMakeVisible(knee_band1_label);
//    knee_band1_label.setText("Knee", juce::dontSendNotification);
//    knee_band1_label.attachToComponent(&knee_band1, true);
    
    // Attack slider (band #1)
    attack_band1.setBounds(90, 240, 160, 25);
    attack_band1.setRange(0, 6);
    attack_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    attack_band1.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(attack_band1);
    attack_band1.addListener(this);
    
//    addAndMakeVisible(attack_band1_label);
//    attack_band1_label.setText("Attack", juce::dontSendNotification);
//    attack_band1_label.attachToComponent(&attack_band1, true);
    
    // Release slider (band #1)
    release_band1.setBounds(90, 275, 160, 25);
    release_band1.setRange(0, 6);
    release_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    release_band1.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(release_band1);
    attack_band1.addListener(this);
    
//    addAndMakeVisible(release_band1_label);
//    release_band1_label.setText("Release", juce::dontSendNotification);
//    release_band1_label.attachToComponent(&release_band1, true);
    
    // Parallel blend slider (band #1)
    parallel_band1.setBounds(170, 50, 100, 100);
    parallel_band1.setRange(0, 6);
    parallel_band1.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    parallel_band1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    parallel_band1.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(parallel_band1);
    parallel_band1.addListener(this);
    
//    addAndMakeVisible(parallel_band1_label);
//    parallel_band1_label.setText("Parallel", juce::dontSendNotification);
//    parallel_band1_label.attachToComponent(&parallel_band1, false);
    
    
    
    /* ---------------------------- */
    // PARALLEL COMPRESSION BAND #2 //
    /* ---------------------------- */
    
    
    
    // Threshold slider (band #2)
    threshold_band2.setBounds(340, 30, 120, 120);
    threshold_band2.setRange(-50, 0);
    threshold_band2.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    threshold_band2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    threshold_band2.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(threshold_band2);
    threshold_band2.addListener(this);
    
//    addAndMakeVisible(threshold_band2_label);
//    threshold_band2_label.setText("Threshold (dB)", juce::dontSendNotification);
//    threshold_band2_label.attachToComponent(&threshold_band2, true);
    
    // Ratio slider (band #2)
    ratio_band2.setBounds(370, 170, 160, 25);
    ratio_band2.setRange(1, 10);
    ratio_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    ratio_band2.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(ratio_band2);
    ratio_band2.addListener(this);
    
//    addAndMakeVisible(ratio_band2_label);
//    ratio_band2_label.setText("Ratio", juce::dontSendNotification);
//    ratio_band2_label.attachToComponent(&ratio_band2, true);
    
    // Knee slider (band #2)
    knee_band2.setBounds(370, 205, 160, 25);
    knee_band2.setRange(0, 6);
    knee_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    knee_band2.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(knee_band2);
    knee_band2.addListener(this);
    
//    addAndMakeVisible(knee_band2_label);
//    knee_band2_label.setText("Knee", juce::dontSendNotification);
//    knee_band2_label.attachToComponent(&knee_band2, true);
    
    // Attack slider (band #2)
    attack_band2.setBounds(370, 240, 160, 25);
    attack_band2.setRange(0, 6);
    attack_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    attack_band2.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(attack_band2);
    attack_band2.addListener(this);
    
//    addAndMakeVisible(attack_band2_label);
//    attack_band2_label.setText("Attack", juce::dontSendNotification);
//    attack_band2_label.attachToComponent(&attack_band2, true);
    
    // Release slider (band #2)
    release_band2.setBounds(370, 275, 160, 25);
    release_band2.setRange(0, 6);
    release_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    release_band2.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(release_band2);
    release_band2.addListener(this);
    
//    addAndMakeVisible(release_band2_label);
//    release_band2_label.setText("Release", juce::dontSendNotification);
//    release_band2_label.attachToComponent(&release_band2, true);
  
    // Parallel blend slider (band #2)
    parallel_band2.setBounds(450, 50, 100, 100);
    parallel_band2.setRange(0, 6);
    parallel_band2.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    parallel_band2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    parallel_band2.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(parallel_band2);
    parallel_band2.addListener(this);
    
//    addAndMakeVisible(parallel_band2_label);
//    parallel_band2_label.setText("Parallel", juce::dontSendNotification);
//    parallel_band2_label.attachToComponent(&parallel_band2, true);
    
    
    
    /* ---------------------------- */
    // PARALLEL COMPRESSION BAND #3 //
    /* ---------------------------- */
    
    
    
    // Threshold slider (band #3)
    threshold_band3.setBounds(620, 30, 120, 120);
    threshold_band3.setRange(-50, 0);
    threshold_band3.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    threshold_band3.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    threshold_band3.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(threshold_band3);
    threshold_band3.addListener(this);
    
//    addAndMakeVisible(threshold_band2_label);
//    threshold_band2_label.setText("Threshold (dB)", juce::dontSendNotification);
//    threshold_band2_label.attachToComponent(&threshold_band2, true);
    
    // Ratio slider (band #3)
    ratio_band3.setBounds(650, 170, 160, 25);
    ratio_band3.setRange(1, 10);
    ratio_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    ratio_band3.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(ratio_band3);
    ratio_band3.addListener(this);
    
//    addAndMakeVisible(ratio_band2_label);
//    ratio_band2_label.setText("Ratio", juce::dontSendNotification);
//    ratio_band2_label.attachToComponent(&ratio_band2, true);
    
    // Knee slider (band #3)
    knee_band3.setBounds(650, 205, 160, 25);
    knee_band3.setRange(0, 6);
    knee_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    knee_band3.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(knee_band3);
    knee_band3.addListener(this);
    
//    addAndMakeVisible(knee_band2_label);
//    knee_band2_label.setText("Knee", juce::dontSendNotification);
//    knee_band2_label.attachToComponent(&knee_band2, true);
    
    // Attack slider (band #3)
    attack_band3.setBounds(650, 240, 160, 25);
    attack_band3.setRange(0, 6);
    attack_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    attack_band3.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(attack_band3);
    attack_band3.addListener(this);
    
//    addAndMakeVisible(attack_band2_label);
//    attack_band2_label.setText("Attack", juce::dontSendNotification);
//    attack_band2_label.attachToComponent(&attack_band2, true);
    
    // Release slider (band #3)
    release_band3.setBounds(650, 275, 160, 25);
    release_band3.setRange(0, 6);
    release_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    release_band3.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(release_band3);
    release_band3.addListener(this);
    
//    addAndMakeVisible(release_band2_label);
//    release_band2_label.setText("Release", juce::dontSendNotification);
//    release_band2_label.attachToComponent(&release_band2, true);
  
    // Parallel blend slider (band #3)
    parallel_band3.setBounds(730, 50, 100, 100);
    parallel_band3.setRange(0, 6);
    parallel_band3.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    parallel_band3.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    parallel_band3.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(parallel_band3);
    parallel_band3.addListener(this);
    
//    addAndMakeVisible(parallel_band2_label);
//    parallel_band2_label.setText("Parallel", juce::dontSendNotification);
//    parallel_band2_label.attachToComponent(&parallel_band2, true);
    
    
    
    /* ---------------------------- */
    //          CUTOFFS             //
    /* ---------------------------- */
    
    
    
    // Low cutoff slider
    lowCutoff.setBounds(30, 55, 50, 225);
    lowCutoff.setRange(20.0, 8000.0);
    lowCutoff.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    lowCutoff.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 150);
//    addAndMakeVisible(lowCutoff);
    lowCutoff.addListener(this);
    
//    lowCutoff_label.setJustificationType(juce::Justification::centred);
//    addAndMakeVisible(lowCutoff_label);
//    lowCutoff_label.setText("Low Cutoff", juce::dontSendNotification);
//    lowCutoff_label.attachToComponent(&lowCutoff, false);
    
    
    
    /* ---------------------------- */
    //          CLIPPING            //
    /* ---------------------------- */
    
    
    
    // Soft-clip button
    softClip.setBounds(900, 180, 120, 120);
    softClip.setButtonText("Soft Clip");
    softClip.setToggleState(false,juce::dontSendNotification);
//    addAndMakeVisible(softClip);
    softClip.addListener(this);
    
    // Soft-clip drive slider
    clipDrive.setBounds(885, 90, 120, 120);
    clipDrive.setRange(0, 10);
    clipDrive.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    clipDrive.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 25);
//    addAndMakeVisible(clipDrive);
    clipDrive.addListener(this);
    
    clipDrive_label.setJustificationType(juce::Justification::centred);
    clipDrive_label.setText("Drive", juce::dontSendNotification);
    clipDrive_label.attachToComponent(&clipDrive, false);
//    addAndMakeVisible(clipDrive_label);

    
}

MLMultiParallelCompAudioProcessorEditor::~MLMultiParallelCompAudioProcessorEditor()
{
}

//==============================================================================
void MLMultiParallelCompAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    
    g.fillAll (juce::Colour (41, 42, 47));

//        g.setColour(juce::Colours::white);
//        
//        g.setFont(20);
//        g.drawText ("Low Band", 250, 9, 100, 30, juce::Justification::left, false);
//        
//        g.setFont(20);
//        g.drawText ("High Band", 630, 9, 100, 30, juce::Justification::centred, false);


}

void MLMultiParallelCompAudioProcessorEditor::resized()
{

}

void MLMultiParallelCompAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    
    if (slider == &lowCutoff) {
        audioProcessor.lowCutoff = lowCutoff.getValue();
    }
    
    if (slider == &highCutoff) {
        audioProcessor.highCutoff = highCutoff.getValue();
    }
    
    if (slider == &threshold_band1) {
        audioProcessor.thresholdBand1 = threshold_band1.getValue();
    }
    
    if (slider == &threshold_band2) {
        audioProcessor.thresholdBand2 = threshold_band2.getValue();
    }
    
    if (slider == &threshold_band3) {
        audioProcessor.thresholdBand3 = threshold_band3.getValue();
    }
    
    if (slider == &ratio_band1) {
        audioProcessor.ratioBand1 = ratio_band1.getValue();
    }
    
    if (slider == &ratio_band2) {
        audioProcessor.ratioBand2 = ratio_band2.getValue();
    }
    
    if (slider == &ratio_band3) {
        audioProcessor.ratioBand3 = ratio_band3.getValue();
    }
    
    if (slider == &knee_band1) {
        audioProcessor.kneeBand1 = knee_band1.getValue();
    }
    
    if (slider == &knee_band2) {
        audioProcessor.kneeBand2 = knee_band2.getValue();
    }
    
    if (slider == &knee_band3) {
        audioProcessor.kneeBand3 = knee_band3.getValue();
    }
    
    if (slider == &attack_band1) {
        audioProcessor.attackBand1 = attack_band1.getValue();
    }
    
    if (slider == &attack_band2) {
        audioProcessor.attackBand2 = attack_band2.getValue();
    }
    
    if (slider == &attack_band3) {
        audioProcessor.attackBand3 = attack_band3.getValue();
    }
    
    if (slider == &release_band1) {
        audioProcessor.releaseBand1 = release_band1.getValue();
    }
    
    if (slider == &release_band2) {
        audioProcessor.releaseBand2 = release_band2.getValue();
    }
    
    if (slider == &release_band3) {
        audioProcessor.releaseBand3 = release_band3.getValue();
    }
    
    if (slider == &clipDrive) {
        audioProcessor.clipDrive = clipDrive.getValue();
    }
    
}

void MLMultiParallelCompAudioProcessorEditor::buttonClicked(juce::Button *button) {
    if (button == &softClip) {
        audioProcessor.clipDrive = softClip.getToggleState();
    }
}

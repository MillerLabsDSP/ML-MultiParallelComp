/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

//==============================================================================
MLMultiParallelCompAudioProcessorEditor::MLMultiParallelCompAudioProcessorEditor (MLMultiParallelCompAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(1100, 340);
    
    
    
    /* ---------------------------- */
    //          I/O GAIN            //
    /* ---------------------------- */
    
    
    
    // Input gain slider
    inputGain.setBounds(25, 30, 50, 275);
    inputGain.setRange(-60, 6);
    inputGain.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    inputGain.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 30);
    inputGain.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(inputGain);
    inputGain.addListener(this);
    
    // Input gain slider
    outputGain.setBounds(1025, 30, 50, 275);
    outputGain.setRange(-60, 6);
    outputGain.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    outputGain.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 30);
    outputGain.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(outputGain);
    outputGain.addListener(this);
    
    
    
    /* ---------------------------- */
    // PARALLEL COMPRESSION BAND #1 //
    /* ---------------------------- */
    
    
    
    // Threshold slider (band #1)
    threshold_band1.setBounds(80, 30, 120, 120);
    threshold_band1.setRange(-50, 0);
    threshold_band1.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    threshold_band1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    threshold_band1.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(threshold_band1);
    threshold_band1.addListener(this);
    
    // Ratio slider (band #1)
    ratio_band1.textFromValueFunction = [](float value)
    {
        float ratio = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return juce::String(ratio) + ":1";
    };
    
    ratio_band1.valueFromTextFunction = [](const juce::String &text)
    {
        float ratio = text.removeCharacters(":1").getFloatValue();
        return ratio;
    };
    
    ratio_band1.setBounds(110, 170, 160, 25);
    ratio_band1.setRange(1, 100);
    ratio_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    ratio_band1.setSkewFactorFromMidPoint(10.f);
    ratio_band1.setNumDecimalPlacesToDisplay(1);
    addAndMakeVisible(ratio_band1);
    ratio_band1.addListener(this);
    
    // Knee slider (band #1)
    knee_band1.textFromValueFunction = [](float value)
    {
        float knee = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return "Knee: " + juce::String(knee) + " dB";
    };
    
    knee_band1.valueFromTextFunction = [](const juce::String &text)
    {
        float knee = text.removeCharacters("Knee: dB").getFloatValue();
        return knee;
    };
    knee_band1.setBounds(110, 205, 160, 25);
    knee_band1.setRange(0, 6);
    knee_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(knee_band1);
    knee_band1.addListener(this);
    
    // Attack slider (band #1)
    attack_band1.textFromValueFunction = [](float value)
    {
        float attack = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return "Attack: " + juce::String(attack) + " ms";
    };
    
    attack_band1.valueFromTextFunction = [](const juce::String &text)
    {
        float attack = text.removeCharacters("Attack: ms").getFloatValue();
        return attack;
    };
    attack_band1.setBounds(110, 240, 160, 25);
    attack_band1.setRange(0, 500);
    attack_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(attack_band1);
    attack_band1.addListener(this);
    
    // Release slider (band #1)
    release_band1.textFromValueFunction = [](float value)
    {
        float release = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return "Release: " + juce::String(release) + " ms";
    };
    
    release_band1.valueFromTextFunction = [](const juce::String &text)
    {
        float release = text.removeCharacters("Release: ms").getFloatValue();
        return release;
    };
    release_band1.setBounds(110, 275, 160, 25);
    release_band1.setRange(0, 500);
    release_band1.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(release_band1);
    attack_band1.addListener(this);
    
    // Parallel blend slider (band #1)
    parallel_band1.setBounds(190, 50, 100, 100);
    parallel_band1.setRange(0, 6);
    parallel_band1.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    parallel_band1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    parallel_band1.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(parallel_band1);
    parallel_band1.addListener(this);

    
    
    /* ---------------------------- */
    // PARALLEL COMPRESSION BAND #2 //
    /* ---------------------------- */
    
    
    
    // Threshold slider (band #2)
    threshold_band2.setBounds(300, 30, 120, 120);
    threshold_band2.setRange(-50, 0);
    threshold_band2.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    threshold_band2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    threshold_band2.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(threshold_band2);
    threshold_band2.addListener(this);
    
    // Ratio slider (band #2)
    ratio_band2.textFromValueFunction = [](float value)
    {
        float ratio = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return juce::String(ratio) + ":1";
    };
    
    ratio_band2.valueFromTextFunction = [](const juce::String &text)
    {
        float ratio = text.removeCharacters(":1").getFloatValue();
        return ratio;
    };
    
    ratio_band2.setBounds(330, 170, 160, 25);
    ratio_band2.setRange(1, 100);
    ratio_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    ratio_band2.setSkewFactorFromMidPoint(10.f);
    ratio_band2.setNumDecimalPlacesToDisplay(1);
    addAndMakeVisible(ratio_band2);
    ratio_band2.addListener(this);
    
    // Knee slider (band #2)
    knee_band2.textFromValueFunction = [](float value)
    {
        float knee = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return "Knee: " + juce::String(knee) + " dB";
    };
    
    knee_band2.valueFromTextFunction = [](const juce::String &text)
    {
        float knee = text.removeCharacters("Knee: dB").getFloatValue();
        return knee;
    };
    knee_band2.setBounds(330, 205, 160, 25);
    knee_band2.setRange(0, 6);
    knee_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(knee_band2);
    knee_band2.addListener(this);
    
    // Attack slider (band #2)
    attack_band2.textFromValueFunction = [](float value)
    {
        float attack = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return "Attack: " + juce::String(attack) + " ms";
    };
    
    attack_band2.valueFromTextFunction = [](const juce::String &text)
    {
        float attack = text.removeCharacters("Attack: ms").getFloatValue();
        return attack;
    };
    attack_band2.setBounds(330, 240, 160, 25);
    attack_band2.setRange(0, 500);
    attack_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(attack_band2);
    attack_band2.addListener(this);
    
    // Release slider (band #2)
    release_band2.textFromValueFunction = [](float value)
    {
        float release = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return "Release: " + juce::String(release) + " ms";
    };
    
    release_band2.valueFromTextFunction = [](const juce::String &text)
    {
        float release = text.removeCharacters("Release: ms").getFloatValue();
        return release;
    };
    release_band2.setBounds(330, 275, 160, 25);
    release_band2.setRange(0, 500);
    release_band2.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(release_band2);
    release_band2.addListener(this);
      
    // Parallel blend slider (band #2)
    parallel_band2.setBounds(410, 50, 100, 100);
    parallel_band2.setRange(0, 6);
    parallel_band2.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    parallel_band2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    parallel_band2.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(parallel_band2);
    parallel_band2.addListener(this);
    
    
    
    /* ---------------------------- */
    // PARALLEL COMPRESSION BAND #3 //
    /* ---------------------------- */
    
    
    
    // Threshold slider (band #3)
    threshold_band3.setBounds(520, 30, 120, 120);
    threshold_band3.setRange(-50, 0);
    threshold_band3.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    threshold_band3.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    threshold_band3.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(threshold_band3);
    threshold_band3.addListener(this);
    
    // Ratio slider (band #3)
    ratio_band3.textFromValueFunction = [](float value)
    {
        float ratio = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return juce::String(ratio) + ":1";
    };
    
    ratio_band3.valueFromTextFunction = [](const juce::String &text)
    {
        float ratio = text.removeCharacters(":1").getFloatValue();
        return ratio;
    };
    
    ratio_band3.setBounds(550, 170, 160, 25);
    ratio_band3.setRange(1, 100);
    ratio_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    ratio_band3.setSkewFactorFromMidPoint(10.f);
    ratio_band3.setNumDecimalPlacesToDisplay(1);
    addAndMakeVisible(ratio_band3);
    ratio_band3.addListener(this);
    
    // Knee slider (band #3)
    knee_band3.textFromValueFunction = [](float value)
    {
        float knee = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return "Knee: " + juce::String(knee) + " dB";
    };
    
    knee_band3.valueFromTextFunction = [](const juce::String &text)
    {
        float knee = text.removeCharacters("Knee: dB").getFloatValue();
        return knee;
    };
    knee_band3.setBounds(550, 205, 160, 25);
    knee_band3.setRange(0, 6);
    knee_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(knee_band3);
    knee_band3.addListener(this);
    
    // Attack slider (band #3)
    attack_band3.textFromValueFunction = [](float value)
    {
        float attack = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return "Attack: " + juce::String(attack) + " ms";
    };
    
    attack_band3.valueFromTextFunction = [](const juce::String &text)
    {
        float attack = text.removeCharacters("Attack: ms").getFloatValue();
        return attack;
    };
    attack_band3.setBounds(550, 240, 160, 25);
    attack_band3.setRange(0, 500);
    attack_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(attack_band3);
    attack_band3.addListener(this);
    
    // Release slider (band #3)
    release_band3.textFromValueFunction = [](float value)
    {
        float release = static_cast<float>(static_cast<int>(value * 10.f))/10.f;
        return "Release: " + juce::String(release) + " ms";
    };
    
    release_band3.valueFromTextFunction = [](const juce::String &text)
    {
        float release = text.removeCharacters("Release: ms").getFloatValue();
        return release;
    };
    release_band3.setBounds(550, 275, 160, 25);
    release_band3.setRange(0, 500);
    release_band3.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(release_band3);
    release_band3.addListener(this);

    // Parallel blend slider (band #3)
    parallel_band3.setBounds(630, 50, 100, 100);
    parallel_band3.setRange(0, 6);
    parallel_band3.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    parallel_band3.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    parallel_band3.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(parallel_band3);
    parallel_band3.addListener(this);
    
    
    
    /* ---------------------------- */
    //          CUTOFFS             //
    /* ---------------------------- */
    
    
    
    // Low cutoff slider
    lowCutoff.textFromValueFunction = [](float value)
    {
        float lowCutoff = static_cast<float>(static_cast<int>(value * 1.f))/1.f;
        return "Low Cutoff: " + juce::String(lowCutoff) + " Hz";
    };
    
    lowCutoff.valueFromTextFunction = [](const juce::String &text)
    {
        float lowCutoff = text.removeCharacters("Low Cutoff: Hz").getFloatValue();
        return lowCutoff;
    };
    lowCutoff.setBounds(775, 170, 200, 25);
    lowCutoff.setRange(20.0, 800.f);
    lowCutoff.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(lowCutoff);
    lowCutoff.addListener(this);
    
    // High cutoff slider
    highCutoff.textFromValueFunction = [](float value)
    {
        float highCutoff = static_cast<float>(static_cast<int>(value * 1.f))/1.f;
        return "High Cutoff: " + juce::String(highCutoff) + " Hz";
    };
    
    highCutoff.valueFromTextFunction = [](const juce::String &text)
    {
        float lowCutoff = text.removeCharacters("High Cutoff: Hz").getFloatValue();
        return lowCutoff;
    };
    highCutoff.setBounds(775, 220, 200, 25);
    highCutoff.setRange(800.f, 8000.0);
    highCutoff.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    addAndMakeVisible(highCutoff);
    highCutoff.addListener(this);
    
    
    
    /* ---------------------------- */
    //          CLIPPING            //
    /* ---------------------------- */
    
    
    
    // Soft-clip button
    clip.setBounds(785, 85, 90, 90);
    clip.setButtonText("Soft Clip");
    clip.setToggleState(false,juce::dontSendNotification);
    addAndMakeVisible(clip);
    clip.addListener(this);
    
    // Soft-clip drive slider
    clipDrive.setBounds(875, 85, 90, 90);
    clipDrive.setRange(0.000001, 10000000);
    clipDrive.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    clipDrive.setTextBoxStyle(juce::Slider::NoTextBox, true, 75, 25);
    addAndMakeVisible(clipDrive);
    clipDrive.addListener(this);
    
//    clipDrive_label.setJustificationType(juce::Justification::centred);
//    clipDrive_label.setText("Drive", juce::dontSendNotification);
//    clipDrive_label.attachToComponent(&clipDrive, false);
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
    if (button == &clip) {
        audioProcessor.clip = clip.getToggleState();
    }
}

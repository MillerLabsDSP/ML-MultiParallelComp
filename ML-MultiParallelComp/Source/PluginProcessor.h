/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MultibandProcessor.h"
#include "PeakCompressor.h"

//==============================================================================
/**
 
 */
class MLMultiParallelCompAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MLMultiParallelCompAudioProcessor();
    ~MLMultiParallelCompAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================
    
private:
    
    Biquad                  LPF1 {Biquad::FilterType::LPF, 0.7071},
                            LPF2 {Biquad::FilterType::LPF, 0.7071},
                            HPF1 {Biquad::FilterType::HPF, 0.7071},
                            HPF2 {Biquad::FilterType::HPF, 0.7071},
                            mbProcessor;
    
    PeakCompressor          compressor;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MLMultiParallelCompAudioProcessor)
};

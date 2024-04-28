/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MultibandProcessor.h"
#include "PeakCompressor.h"
#include "SharedImages.h"

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
    
    SharedImages* getSharedImagesPtr() { return m_pSharedImagesPtr; };
    
    float lowCutoff = 500.f;
    float highCutoff = 3000.f;
    
    float thresholdBand1 = 0.f;
    float thresholdBand2 = 0.f;
    float thresholdBand3 = 0.f;
    
    float ratioBand1 = 1.f;
    float ratioBand2 = 1.f;
    float ratioBand3 = 1.f;
    
    float kneeBand1 = 0.f;
    float kneeBand2 = 0.f;
    float kneeBand3 = 0.f;
    
    float attackBand1 = 0.f;
    float attackBand2 = 0.f;
    float attackBand3 = 0.f;
    
    float releaseBand1 = 0.f;
    float releaseBand2 = 0.f;
    float releaseBand3 = 0.f;
    
    float clipDrive = 1000.f; // resistor value
    
    bool clip = false;
    
private:
    
    juce::SharedResourcePointer<SharedImages> m_pSharedImagesPtr;
    
    MultibandProcessor          mbProcessor;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MLMultiParallelCompAudioProcessor)
};

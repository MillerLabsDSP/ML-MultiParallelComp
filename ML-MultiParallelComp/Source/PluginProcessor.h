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
    
    juce::AudioProcessorValueTreeState apvts;
    
    SharedImages* getSharedImagesPtr() { return m_pSharedImagesPtr; };
    
    float inputGain = 0.f; // dB scale -> applied linearly in processBlock
    float outputGain = 0.f;
    
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
    
    float parallel1Val = 0.f;
    float parallel2Val = 0.f;
    float parallel3Val = 0.f;
    
    float clipDrive = 1e-7f; // resistor value
    float makeupGain = 1.f;
    bool clip = false;
    
    static const juce::StringRef INPUTGAIN;
    static const juce::StringRef OUTPUTGAIN;
    
    static const juce::StringRef LINKWITZ1;
    static const juce::StringRef LINKWITZ2;
    
    static const juce::StringRef THRESHOLD_BAND1;
    static const juce::StringRef THRESHOLD_BAND2;
    static const juce::StringRef THRESHOLD_BAND3;
    
    static const juce::StringRef PARALLEL_BAND1;
    static const juce::StringRef PARALLEL_BAND2;
    static const juce::StringRef PARALLEL_BAND3;
    
    static const juce::StringRef RATIO_BAND1;
    static const juce::StringRef RATIO_BAND2;
    static const juce::StringRef RATIO_BAND3;
    
    static const juce::StringRef KNEE_BAND1;
    static const juce::StringRef KNEE_BAND2;
    static const juce::StringRef KNEE_BAND3;
    
    static const juce::StringRef ATTACK_BAND1;
    static const juce::StringRef ATTACK_BAND2;
    static const juce::StringRef ATTACK_BAND3;
    
    static const juce::StringRef RELEASE_BAND1;
    static const juce::StringRef RELEASE_BAND2;
    static const juce::StringRef RELEASE_BAND3;
    
    static const juce::StringRef CLIPDRIVE;
    static const juce::StringRef MAKEUP;
    static const juce::StringRef CLIPTOGGLEBUTTON;
    
    void inputGainChanged(float value);
    void outputGainChanged(float value);
    
    void lowCutoffChanged(float value);
    void highCutoffChanged(float value);
    
    void thresholdBand1Changed(float value);
    void thresholdBand2Changed(float value);
    void thresholdBand3Changed(float value);
    
    void parallelBand1Changed(float value);
    void parallelBand2Changed(float value);
    void parallelBand3Changed(float value);
    
    void ratioBand1Changed(float value);
    void ratioBand2Changed(float value);
    void ratioBand3Changed(float value);
    
    void kneeBand1Changed(float value);
    void kneeBand2Changed(float value);
    void kneeBand3Changed(float value);
    
    void attackBand1Changed(float value);
    void attackBand2Changed(float value);
    void attackBand3Changed(float value);
    
    void releaseBand1Changed(float value);
    void releaseBand2Changed(float value);
    void releaseBand3Changed(float value);
    
    void clipButtonToggled(bool state);
    void clipDriveChanged(float value);
    void clipMakeupChanged(float value);

    
private:
    
    float gain = 1.f;
    
    int ParameterVersionHint = 1;
    
    juce::SharedResourcePointer<SharedImages>       m_pSharedImagesPtr;
    MultibandProcessor                              mbProcessor;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MLMultiParallelCompAudioProcessor)
};

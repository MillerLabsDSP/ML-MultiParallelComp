/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MLMultiParallelCompAudioProcessor::MLMultiParallelCompAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    
    
}
    MLMultiParallelCompAudioProcessor::~MLMultiParallelCompAudioProcessor()
{
}

//==============================================================================
const juce::String MLMultiParallelCompAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MLMultiParallelCompAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MLMultiParallelCompAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MLMultiParallelCompAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MLMultiParallelCompAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MLMultiParallelCompAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MLMultiParallelCompAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MLMultiParallelCompAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MLMultiParallelCompAudioProcessor::getProgramName (int index)
{
    return {};
}

void MLMultiParallelCompAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MLMultiParallelCompAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mbProcessor.prepareToPlay(sampleRate);
}

void MLMultiParallelCompAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MLMultiParallelCompAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MLMultiParallelCompAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
        
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto numSamples = buffer.getNumSamples();
    
    mbProcessor.setLowCutoffFrequency(lowCutoff);
    mbProcessor.setHighCutoffFrequency(highCutoff);
    
    /* COMPRESSION PARAMETERS */
    
    // Compression (band #1)
    mbProcessor.setThreshold_band1(thresholdBand1);
    mbProcessor.setRatio_band1(ratioBand1);
    mbProcessor.setKnee_band1(kneeBand1);
    mbProcessor.setAttack_band1(attackBand1);
    mbProcessor.setRelease_band1(releaseBand1);

    // Compression (band #2)
    mbProcessor.setThreshold_band2(thresholdBand2);
    mbProcessor.setRatio_band2(ratioBand2);
    mbProcessor.setKnee_band2(kneeBand2);
    mbProcessor.setAttack_band2(attackBand2);
    mbProcessor.setRelease_band2(releaseBand2);
    
    // Compression (band #3)
    mbProcessor.setThreshold_band3(thresholdBand3);
    mbProcessor.setRatio_band3(ratioBand3);
    mbProcessor.setKnee_band3(kneeBand3);
    mbProcessor.setAttack_band3(attackBand3);
    mbProcessor.setRelease_band3(releaseBand3);
    
    // Distortion
    if (clip) {
        mbProcessor.setResistorValue(clipDrive);
    } else {
        mbProcessor.setResistorValue(0.00000001f);
    }
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            
        auto* channelData = buffer.getWritePointer (channel);
        
        mbProcessor.processBuffer(channelData, numSamples, channel);

    }

}

//==============================================================================
bool MLMultiParallelCompAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MLMultiParallelCompAudioProcessor::createEditor()
{
    return new MLMultiParallelCompAudioProcessorEditor (*this);
}

//==============================================================================
void MLMultiParallelCompAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MLMultiParallelCompAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MLMultiParallelCompAudioProcessor();
}



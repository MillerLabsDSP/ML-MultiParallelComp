/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

const juce::StringRef MLMultiParallelCompAudioProcessor::INPUTGAIN = "INPUTGAIN";
const juce::StringRef MLMultiParallelCompAudioProcessor::OUTPUTGAIN = "OUTPUTGAIN";

const juce::StringRef MLMultiParallelCompAudioProcessor::LINKWITZ1 = "LINKWITZ1";
const juce::StringRef MLMultiParallelCompAudioProcessor::LINKWITZ2 = "LINKWITZ2";

const juce::StringRef MLMultiParallelCompAudioProcessor::THRESHOLD_BAND1 = "THRESHOLD_BAND1";
const juce::StringRef MLMultiParallelCompAudioProcessor::THRESHOLD_BAND2 = "THRESHOLD_BAND2";
const juce::StringRef MLMultiParallelCompAudioProcessor::THRESHOLD_BAND3 = "THRESHOLD_BAND3";

const juce::StringRef MLMultiParallelCompAudioProcessor::PARALLEL_BAND1 = "PARALLEL_BAND1";
const juce::StringRef MLMultiParallelCompAudioProcessor::PARALLEL_BAND2 = "PARALLEL_BAND2";
const juce::StringRef MLMultiParallelCompAudioProcessor::PARALLEL_BAND3 = "PARALLEL_BAND3";

const juce::StringRef MLMultiParallelCompAudioProcessor::RATIO_BAND1 = "RATIO_BAND1";
const juce::StringRef MLMultiParallelCompAudioProcessor::RATIO_BAND2 = "RATIO_BAND2";
const juce::StringRef MLMultiParallelCompAudioProcessor::RATIO_BAND3 = "RATIO_BAND3";

const juce::StringRef MLMultiParallelCompAudioProcessor::KNEE_BAND1 = "KNEE_BAND1";
const juce::StringRef MLMultiParallelCompAudioProcessor::KNEE_BAND2 = "KNEE_BAND2";
const juce::StringRef MLMultiParallelCompAudioProcessor::KNEE_BAND3 = "KNEE_BAND3";

const juce::StringRef MLMultiParallelCompAudioProcessor::ATTACK_BAND1 = "ATTACK_BAND1";
const juce::StringRef MLMultiParallelCompAudioProcessor::ATTACK_BAND2 = "ATTACK_BAND2";
const juce::StringRef MLMultiParallelCompAudioProcessor::ATTACK_BAND3 = "ATTACK_BAND3";

const juce::StringRef MLMultiParallelCompAudioProcessor::RELEASE_BAND1 = "RELEASE_BAND1";
const juce::StringRef MLMultiParallelCompAudioProcessor::RELEASE_BAND2 = "RELEASE_BAND2";
const juce::StringRef MLMultiParallelCompAudioProcessor::RELEASE_BAND3 = "RELEASE_BAND3";

const juce::StringRef MLMultiParallelCompAudioProcessor::CLIPTOGGLEBUTTON = "CLIPTOGGLEBUTTON";
const juce::StringRef MLMultiParallelCompAudioProcessor::MAKEUP = "MAKEUP";
const juce::StringRef MLMultiParallelCompAudioProcessor::CLIPDRIVE = "CLIPDRIVE";

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
                       ),
#endif
apvts(*this, nullptr, "Params", createParams()) {
    
}
    MLMultiParallelCompAudioProcessor::~MLMultiParallelCompAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout MLMultiParallelCompAudioProcessor::createParams() {
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"INPUTGAIN", ParameterVersionHint}, "Input Gain", juce::NormalisableRange<float>(-48.f, 6.f, 0.1f), 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"OUTPUTGAIN", ParameterVersionHint}, "Output Gain", juce::NormalisableRange<float>(-48.f, 6.f, 0.1f), 0.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"LINKWITZ1", ParameterVersionHint}, "Low-Mid Crossover Frequency", 20.f, 800.f, 20.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"LINKWITZ2", ParameterVersionHint}, "Mid-High Crossover Frequency", 800.f, 8000.f, 800.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"PARALLEL_BAND1", ParameterVersionHint}, "Low Parallel Blend", juce::NormalisableRange<float>(0.f, 10.f, 0.1f), 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"PARALLEL_BAND2", ParameterVersionHint}, "Low Parallel Blend", juce::NormalisableRange<float>(0.f, 10.f, 0.1f), 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"PARALLEL_BAND3", ParameterVersionHint}, "Low Parallel Blend", juce::NormalisableRange<float>(0.f, 10.f, 0.1f), 0.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"THRESHOLD_BAND1", ParameterVersionHint}, "Low Threshold", juce::NormalisableRange<float>(-50.f, 0.f, 0.1f), 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"THRESHOLD_BAND2", ParameterVersionHint}, "Mid Threshold", juce::NormalisableRange<float>(-50.f, 0.f, 0.1f), 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"THRESHOLD_BAND3", ParameterVersionHint}, "High Threshold", juce::NormalisableRange<float>(-50.f, 0.f, 0.1f), 0.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"RATIO_BAND1", ParameterVersionHint}, "Low Ratio", juce::NormalisableRange<float>(2.f, 20.f, 0.1f), 2.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"RATIO_BAND2", ParameterVersionHint}, "Mid Ratio", juce::NormalisableRange<float>(2.f, 20.f, 0.1f), 2.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"RATIO_BAND3", ParameterVersionHint}, "High Ratio", juce::NormalisableRange<float>(2.f, 20.f, 0.1f), 2.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"KNEE_BAND1", ParameterVersionHint}, "Low Knee", 0.f, 18.f, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"KNEE_BAND2", ParameterVersionHint}, "Mid Knee", 0.f, 18.f, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"KNEE_BAND3", ParameterVersionHint}, "High Knee", 0.f, 18.f, 0.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"ATTACK_BAND1", ParameterVersionHint}, "Low Attack", juce::NormalisableRange<float>(0.01f, 500.f, 0.01f), 2.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"ATTACK_BAND2", ParameterVersionHint}, "Mid Attack", juce::NormalisableRange<float>(0.01f, 500.f, 0.01f), 2.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"ATTACK_BAND3", ParameterVersionHint}, "High Attack", juce::NormalisableRange<float>(0.01f, 500.f, 0.01f), 2.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"RELEASE_BAND1", ParameterVersionHint}, "Low Release", juce::NormalisableRange<float>(0.1f, 1000.f, 0.01f), 30.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"RELEASE_BAND2", ParameterVersionHint}, "Mid Release", juce::NormalisableRange<float>(0.1f, 1000.f, 0.01f), 30.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"RELEASE_BAND3", ParameterVersionHint}, "High Release", juce::NormalisableRange<float>(0.1f, 1000.f, 0.01f), 30.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"CLIPDRIVE", ParameterVersionHint}, "Clipper Drive",juce::NormalisableRange<float>(1e-7f, 1e7f, 100.f), 1e-7f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"MAKEUP", ParameterVersionHint}, "Clipper Makeup Gain", juce::NormalisableRange<float>(1.f, 10.f, 0.1f), 1.f));
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"CLIPTOGGLEBUTTON", ParameterVersionHint}, "Clip Toggle", false));
    
    return { params.begin(), params.end() };
    
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
    
    mbProcessor.setInputGain(inputGain);
    mbProcessor.setOutputGain(outputGain);
    
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
    
    // Parallel readdition
    mbProcessor.setParallel_1(parallel1Val);
    mbProcessor.setParallel_2(parallel2Val);
    mbProcessor.setParallel_3(parallel3Val);
    
    // Distortion
    mbProcessor.setResistorValue(clipDrive);
    mbProcessor.setMakeupValue(makeupGain);
    mbProcessor.setClipState(clip);
    
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
    
    auto currentState = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (currentState.createXml());
    copyXmlToBinary(*xml, destData);
    
}

void MLMultiParallelCompAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    
    getXmlFromBinary(data, sizeInBytes);
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary(data, sizeInBytes));
    juce::ValueTree newTree = juce::ValueTree::fromXml(*xml);
    apvts.replaceState(newTree);
    
}

void MLMultiParallelCompAudioProcessor::inputGainChanged(float value) {
    inputGain = value;
}

void MLMultiParallelCompAudioProcessor::outputGainChanged(float value) {
    outputGain = value;
}

void MLMultiParallelCompAudioProcessor::lowCutoffChanged(float value) {
    lowCutoff = value;
}

void MLMultiParallelCompAudioProcessor::highCutoffChanged(float value) {
    highCutoff = value;
}

void MLMultiParallelCompAudioProcessor::thresholdBand1Changed(float value) {
    thresholdBand1 = value;
}

void MLMultiParallelCompAudioProcessor::thresholdBand2Changed(float value) {
    thresholdBand2 = value;
}

void MLMultiParallelCompAudioProcessor::thresholdBand3Changed(float value) {
    thresholdBand3 = value;
}

void MLMultiParallelCompAudioProcessor::parallelBand1Changed(float value) {
    parallel1Val = value;
}

void MLMultiParallelCompAudioProcessor::parallelBand2Changed(float value) {
    parallel2Val = value;
}

void MLMultiParallelCompAudioProcessor::parallelBand3Changed(float value) {
    parallel3Val = value;
}

void MLMultiParallelCompAudioProcessor::ratioBand1Changed(float value) {
    ratioBand1 = value;
}

void MLMultiParallelCompAudioProcessor::ratioBand2Changed(float value) {
    ratioBand2 = value;
}

void MLMultiParallelCompAudioProcessor::ratioBand3Changed(float value) {
    ratioBand3 = value;
}

void MLMultiParallelCompAudioProcessor::kneeBand1Changed(float value) {
    kneeBand1 = value;
}

void MLMultiParallelCompAudioProcessor::kneeBand2Changed(float value) {
    kneeBand2 = value;
}

void MLMultiParallelCompAudioProcessor::kneeBand3Changed(float value) {
    kneeBand3 = value;
}

void MLMultiParallelCompAudioProcessor::attackBand1Changed(float value) {
    attackBand1 = value;
}

void MLMultiParallelCompAudioProcessor::attackBand2Changed(float value) {
    attackBand2 = value;
}

void MLMultiParallelCompAudioProcessor::attackBand3Changed(float value) {
    attackBand3 = value;
}

void MLMultiParallelCompAudioProcessor::releaseBand1Changed(float value) {
    releaseBand1 = value;
}

void MLMultiParallelCompAudioProcessor::releaseBand2Changed(float value) {
    releaseBand2 = value;
}

void MLMultiParallelCompAudioProcessor::releaseBand3Changed(float value) {
    releaseBand3 = value;
}

void MLMultiParallelCompAudioProcessor::clipButtonToggled(bool state) {
    clip = state;
}

void MLMultiParallelCompAudioProcessor::clipMakeupChanged(float value) {
    makeupGain = value;
}

void MLMultiParallelCompAudioProcessor::clipDriveChanged(float value) {
    clipDrive = value;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MLMultiParallelCompAudioProcessor();
}



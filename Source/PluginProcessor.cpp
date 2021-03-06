/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
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
    for (int i = 0; i < p.maxNumBirds; i++)
    {
        birds.emplace_back(birds.size(), &p, &birds, &birdMap, &birdMapCount);
    }
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mySampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;

    ldrFilterDSP.prepare(spec);
    ldrFilterDSP.reset();
    ldrFilterDSP.setMode(juce::dsp::LadderFilterMode::LPF24);
    lp.clear();
    lp.setSize(2, samplesPerBlock);
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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


void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    const float bMax = buffer.getMagnitude(0,buffer.getNumSamples());
    p.lastBufferMaxAmp = bMax;

    const float volumeSpeedDelta = p.volumeSpeedMod * p.lastBufferMaxAmp * p.velocityMax * 0.5f;
    const float volumeAvoidDelta = p.volumeAvoidMod * p.lastBufferMaxAmp * p.avoidMax * 0.5f;
    const float volumeMatchDelta = p.volumeMatchMod * p.lastBufferMaxAmp * p.matchVelocityMax * 0.5f;
    const float volumeGroupDelta = p.volumeGroupMod * p.lastBufferMaxAmp * p.groupMax * 0.5f;
    const float volumeColorDelta = p.volumeColorMod * p.lastBufferMaxAmp;


    lp.makeCopyOf(buffer, true);
    juce::dsp::AudioBlock<float> block(lp);
    const juce::dsp::ProcessContextReplacing<float> processContext{ block };
    ldrFilterDSP.setCutoffFrequencyHz(p.lpCuttoff);
    ldrFilterDSP.process(processContext);

    const float lpMax = lp.getMagnitude(0, buffer.getNumSamples());

    p.lpMaxAmp = lpMax;

    const float lpSpeedDelta = p.lpSpeedMod * p.lpMaxAmp * p.velocityMax * 0.5f;
    const float lpAvoidDelta = p.lpAvoidMod * p.lpMaxAmp * p.avoidMax * 0.5f;
    const float lpMatchDelta = p.lpMatchMod * p.lpMaxAmp * p.matchVelocityMax * 0.5f;
    const float lpGroupDelta = p.lpGroupMod * p.lpMaxAmp * p.groupMax * 0.5f;

    p.moddedVelocity = p.defaultVelocity + lpSpeedDelta * p.lpTotalWeight + volumeSpeedDelta * p.volumeTotalWeight;
    p.moddedAvoid = p.avoidWeight + lpAvoidDelta * p.lpTotalWeight + volumeAvoidDelta * p.volumeTotalWeight;
    p.moddedMatch = p.matchVelocityWeight + lpMatchDelta * p.lpTotalWeight + volumeMatchDelta * p.volumeTotalWeight;
    p.moddedGroup = p.groupWeight + lpGroupDelta * p.lpTotalWeight + volumeGroupDelta * p.volumeTotalWeight;
    p.moddedColor = volumeColorDelta * p.volumeTotalWeight;
    
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}

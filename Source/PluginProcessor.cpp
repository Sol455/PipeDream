/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PipeDreamAudioProcessor::PipeDreamAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
, apvts(*this, nullptr, "PARAMETERS", createParameterLayout()                       )
#endif
{
    variableTree = {
        
        "Variables", {},
        {
            {"Group", {{ "name", "IR Vars"}},
                {
                    {"Parameter", {{"id", "file1"}, {"value", "/"}}},
                    {"Parameter", {{"id", "root"}, {"value", "/"}}},
                }
            }
        }
    };
}

PipeDreamAudioProcessor::~PipeDreamAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
    PipeDreamAudioProcessor::createParameterLayout()
{
        std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
        
//        auto inputGainParm = std::make_unique<juce::AudioParameterFloat>(inputGainSliderId, inputGainSliderName, -24.0f, 24.0f, 0.0f);
//
//        params.push_back(std::move(inputGainParm));
        
        return {params.begin(), params.end() };
}

void PipeDreamAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    
}
//==============================================================================
const juce::String PipeDreamAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PipeDreamAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PipeDreamAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PipeDreamAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PipeDreamAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PipeDreamAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PipeDreamAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PipeDreamAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PipeDreamAudioProcessor::getProgramName (int index)
{
    return {};
}

void PipeDreamAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PipeDreamAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    irLoader.reset();
    irLoader.prepare(spec);
    
    if (savedFile.existsAsFile()) {
        irLoader.loadImpulseResponse(savedFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
    }
}

void PipeDreamAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PipeDreamAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void PipeDreamAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock<float> block {buffer};
    if (irLoader.getCurrentIRSize() > 0)
    {
        irLoader.process(juce::dsp::ProcessContextReplacing<float>(block));
    }
//    // In case we have more outputs than inputs, this code clears any output
//    // channels that didn't contain input data, (because these aren't
//    // guaranteed to be empty - they may contain garbage).
//    // This is here to avoid people getting screaming feedback
//    // when they first compile a plugin, but obviously you don't need to keep
//    // this code if your algorithm always overwrites all the output channels.
//    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
//        buffer.clear (i, 0, buffer.getNumSamples());
//
//    // This is the place where you'd normally do the guts of your plugin's
//    // audio processing...
//    // Make sure to reset the state if your inner loop is processing
//    // the samples and the outer loop is handling the channels.
//    // Alternatively, you can process the samples with the channels
//    // interleaved by keeping the same state.
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
//    }
}

//==============================================================================
bool PipeDreamAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PipeDreamAudioProcessor::createEditor()
{
    return new PipeDreamAudioProcessorEditor (*this);
}

//==============================================================================
void PipeDreamAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    apvts.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    apvts.state.writeToStream (stream);
}

void PipeDreamAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    variableTree = tree.getChildWithName("variables");
    
    if (tree.isValid())
    {
        apvts.state = tree;
        
        savedFile = variableTree.getProperty("file1");
        root = variableTree.getProperty("root");
        
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PipeDreamAudioProcessor();
}

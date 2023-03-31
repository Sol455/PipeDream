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
    
    //Retrieve APVTS stored paramaters and assign them to cached pointers    //Assert that parameter names are spelt correctly
        
        using namespace Params;
        const auto& params = GetParams();
        
        auto IntHelper = [&apvts = this->apvts, &params](auto& param, const auto& paramName)
        {
            param = dynamic_cast<juce::AudioParameterInt*>(apvts.getParameter(params.at(paramName)));
            jassert(param !=nullptr);
        };
    
    IntHelper(pitchsel1, Names::Pitch_Sel_1);
    
    
    
    //juce::AudioParameterInt
        
    
//    auto choiceHelper = [&apvts = this->apvts, &params](auto& param, const auto& paramName)
//    {
//        param = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(params.at(paramName)));
//        jassert(param !=nullptr);
//    };
    
    
        //IntHelper(pitchsel1, Names::Pitch_Sel_1);
    
        //lowMidCrossover->get();
        //floatHelper(lowBandComp.release, Names::Release_Low_Band);
        //floatHelper(lowBandComp.threshold, Names::Threshold_Low_Band);
    
        

}

PipeDreamAudioProcessor::~PipeDreamAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
    PipeDreamAudioProcessor::createParameterLayout()
{
        //std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
        
//        auto inputGainParm = std::make_unique<juce::AudioParameterFloat>(inputGainSliderId, inputGainSliderName, -24.0f, 24.0f, 0.0f);
          //params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID{ "Pitch Select 1", 1}, "Pitch_Select_1", -12, 12, 0));

//
//        params.push_back(std::move(inputGainParm));
        
        //return {params.begin(), params.end() };
        
        
        juce::AudioProcessorValueTreeState::ParameterLayout layout;

        using namespace juce;
        using namespace Params;
        const auto& params = GetParams();
        
        //auto gainRange = NormalisableRange<float>(-24.f, 24.f, 0.5, 1);
        
        //auto pitchRange = NormalisableRange<float>(-24, 24, 1, 1);
        
//        layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{params.at(Names::Pitch_Sel_1),1},
//                                                                                params.at(Names::Pitch_Sel_1),
//                                                                                pitchRange,
//                                                                                0));
        
        
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{params.at(Names::Pitch_Sel_1),1},
                                                            params.at(Names::Pitch_Sel_1),
                                                            -24,
                                                            24,
                                                             0));
        
        //<juce::AudioParameterFloat(<#const ParameterID &parameterID#>, <#const String &parameterName#>, <#NormalisableRange<float> normalisableRange#>, <#float defaultValue#>)
        
        return layout;
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
    //0,1 2
    readIRFromFile(2, 0);
    readIRFromFile(2, 1);
    readIRFromFile(2, 2);
    irLoader.reset();
    irLoader.prepare(spec);
    
    //auto g = apvts.getRawParameterValue("PITCHSEL1");
    
    //apvts.getRawParameterValue(<#StringRef parameterID#>)
    
    
}
    
void PipeDreamAudioProcessor::readIRFromFile(int IRNum, int IRtoWrite) {
    
    currentIR = FilePath + IRNames[IRNum];
    
    if(currentIR.existsAsFile())
    {
        
        juce::AudioFormatManager manager;
        manager.registerBasicFormats();
        juce::AudioFormatReader *reader { manager.createReaderFor (std::move (currentIR)) };

//        if (reader == nullptr)
//        {
//            jassertfalse;
//            return;
//        }

        
        bufferStore.SetInfoAll(static_cast<int> (reader->numChannels), static_cast<int>(reader->lengthInSamples));
        
        repitchBuffer(reader, 0, 44000);
        reader->read (bufferStore.BufBankWriteP(IRtoWrite), bufferStore.getChannels(IRtoWrite), 0, bufferStore.getSamples(IRtoWrite));
        
        
        bufferStore.SetSampleRate(IRtoWrite, reader->sampleRate);
    }
}
//std::unique_ptr<juce::AudioFormatReader>
void PipeDreamAudioProcessor::repitchBuffer(juce::AudioFormatReader *reader, int bufferNum, const double& dOutSampleRate) {
    
        juce::AudioSampleBuffer temp;
    
        //sofar = 0;
    
        //clear buffers
        bufferStore.BufBankReadP(bufferNum).clear();
        //waveBuffer.clear();
        temp.clear();
    
        double ratio =  reader->sampleRate / dOutSampleRate;
    
        temp.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
    
        //next to do
        bufferStore.SetBufferSize(bufferNum, static_cast<int> (reader->numChannels), ((static_cast<int>(reader->lengthInSamples))/ ratio));
        
    
    //waveBuffer.setSize((int)reader->numChannels, (int)(((double)reader->lengthInSamples) / ratio));
    
        reader->read(&temp, 0, (int)reader->lengthInSamples, 0, true, true);
    
        juce::ScopedPointer<juce::LagrangeInterpolator> resampler = new juce::LagrangeInterpolator();
    
        const float *const * inputs  = bufferStore.BufBankarrayReadP(bufferNum); //temp.getArrayOfReadPointers();
        float *const*  outputs = bufferStore.BufBankWriteP(bufferNum);
        //float **outputs = waveBuffer.getArrayOfWritePointers();
        for (int c = 0; c < bufferStore.getChannels(bufferNum); c++)
        {
            resampler->reset();
            resampler->process(ratio, inputs[c], outputs[c], bufferStore.getSamples(bufferNum)); //.getNumSamples());
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
    
    //auto g = apvts.getRawParameterValue("PITCHSEL1");
    //std::cout << g->load() <<std::endl;
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    auto pitches1 = pitchsel1->get();
    
    bufferTransfer.set (BufferWithSampleRate { std::move (bufferStore.BufBankReadP(pitches1)),
                                                bufferStore.GetSampleRate(pitches1)});

    juce::dsp::AudioBlock<float> block {buffer};
    
    bufferTransfer.get ([this] (BufferWithSampleRate& buf)
    {
        irLoader.loadImpulseResponse (std::move (buf.buffer),
                                         buf.sampleRate,
                                         juce::dsp::Convolution::Stereo::yes,
                                         juce::dsp::Convolution::Trim::yes,
                                         juce::dsp::Convolution::Normalise::yes);
    });
    
    irLoader.process(juce::dsp::ProcessContextReplacing<float>(block));

    
    
//    if (irLoader.getCurrentIRSize() > 0)
//    {
//        irLoader.process(juce::dsp::ProcessContextReplacing<float>(block));
//    }
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
        
        //irLoader.loadImpulseResponse(savedFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
        
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PipeDreamAudioProcessor();
}

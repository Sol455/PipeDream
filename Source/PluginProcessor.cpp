/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "cmath"

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
    IntHelper(pitchsel2, Names::Pitch_Sel_2);
    IntHelper(pitchsel3, Names::Pitch_Sel_3);
    IntHelper(pitchsel4, Names::Pitch_Sel_4);
    IntHelper(pitchsel5, Names::Pitch_Sel_5);

}

PipeDreamAudioProcessor::~PipeDreamAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
    PipeDreamAudioProcessor::createParameterLayout()
{

        
        juce::AudioProcessorValueTreeState::ParameterLayout layout;

        using namespace juce;
        using namespace Params;
        const auto& params = GetParams();
       

        
        
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{params.at(Names::Pitch_Sel_1),1},
                                                            params.at(Names::Pitch_Sel_1),
                                                            -12,
                                                            12,
                                                             0));
        
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{params.at(Names::Pitch_Sel_2),1},
                                                            params.at(Names::Pitch_Sel_2),
                                                            -12,
                                                            12,
                                                             0));
        
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{params.at(Names::Pitch_Sel_3),1},
                                                            params.at(Names::Pitch_Sel_3),
                                                            -12,
                                                            12,
                                                             0));
    
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{params.at(Names::Pitch_Sel_4),1},
                                                            params.at(Names::Pitch_Sel_4),
                                                            -12,
                                                            12,
                                                             0));
        
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{params.at(Names::Pitch_Sel_5),1},
                                                            params.at(Names::Pitch_Sel_5),
                                                            -12,
                                                            12,
                                                             0));

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
    readIRFromFile(0, 0);
    //readIRFromFile(1, 1);
    //readIRFromFile(2, 2);
    irLoader.reset();
    irLoader.prepare(spec);
    
    //auto g = apvts.getRawParameterValue("PITCHSEL1");
    //apvts.getRawParameterValue(<#StringRef parameterID#>)
    
}
    
void PipeDreamAudioProcessor::readIRFromFile(int IRNum, int bufferNum) {
    
    currentIR = FilePath + IRNames[IRNum];
    
    if(currentIR.existsAsFile())
    {
        juce::AudioFormatManager manager;
        manager.registerBasicFormats();
        //std::unique_ptr<juce::AudioFormatReader> reader { manager.createReaderFor (std::move (currentIR)) };
        juce::AudioFormatReader *reader { manager.createReaderFor (std::move (currentIR)) };

        if (reader == nullptr)
        {
            jassertfalse;
            return;
        }
        
        //set all buffer classes to IR sample rate
        //bufferStore.SetInfoAll(static_cast<int> (reader->numChannels), static_cast<int>(reader->lengthInSamples));

        //read the File to the buffer
//        reader->read (bufferStore.BufBankWriteP(IRtoWrite), bufferStore.getChannels(IRtoWrite), 0, bufferStore.getSamples(IRtoWrite));
//        bufferStore.SetSampleRate(IRtoWrite, reader->sampleRate);
        
        repitchBuffer(reader, 0, -12);
        repitchBuffer(reader, 1, -11);
        repitchBuffer(reader, 2, -10);
        repitchBuffer(reader, 3, -9);
        repitchBuffer(reader, 4, -8);
        repitchBuffer(reader, 5, -7);
        repitchBuffer(reader, 6, -6);
        repitchBuffer(reader, 7, -5);
        repitchBuffer(reader, 8, -4);
        repitchBuffer(reader, 9, -3);
        repitchBuffer(reader, 10, -2);
        repitchBuffer(reader, 11, -1);
        repitchBuffer(reader, 12, 0);
        repitchBuffer(reader, 13, 1);
        repitchBuffer(reader, 14, 2);
        repitchBuffer(reader, 15, 3);
        repitchBuffer(reader, 16, 4);
        repitchBuffer(reader, 17, 5);
        repitchBuffer(reader, 18, 6);
        repitchBuffer(reader, 19, 7);
        repitchBuffer(reader, 20, 8);
        repitchBuffer(reader, 21, 8);
        repitchBuffer(reader, 22, 10);
        repitchBuffer(reader, 23, 11);
        repitchBuffer(reader, 24, 12);
//        for (int i = 1; i < 12; i ++) {
//            repitchBuffer(reader, i, -(12 - i));
//       }
//
//        for (int i = 12; i <= 24; i++) {
//            repitchBuffer(reader, i, (i - 12));
//       }
        
//            repitchBuffer(reader, 4, 100000, -2);
//            repitchBuffer(reader, 5, 200000, 3);
         delete reader;
    }
}
//std::unique_ptr<juce::AudioFormatReader>
void PipeDreamAudioProcessor::repitchBuffer(juce::AudioFormatReader *reader, int bufferNum, int semitoneIn) {
    
    float semitoneToPhase;
    semitoneToPhase = pow(2.0, semitoneIn/12.0);
    std::cout << "\t\n\nsemitone:" << semitoneToPhase;
    
    juce::AudioSampleBuffer temp;
    //clear buffers
    bufferStore.BufBankReadP(bufferNum).clear();
    temp.clear();
    
    double ratio =  semitoneToPhase;//dOutSampleRate; reader->sampleRate / dOutSampleRate;
    std::cout << "\t\n\nre-campling ratio:" << semitoneToPhase;

    //set buffer Sizes
    temp.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
    bufferStore.SetBufferSize(bufferNum, static_cast<int> (reader->numChannels), (static_cast<int>(reader->lengthInSamples)/ ratio));
    
    //read file into tempory buffer
    reader->read(&temp, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);
    
    juce::LagrangeInterpolator interpolator;
                int iResult = 0;
                std::cout << reader->numChannels;
                for (int i = 0; i < reader->numChannels; i++) {
                    iResult = interpolator.process(ratio,
                        temp.getReadPointer(i),
                        bufferStore.BufBankBufferWriteP1(bufferNum, i),
                        bufferStore.getSamples(bufferNum));
                }
    
     bufferStore.SetSampleRate(bufferNum, reader->sampleRate);
    
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
    
    auto g = apvts.getRawParameterValue("Pitch_Sel_1");
    //std::cout << g->load() <<std::endl;
    
    juce::ScopedNoDenormals noDenormals;
    
    //auto g = apvts.getRawParameterValue("PITCHSEL1");
    //std::cout << g->load() <<std::endl;
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    auto pitches1 = pitchsel1->get() + 12;
    
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

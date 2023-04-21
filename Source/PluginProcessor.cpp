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
    
    auto floatHelper = [&apvts = this->apvts, &params](auto& param, const auto& paramName)
       {
           param = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(params.at(paramName)));
           jassert(param !=nullptr);
       };
    
    auto choiceHelper = [&apvts = this->apvts, &params](auto& param, const auto& paramName)
    {
        param = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(params.at(paramName)));
        jassert(param !=nullptr);
    };
    
    auto boolHelper = [&apvts = this->apvts, &params](auto& param, const auto& paramName)
    {
        param = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(params.at(paramName)));
        jassert(param !=nullptr);
    };
    
    IntHelper(pitchsel1, Names::Pitch_Sel_1);
    IntHelper(pitchsel2, Names::Pitch_Sel_2);
    IntHelper(pitchsel3, Names::Pitch_Sel_3);
    IntHelper(pitchsel4, Names::Pitch_Sel_4);
    IntHelper(pitchsel5, Names::Pitch_Sel_5);
    
    floatHelper(outGain1, Names::Gain_Out_1);
    floatHelper(outGain2, Names::Gain_Out_2);
    floatHelper(outGain3, Names::Gain_Out_3);
    floatHelper(outGain4, Names::Gain_Out_4);
    floatHelper(outGain5, Names::Gain_Out_5);
    
    floatHelper(DryWet, Names::Dry_Wet);
    floatHelper(DecayTime, Names::Decay_Time);
    
    floatHelper(LowPassCutOff, Names::Low_Pass_Cut_Off);
    floatHelper(HighPassCutOff, Names::High_Pass_Cut_Off);
    
    choiceHelper(ChordSel, Names::Chord_Sel);
    choiceHelper(rootSel, Names::Root_Sel);
    
    boolHelper(ChordHold, Names::Chord_Hold);

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
        
        auto gainRange = NormalisableRange<float>(-24.f, 24.f, 0.5, 1);
        
        //gain
        layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterID {params.at(Names::Gain_Out_1), 1},
                                                        params.at(Names::Gain_Out_1),
                                                        gainRange,
                                                        0 ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterID {params.at(Names::Gain_Out_2), 1},
                                                        params.at(Names::Gain_Out_2),
                                                        gainRange,
                                                        0 ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterID {params.at(Names::Gain_Out_3), 1},
                                                        params.at(Names::Gain_Out_3),
                                                        gainRange,
                                                        0 ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterID {params.at(Names::Gain_Out_4), 1},
                                                        params.at(Names::Gain_Out_4),
                                                        gainRange,
                                                        0 ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterID {params.at(Names::Gain_Out_5), 1},
                                                        params.at(Names::Gain_Out_5),
                                                        gainRange,
                                                        0 ));
       

        //current pitch
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{params.at(Names::Pitch_Sel_1),1},
                                                            params.at(Names::Pitch_Sel_1),
                                                            -12,
                                                            24,
                                                             0));
        
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{params.at(Names::Pitch_Sel_2),1},
                                                            params.at(Names::Pitch_Sel_2),
                                                            -12,
                                                            24,
                                                             0));
        
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{params.at(Names::Pitch_Sel_3),1},
                                                            params.at(Names::Pitch_Sel_3),
                                                            -12,
                                                            24,
                                                             0));
    
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{params.at(Names::Pitch_Sel_4),1},
                                                            params.at(Names::Pitch_Sel_4),
                                                            -12,
                                                            24,
                                                             0));
        
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{params.at(Names::Pitch_Sel_5),1},
                                                            params.at(Names::Pitch_Sel_5),
                                                            -12,
                                                            24,
                                                             0));
        
        //chords
    

        juce::StringArray chords = {"Mono","5th","Sus2","Minor","Maj","Sus4","Maj7","min7","7sus"};
        juce::StringArray roots = {"C","C#","D","D#","E","F","F#","G","G#","A", "A#",       "B","C","C#","D","D#","E","F","F#","G","G#","A", "A#", "B","C"};


        layout.add(std::make_unique<AudioParameterChoice>(ParameterID {params.at(Names::Chord_Sel), 1},
                                                          params.at(Names::Chord_Sel),
                                                          chords,
                                                          3));
        
        layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{params.at(Names::Root_Sel),1},
                                                            params.at(Names::Root_Sel),
                                                            roots,
                                                            0));
        
        
        layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{params.at(Names::Chord_Hold),1},
                                                            params.at(Names::Chord_Hold),
                                                            false));
        
        //Dry/ wet
        
        auto dryWetRange = NormalisableRange<float>(0.f, 1.f, 0.01);

        layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterID {params.at(Names::Dry_Wet), 1},
                                                        params.at(Names::Dry_Wet),
                                                        dryWetRange,
                                                        0.5 ));
        
        //Decay
        
        auto decayRange = NormalisableRange<float>(0.f, 3.f, 0.01);

        
        layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterID {params.at(Names::Decay_Time), 1},
                                                        params.at(Names::Decay_Time),
                                                        decayRange,
                                                        1 ));
        
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID {params.at(Names::Low_Pass_Cut_Off), 1},
                                                             params.at(Names::Low_Pass_Cut_Off),
                                                             NormalisableRange<float>(0, 2000, 1, 1), 400));
        
        
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID {params.at(Names::High_Pass_Cut_Off), 1},
                                                             params.at(Names::High_Pass_Cut_Off),
                                                             NormalisableRange<float>(0, 2000, 1, 1), 400));
        
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
    
    soundtouch.setSampleRate(sampleRate);
    soundtouch.setChannels(1);
    
    dry_wet_mixer.prepare(spec);
    readIRFromFile(2, 0);
    ParallelConvs.prepare(spec);
    
    for(auto& buffer: audioSplitBuffers)
    {
        buffer.setSize(spec.numChannels, samplesPerBlock);
    }
    
    setCurrentIRs();
    setDecay(pitchsel1->get());
    setDecay(pitchsel2->get());
    setDecay(pitchsel3->get());
    setDecay(pitchsel4->get());
    setDecay(pitchsel5->get());
    
}
    
void PipeDreamAudioProcessor::readIRFromFile(int IRNum, int bufferNum) {
    
    currentIR = FilePath + IRNames[IRNum];
    
    if(currentIR.existsAsFile())
    {
        juce::AudioFormatManager manager;
        manager.registerBasicFormats();
        juce::AudioFormatReader *reader { manager.createReaderFor (std::move (currentIR)) };

        if (reader == nullptr)
        {
            jassertfalse;
            return;
        }
        
        //trim buffer below 1sec
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
        repitchBuffer(reader, 21, 9);
        repitchBuffer(reader, 22, 10);
        repitchBuffer(reader, 23, 11);
        repitchBuffer(reader, 24, 12);
        repitchBuffer(reader, 25, 13);
        repitchBuffer(reader, 26, 14);
        repitchBuffer(reader, 27, 15);
        repitchBuffer(reader, 28, 16);
        repitchBuffer(reader, 29, 17);
        repitchBuffer(reader, 30, 18);
        repitchBuffer(reader, 31, 19);
        repitchBuffer(reader, 32, 20);
        repitchBuffer(reader, 33, 21);
        repitchBuffer(reader, 34, 22);
        repitchBuffer(reader, 35, 23);
        repitchBuffer(reader, 36, 24);
        
//        for (int i = 0; i < 12; i ++) {
//            repitchBuffer(reader, i, (-12 + i));
//       }
//
//        for (int i = 12; i <= 24; i++) {
//            repitchBuffer(reader, i, (i - 12));
//       }
        
//            repitchBuffer(reader, 4, 100000, -2);
//            repitchBuffer(reader, 5, 200000, 3);
        
        for (int i = 0; i < 37; i++) {
            normaliseAndTrim(i);
        }
        std::cout << "\n\nDONE!!\n\n";
         delete reader;
        
        setCurrentIRs();
    }
}

void PipeDreamAudioProcessor::repitchBuffer(juce::AudioFormatReader *reader, int bufferNum, int semitoneIn) {
    
    float semitoneToPhase;
    semitoneToPhase = pow(2.0, semitoneIn/12.0);
    
    juce::AudioSampleBuffer temp;
    //1. clear buffers
    bufferStore.BufBankReadP(bufferNum).clear();
    referenceBuffers.BufBankReadP(bufferNum).clear();

    temp.clear();
    
    double ratio =  semitoneToPhase;//dOutSampleRate; reader->sampleRate / dOutSampleRate;

    //2. set buffer Sizes
    temp.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
    bufferStore.SetBufferSize(bufferNum, static_cast<int> (reader->numChannels), (static_cast<int>(reader->lengthInSamples)/ ratio));
    referenceBuffers.SetBufferSize(bufferNum, static_cast<int> (reader->numChannels), (static_cast<int>(reader->lengthInSamples)/ ratio));
    
    //3. read file into tempory buffer
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
     referenceBuffers.SetSampleRate(bufferNum, reader->sampleRate);
}

void PipeDreamAudioProcessor::normaliseAndTrim(int bufferNum) {

        //trim
        int numSamples = bufferStore.getSamples(bufferNum);
        int blockSize = static_cast<int>(std::floor(this->getSampleRate()) / 100);
        int endBlockNum = numSamples / blockSize;
        //std::cout << "\nOrigional Length:" << numSamples;

        float localMaxMagnitude = 0.0f;
        while ((endBlockNum - 1) * blockSize > 0) {
            --endBlockNum;
            localMaxMagnitude = bufferStore.BufBankReadP(bufferNum).getMagnitude(endBlockNum * blockSize, blockSize);
            if (localMaxMagnitude > 0.001) {
                break;
            }
        }
        
    int trimmedNumSamples = endBlockNum * blockSize - 1;
    //std::cout << "\ntrimmed length :" << trimmedNumSamples;
    int numChannels =  bufferStore.getChannels(bufferNum);
    bufferStore.SetInfo1(bufferNum, numChannels, trimmedNumSamples);
    
    //normalize
    float MaxMagnitude = bufferStore.BufBankReadP(bufferNum).getMagnitude(0, bufferStore.getSamples(bufferNum));
    bufferStore.BufBankReadP(bufferNum).applyGain(1.0f / (MaxMagnitude + 0.01));
    std::cout << "\nBufStore BUFFER MAG:"<< MaxMagnitude;
    
    //copy to refernce buffers
    auto currentBuffer = bufferStore.BufBankReadP(bufferNum);
    
    referenceBuffers.makecopy(bufferNum, currentBuffer);
    
    float MaxMagnitudeRef = referenceBuffers.BufBankReadP(bufferNum).getMagnitude(0, referenceBuffers.getSamples(bufferNum));    
    std::cout << "\RefBuffer BUFFER MAG:"<< MaxMagnitudeRef;
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


void PipeDreamAudioProcessor::updateCurrentIRs() {
    
    for(int i = 0; i < 5; i ++) {
        int z = i;
        bufferTransfers[i].get ([this, z] (BufferWithSampleRate& buf)
                                {
            
            ParallelConvs.processors[z].loadImpulseResponse (std::move (buf.buffer),
                                                             buf.sampleRate,
                                                             juce::dsp::Convolution::Stereo::yes,
                                                             juce::dsp::Convolution::Trim::yes,
                                                             juce::dsp::Convolution::Normalise::yes);
        });
    }
    
}

void PipeDreamAudioProcessor::setCurrentIRs() {
    
    std::array<int, 5> pitches;
    
    pitches = {
        pitchsel1->get() + 12,
        pitchsel2->get() + 12,
        pitchsel3->get() + 12,
        pitchsel4->get() + 12,
        pitchsel5->get() + 12
    };
    
    for (int i = 0; i < 5; i++){
        bufferTransfers[i].set(BufferWithSampleRate {std::move (bufferStore.BufBankReadP(pitches[i])),
            bufferStore.GetSampleRate(pitches[i])});
    }

}

void PipeDreamAudioProcessor::setCurrentIR(int voiceNumber) {
    
    std::array<int, 5> pitches;
    
    pitches = {
        pitchsel1->get() + 12,
        pitchsel2->get() + 12,
        pitchsel3->get() + 12,
        pitchsel4->get() + 12,
        pitchsel5->get() + 12
    };
    
    bufferTransfers[voiceNumber].set(BufferWithSampleRate {std::move (bufferStore.BufBankReadP(pitches[voiceNumber])),
        bufferStore.GetSampleRate(pitches[voiceNumber])});
}

void PipeDreamAudioProcessor::splitAudio(const juce::AudioBuffer<float> &inputBuffer) {
    for(auto& fb: audioSplitBuffers)
    {
        fb = inputBuffer;
    }
    auto sb0Block = juce::dsp::AudioBlock<float>(audioSplitBuffers[0]);
    auto sb1Block = juce::dsp::AudioBlock<float>(audioSplitBuffers[1]);
    auto sb2Block = juce::dsp::AudioBlock<float>(audioSplitBuffers[2]);
    auto sb3Block = juce::dsp::AudioBlock<float>(audioSplitBuffers[3]);
    auto sb4Block = juce::dsp::AudioBlock<float>(audioSplitBuffers[4]);

    auto sb0Ctx = juce::dsp::ProcessContextReplacing<float>(sb0Block);
    auto sb1Ctx = juce::dsp::ProcessContextReplacing<float>(sb1Block);
    auto sb2Ctx = juce::dsp::ProcessContextReplacing<float>(sb2Block);
    auto sb3Ctx = juce::dsp::ProcessContextReplacing<float>(sb3Block);
    auto sb4Ctx = juce::dsp::ProcessContextReplacing<float>(sb4Block);
    
    //conv1.process(sb0Ctx);
    //put filtering here
//
//    LP1.process(fb0Ctx);
//    AP2.process(fb0Ctx);
//
//    HP1.process(fb1Ctx);
//    filterBuffers[2] = filterBuffers[1];
//    LP2.process(fb1Ctx);
//
//    HP2.process(fb2Ctx);
    
}

void PipeDreamAudioProcessor::chordProcess() {
//     int currentChord = ChordSel->getCurrentChoiceName().getIntValue();
//    
//
//    
//         apvts.getParameter("Pitch_Sel_1")->beginChangeGesture();
//         apvts.getParameter("Pitch_Sel_1")->setValueNotifyingHost(5.0);
//         apvts.getParameter("Pitch_Sel_1")->endChangeGesture();


}

void PipeDreamAudioProcessor::setDecay(int bufferNum) {
    
      auto decayTimeValue = apvts.getRawParameterValue("Decay_Time");
      int decaySample = static_cast<int>(std::round(decayTimeValue->load() * referenceBuffers.GetSampleRate(bufferNum)));

      double stretchRatio = referenceBuffers.getSamples(bufferNum) / static_cast<double>(decaySample);
    
    
      juce::AudioBuffer<float> temp;

      int numChannels = referenceBuffers.getChannels(bufferNum);
      soundtouch.setTempo(stretchRatio);

      temp.setSize(numChannels, decaySample, false, true, false);

      //bufferStore.SetBufferSizeStretch(bufferNum, numChannels, decaySample);
    
      for (int channel = 0; channel < numChannels; ++channel) {
        soundtouch.putSamples(referenceBuffers.BufBankBufferReadP1(bufferNum, channel),
                              referenceBuffers.getSamples(bufferNum));
          
        soundtouch.receiveSamples(temp.getWritePointer(channel),
                                  decaySample);
        soundtouch.clear();
      }
    
    //bufferStore.BufBankBufferWriteP1(bufferNum, channel) //pointer switch
    bufferStore.makecopy(bufferNum, temp);
    //setCurrentIR(bufferNum );
}

void PipeDreamAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
//    auto chords = ChordSel->getCurrentChoiceName().getFloatValue(); //
//    //ratio->getCurrentChoiceName().getFloatValue());
//    std::cout << chords;
    
    juce::ScopedNoDenormals noDenormals;
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto dry_wet_ratio = DryWet->get();
    
    //setCurrentIRs();

    juce::dsp::AudioBlock<float> block {buffer};
    auto conteky = juce::dsp::ProcessContextReplacing<float>(block);

    
    dry_wet_mixer.pushDrySamples(block);
    dry_wet_mixer.setWetMixProportion(dry_wet_ratio);
    
    
    updateCurrentIRs();
    splitAudio(buffer);
    
    outGainParams[0] = outGain1->get();
    outGainParams[1] = outGain2->get();
    outGainParams[2] = outGain3->get();
    outGainParams[3] = outGain4->get();
    outGainParams[4] = outGain5->get();

    ParallelConvs.process(conteky, outGainParams);
    
    //put filters here
    
    dry_wet_mixer.mixWetSamples(block);
    

    
    

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

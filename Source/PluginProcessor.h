/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "paths.h"
#include "BufferTran.h"
#include "Buffers.h"
#include "Params.h"
#include "ParallelProcessors.h"
#include "../SoundTouch/ea_soundtouch/ea_soundtouch.h"
#include "ParamAttachHelper.h"
//#include "ea_soundtouch.h"

//==============================================================================
/**
*/
class PipeDreamAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener, public juce::Timer
{
public:
    //==============================================================================
    PipeDreamAudioProcessor();
    ~PipeDreamAudioProcessor() override;

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
    //===================================
    void readIRFromFile(int IRNum, int bufferBankID);
    void readIRFromBinary(int IRNum, int bufferBankID);
    void repitchBuffer(juce::AudioFormatReader *reader, int bufferNum, int semitone, int bufferBankID);
    void normaliseAndTrim(int bufferBankID, int bufferNum);
    void updateCurrentIRs();
    void setCurrentIRs();
    void setDecay(int bufferNum);
    void setCurrentIR(int voiceNumber);
    void updateFilters();
    void updateDecayTime(int voiceNumber);
    void computeChords();
    void loadUserIR();
    void timerCallback() override;
    void computeHeldChords();
    //virtual void timerCallback ()=0;

    
    juce::AudioProcessorValueTreeState apvts;
    juce::ValueTree& getVariableTree(){ return variableTree; };
    
    juce::File root, savedFile, currentIR;
    
    juce::dsp::Convolution conv1;
    juce::dsp::Convolution conv2;
    juce::dsp::Convolution conv3;
    juce::dsp::Convolution conv4;
    juce::dsp::Convolution conv5;
    
    juce::AudioSampleBuffer fileBuffer;
    
    std::array<BufferStore, 4> BufferStoreArray;
    std::array<BufferStore, 4> referenceBufferStoreArray;

    //split buffers (prep for filtering)
    std::array<juce::AudioBuffer<float>, 5> audioSplitBuffers;
    
    std::array<juce::dsp::Convolution, 5> convObjects;
    
    
    //Parallel instances
    ParallelProcessors<juce::dsp::Convolution, 5> ParallelConvs;

    //buffer transfes
    std::array<BufferTransfer, 5> bufferTransfers;
    
    //gain
    std::array<float, 5> outGainParams;
    
    juce::dsp::DryWetMixer<float> dry_wet_mixer;
    
    juce::Value UserIRFilePath = apvts.state.getPropertyAsValue("UserIRFilePath", nullptr, true);
    
    // using namespace
    std::array<int, 5> HeldChordValues;


private:
    
    juce::AudioParameterInt* pitchsel1 {nullptr};
    juce::AudioParameterInt* pitchsel2 {nullptr};
    juce::AudioParameterInt* pitchsel3 {nullptr};
    juce::AudioParameterInt* pitchsel4 {nullptr};
    juce::AudioParameterInt* pitchsel5 {nullptr};
    
    juce::AudioParameterFloat* outGain1 {nullptr};
    juce::AudioParameterFloat* outGain2 {nullptr};
    juce::AudioParameterFloat* outGain3 {nullptr};
    juce::AudioParameterFloat* outGain4 {nullptr};
    juce::AudioParameterFloat* outGain5 {nullptr};
    
    juce::AudioParameterChoice* ChordSel {nullptr};
    juce::AudioParameterChoice* rootSel {nullptr};
    juce::AudioParameterBool* ChordHold {nullptr};
    
    juce::AudioParameterFloat* DryWet {nullptr};
    juce::AudioParameterFloat* DecayTime {nullptr};
    juce::AudioParameterFloat* LowPassCutOff {nullptr};
    juce::AudioParameterFloat* HighPassCutOff {nullptr};
    juce::AudioParameterChoice* IRSelect {nullptr};
    
    juce::AudioParameterFloat* GainInM {nullptr};
    juce::AudioParameterFloat* GainOutM {nullptr};
    juce::dsp::Gain<float> inputGainM, outputGainM;
    
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter <float>, juce::dsp::IIR::Coefficients <float>> lowPassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter <float>, juce::dsp::IIR::Coefficients <float>> highPassFilter;
    
    
    std::array<juce::dsp::Gain<float>, 5> outGains;
    
    soundtouch::SoundTouch soundtouch;
    
    std::array<juce::AudioBuffer<float>, 5> bufferCache;
    
    //Parameter listeners & helpers
    foleys::ParameterAttachment<int> pitchSel1PA;
    foleys::ParameterAttachment<int> pitchSel2PA;
    foleys::ParameterAttachment<int> pitchSel3PA;
    foleys::ParameterAttachment<int> pitchSel4PA;
    foleys::ParameterAttachment<int> pitchSel5PA;
    foleys::ParameterAttachment<int> chordSelPA;
    foleys::ParameterAttachment<int> RootSelPA;
    foleys::ParameterAttachment<bool> ChordHoldPA;
    
    int chordArray[9][5] = {
        {0, 0, 0, 0, 0}, // Mono
        {0, 0, 0, 7, 7}, // 5th
        {0, 0, 2, 2, 7}, // Sus2
        {0, 0, 3, 3, 7}, // Minor
        {0, 0, 4, 4, 7}, // Major
        {0, 0, 5, 5, 7}, // sus4
        {0, 4, 4, 7, 11}, // Maj7
        {0, 3, 3, 7, 11}, // min7
        {0, 0, 7, 9, 9} // 7sus
    };
    
    bool lockFlag = false;

    juce::dsp::ProcessSpec spec;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::ValueTree variableTree {"variableTree"};
    void parameterChanged (const juce::String& arameterID, float newValue) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PipeDreamAudioProcessor)
};

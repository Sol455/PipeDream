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
//#include "ea_soundtouch.h"

//==============================================================================
/**
*/
class PipeDreamAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
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
    void readIRFromFile(int IRNum, int IRtoWrite);
    //void rePitchBuffer(int test);
    //void repitchBuffer(juce::AudioFormatReader* reader, int bufferNum);
    void repitchBuffer(juce::AudioFormatReader *reader, int bufferNum, int semitone);
    void normaliseAndTrim(int bufferNum);
    void splitAudio(const juce::AudioBuffer<float> &inputBuffer);
    void updateCurrentIRs();
    void setCurrentIRs();
    void chordProcess();
    void setDecay(int bufferNum);

    
    juce::AudioProcessorValueTreeState apvts;
    juce::ValueTree& getVariableTree(){ return variableTree; };
    
    juce::File root, savedFile, currentIR;
    
    juce::dsp::Convolution conv1;
    juce::dsp::Convolution conv2;
    juce::dsp::Convolution conv3;
    juce::dsp::Convolution conv4;
    juce::dsp::Convolution conv5;
    
    juce::AudioSampleBuffer fileBuffer;
    
    
    static juce::String getPathtoIRFolder()
    {
        if((juce::SystemStats::getOperatingSystemType() & juce::SystemStats::MacOSX) != 0)
        {
            return juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonApplicationDataDirectory).getFullPathName() + "/Application Support/PipeDream/IRs/";
        }
        return "";
    }
    
    juce::String FilePath = getPathtoIRFolder();

    BufferStore bufferStore;
    BufferStore referenceBuffers;
    
    juce::String IRNames[3]= {"DRAIN.wav", "GUITAR.wav", "PVC_A2.wav"};
    
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


    
private:
    //==============================================================================
    
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

    
    std::array<juce::dsp::Gain<float>, 5> outGains;
    
    soundtouch::SoundTouch soundtouch;
    
    //juce::dsp::Gain<float> testGain;
    
    
    //std::array<int>, 5, 9> chordArray;
    

    //"Mono","5th","Sus2","Minor","Maj","Sus4","Maj7","min7","7sus"
    
    

    //juce::AudioParameterFloat* midHighCrossover {nullptr};
    
    juce::dsp::ProcessSpec spec;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::ValueTree variableTree {"variableTree"};
    void parameterChanged (const juce::String& arameterID, float newValue) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PipeDreamAudioProcessor)
};

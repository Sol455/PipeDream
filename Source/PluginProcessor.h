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
    void splitAudio(const juce::AudioBuffer<float> &inputBuffer);
    void updateCurrentIRs();
    void setCurrentIRs();

    
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
    
    BufferTransfer bufferTransfer1;
    BufferTransfer bufferTransfer2;
    BufferTransfer bufferTransfer3;
    BufferTransfer bufferTransfer4;
    BufferTransfer bufferTransfer5;

    BufferStore bufferStore;
    
    juce::String IRNames[3]= {"DRAIN.wav", "GUITAR.wav", "PVC_A2.wav"};
    
    std::array<juce::AudioBuffer<float>, 5> audioSplitBuffers;
    std::array<juce::dsp::Convolution, 5> convObjects;
    
    ParallelProcessors<juce::dsp::Convolution, 5> ParallelConvs;

    
    std::array<BufferTransfer, 5> bufferTransfers;
    

    
private:
    //==============================================================================
    
    juce::AudioParameterInt* pitchsel1 {nullptr};
    juce::AudioParameterInt* pitchsel2 {nullptr};
    juce::AudioParameterInt* pitchsel3 {nullptr};
    juce::AudioParameterInt* pitchsel4 {nullptr};
    juce::AudioParameterInt* pitchsel5 {nullptr};
    //juce::AudioParameterFloat* midHighCrossover {nullptr};
    
    juce::dsp::ProcessSpec spec;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::ValueTree variableTree {"variableTree"};
    void parameterChanged (const juce::String& arameterID, float newValue) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PipeDreamAudioProcessor)
};

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
    void repitchBuffer(juce::AudioFormatReader *reader, int bufferNum, const double& dOutSampleRate);
    
    juce::AudioProcessorValueTreeState apvts;

    
    juce::ValueTree& getVariableTree(){ return variableTree; };
    
    
    juce::File root, savedFile, currentIR;
    juce::dsp::Convolution irLoader;
    
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
    
    BufferTransfer bufferTransfer;
    BufferStore bufferStore;
    
    juce::String IRNames[3]= {"DRAIN.wav", "GUITAR.wav", "PVC_A2.wav"};

    
private:
    //==============================================================================
    
    juce::AudioParameterInt* pitchsel1 {nullptr};
    juce::AudioParameterFloat* pitchsel2 {nullptr};
    juce::AudioParameterFloat* pitchsel3 {nullptr};
    juce::AudioParameterFloat* pitchsel4 {nullptr};
    juce::AudioParameterFloat* pitchsel5 {nullptr};
    //juce::AudioParameterFloat* midHighCrossover {nullptr};
    
    juce::dsp::ProcessSpec spec;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::ValueTree variableTree {"variableTree"};
    void parameterChanged (const juce::String& arameterID, float newValue) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PipeDreamAudioProcessor)
};

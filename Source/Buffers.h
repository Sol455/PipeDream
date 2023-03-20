/*
  ==============================================================================

    Buffers.h
    Created: 17 Mar 2023 4:25:11pm
    Author:  Sol Harter

  ==============================================================================
*/
#ifndef Buffers_h
#define Buffers_h

#include <JuceHeader.h>

#pragma once



class BufferStore
{
public:
    //const float *const* BufBankWriteP (int bufID)
    auto BufBankWriteP (int bufID)
    {
        return bufferBank1[bufID].buffer.getArrayOfWritePointers();
    }
    
    auto BufBankarrayReadP (int bufID)
    {
        return bufferBank1[bufID].buffer.getArrayOfReadPointers();
    }
    
    void SetBufferSize (int buffernum, int numberofChannels, int numberofSamples)
    {
        //for(int i = 0; i < 12; i++ ) {
            bufferBank1[buffernum].buffer.setSize(numberofChannels, numberofSamples, true, false, false);
            //return bufferBank1[bufID];
       // }
    }
    
    void SetInfoAll (int numberofChannels, int numberofSamples)
    {
        for(int i = 0; i < 12; i++ ) {
            bufferBank1[i].buffer.setSize(numberofChannels, numberofSamples, true, false, false);
            //return bufferBank1[bufID];
        }
    }
    
    void SetSampleRate (int bufID, double sampleRate)
    {
        bufferBank1[bufID].sampleRate = sampleRate;
    }
    
    double GetSampleRate (int bufID)
    {
        return bufferBank1[bufID].sampleRate;
    }
    
    auto BufBankReadP (int bufID)
    {
        return bufferBank1[bufID].buffer;
    }
    
    int getSamples (int bufID)
    {
        return bufferBank1[bufID].buffer.getNumSamples();
    }
    
    int getChannels (int bufID)
    {
        return bufferBank1[bufID].buffer.getNumChannels();
    }


private:
    
//    juce::AudioBuffer<float> bufa1;
//    juce::AudioBuffer<float> bufa2;
//    juce::AudioBuffer<float> bufa3;
//    juce::AudioBuffer<float> bufa4;
//    juce::AudioBuffer<float> bufa5;
//    juce::AudioBuffer<float> bufa6;
//    juce::AudioBuffer<float> bufa7;
//    juce::AudioBuffer<float> bufa8;
//    juce::AudioBuffer<float> bufa9;
//    juce::AudioBuffer<float> bufa10;
//    juce::AudioBuffer<float> bufa11;
//    juce::AudioBuffer<float> bufa12;
    
    BufferWithSampleRate bufferBank1[12] = {
    };

};

#endif

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
    auto BufBankWriteP (int bufID)
    {
        return bufferBank1[bufID].getArrayOfWritePointers();
    }
    
    void SetInfo (int numberofChannels, int numberofSamples)
    {
        for(int i = 0; i < 12; i++ ) {
            bufferBank1[i].setSize(numberofChannels, numberofSamples, true, false, false);
            //return bufferBank1[bufID];
        }
    }

    
    auto BufBankReadP (int bufID)
    {
        return bufferBank1[bufID];
    }
    
    int getSamples (int bufID)
    {
        return bufferBank1[bufID].getNumSamples();
    }
    
    int getChannels (int bufID)
    {
        return bufferBank1[bufID].getNumChannels();
    }

    
    //juce::AudioBuffer::getArrayOfReadPointers
private:
    
    juce::AudioBuffer<float> bufa1;
    juce::AudioBuffer<float> bufa2;
    juce::AudioBuffer<float> bufa3;
    juce::AudioBuffer<float> bufa4;
    juce::AudioBuffer<float> bufa5;
    juce::AudioBuffer<float> bufa6;
    juce::AudioBuffer<float> bufa7;
    juce::AudioBuffer<float> bufa8;
    juce::AudioBuffer<float> bufa9;
    juce::AudioBuffer<float> bufa10;
    juce::AudioBuffer<float> bufa11;
    juce::AudioBuffer<float> bufa12;
    
    juce::AudioBuffer<float> bufferBank1[12] = {
        bufa1,
        bufa2,
        bufa3,
        bufa4,
        bufa5,
        bufa6,
        bufa7,
        bufa8,
        bufa9,
        bufa10,
        bufa11,
        bufa12,
    };

    

};

#endif

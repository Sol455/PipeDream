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
        return bufferBank1[bufID].buffer.getArrayOfWritePointers();
    }
    
    auto BufBankarrayReadP (int bufID)
    {
        return bufferBank1[bufID].buffer.getArrayOfReadPointers();
    }
    
    auto BufBankBufferWriteP1 (int bufID, int pointer)
    {
        return bufferBank1[bufID].buffer.getWritePointer(pointer);
    }
    
    void SetBufferSize (int buffernum, int numberofChannels, int numberofSamples)
    {
            //bufferBank1[buffernum].buffer.setSize(numberofChannels, numberofSamples, true, false, false);
        bufferBank1[buffernum].buffer.setSize(numberofChannels, numberofSamples);
    }
    
    void SetInfoAll (int numberofChannels, int numberofSamples)
    {
        for(int i = 0; i < 12; i++ ) {
            bufferBank1[i].buffer.setSize(numberofChannels, numberofSamples, true, false, false);
            //return bufferBank1[bufID];
        }
    }
    
    void SetInfo1 (int buffernum, int numberofChannels, int numberofSamples)
    {
            bufferBank1[buffernum].buffer.setSize(numberofChannels, numberofSamples, true, false, false);
            //return bufferBank1[bufID];
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
    
    BufferWithSampleRate bufferBank1[37] = {
    };

};

#endif

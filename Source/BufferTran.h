/*
  ==============================================================================

    BufferTran.h
    Created: 16 Mar 2023 3:28:49pm
    Author:  Sol Harter

  ==============================================================================
*/
#ifndef BufferTran_h
#define BufferTran_h

#pragma once

#include <JuceHeader.h>

//inline std::unique_ptr<juce::InputStream> createAssetInputStream (const char* resourcePath);



struct BufferWithSampleRate
{
    BufferWithSampleRate() = default;

    BufferWithSampleRate (juce::AudioBuffer<float>&& bufferIn, double sampleRateIn)
        : buffer (std::move (bufferIn)), sampleRate (sampleRateIn) {}

    juce::AudioBuffer<float> buffer;
    double sampleRate = 0.0;
};

class BufferTransfer
{
public:
    void set (BufferWithSampleRate&& p)
    {
        const juce::SpinLock::ScopedLockType lock (mutex);
        buffer = std::move (p);
        newBuffer = true;
    }

    // Call `fn` passing the new buffer, if there's one available
    template <typename Fn>
    void get (Fn&& fn)
    {
        const juce::SpinLock::ScopedTryLockType lock (mutex);

        if (lock.isLocked() && newBuffer)
        {
            fn (buffer);
            newBuffer = false;
        }
    }

private:
    BufferWithSampleRate buffer;
    bool newBuffer = false;
    juce::SpinLock mutex;
};




#define PIP_DEMO_UTILITIES_INCLUDED
#endif

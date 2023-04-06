/*
  ==============================================================================

    ParallelProcessors.h
    Created: 4 Apr 2023 7:43:22pm
    Author:  Sol Harter

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

template <class ProcessorType, size_t numProcessors>
class ParallelProcessors
{
public:
    
    std::array<juce::dsp::Gain<float>, 5> outGains;
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        for (size_t i = 0; i < numTempBuffers; ++i)
            tempBuffers[i] = juce::dsp::AudioBlock<float> (tempBuffersMemory[i], spec.numChannels, spec.maximumBlockSize);
        
        for (auto& p : processors)
            p.prepare (spec);
        
        for (int i = 0; i < 5; i ++) {
            outGains[i].prepare(spec);
        }
    }

    template <class ProcessContext>
    void process (const ProcessContext& context, std::array<float, 5> outGainParams)
    {
        //gainTest = outGainParams[0]; //-24;//outGain1->get();
        
        for (int i = 0; i < 5; i ++) {
            outGains[i].setGainDecibels(outGainParams[i]);
        }
        
        //outGains[0].setGainDecibels(outGainParams[2]);
        
        const auto numSamples = context.getInputBlock().getNumSamples();
        
        // The current context might hold smaller blocks than the one that we allocated.
        // This is why we make temporary sub-blocks from the pre-allocated temp buffers
        std::array<juce::dsp::AudioBlock<float>, numTempBuffers> processingTempBlocks;
        std::array<juce::dsp::AudioBlock<float>, numTempBuffers> processingTempBlocksOut;
        
        // Let the first processors process with a non-replacing context that writes to the
        // temp buffers
        for (size_t i = 0; i < numTempBuffers; ++i)
        {
            processingTempBlocks[i] = tempBuffers[i].getSubBlock (0, numSamples);
            processors[i].process (juce::dsp::ProcessContextNonReplacing (context.getInputBlock(), processingTempBlocks[i]));
            
            //applyGain(processingTempBlocks[i], testOutGain);
            
            //auto block = juce::dsp::AudioBlock<float>(buffer);
            auto ctx = juce::dsp::ProcessContextReplacing<float>(processingTempBlocks[i]);
            outGains[i].process(ctx);
            
            //auto gain_out = outGains[0];
            //std::cout << gain_out;
            //juce::dsp::ProcessContextNonReplacing<float>(processingTempBlocks[i], processingTempBlocksOut[i]);
            //outGains[i].process(juce::dsp::ProcessContextNonReplacing (processingTempBlocks[i], processingTempBlocksOut[i]));
            
        }

        // Let the last processor process the context passed in so that it writes to the
        // desired output block
        processors.back().process (context);
        outGains[4].process(context);

        // Accumulate the temporary block data into the output buffer
        for (auto& tempBlock : processingTempBlocks)
            context.getOutputBlock() += tempBlock;
    }

    /** The actual processors */
    std::array<ProcessorType, numProcessors> processors;

private:
    
    template<typename T, typename U>
    void applyGain(T& buffer, U& gain)
        {
            auto block = juce::dsp::AudioBlock<float>(buffer);
            auto ctx = juce::dsp::ProcessContextReplacing<float>(block);
            gain.process(ctx);
        }
    

    static constexpr auto numTempBuffers = numProcessors - 1;
    
    std::array<juce::HeapBlock<char>,        numTempBuffers> tempBuffersMemory;
    std::array<juce::dsp::AudioBlock<float>, numTempBuffers> tempBuffers;
};

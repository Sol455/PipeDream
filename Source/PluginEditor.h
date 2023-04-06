/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Params.h"

//==============================================================================
/**
*/
class PipeDreamAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PipeDreamAudioProcessorEditor (PipeDreamAudioProcessor&);
    ~PipeDreamAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PipeDreamAudioProcessor& audioProcessor;
    juce::ValueTree& variableTree;
    
    juce::Slider PitchSel1Slider;
    juce::Slider PitchSel2Slider;
    juce::Slider PitchSel3Slider;
    juce::Slider PitchSel4Slider;
    juce::Slider PitchSel5Slider;
    
    juce::Slider GainOut1Slider;
    juce::Slider GainOut2Slider;
    juce::Slider GainOut3Slider;
    juce::Slider GainOut4Slider;
    juce::Slider GainOut5Slider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> PitchSel1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> PitchSel2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> PitchSel3Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> PitchSel4Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> PitchSel5Attachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> GainOut1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> GainOut2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> GainOut3Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> GainOut4Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> GainOut5Attachment;
    
    juce::TextButton loadBtn;
    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::Label irName;
    
    juce::Component pitchSelControls;
    juce::Component gainControls;
    juce::Component filterControls;
    juce::Component chordControls;
    
    
//    juce::Identifier FilePath1;
//    juce::Identifier Root;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PipeDreamAudioProcessorEditor)
};

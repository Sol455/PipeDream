/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Params.h"
#include "LooknFeel.h"

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
    void computeHeldChords();
    void computeChords();
    void updateDecayTime(int voiceNumber);
    void makeSlider(juce::Slider &slider, juce::String textValueSuffix);
    void makeLabel(juce::Label &label, juce::String text, juce::Component *slider);
    void IRChanged();


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    juce::CustomStyle customStyle;
    
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
    
    std::array<int, 5> HeldChordValues;
    //std::array<int, 5> currentChordValues;
    
    //std::Atomic lockValue = 4;
    
    bool lockFlag = false;
    
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
    
    juce::Slider ChordSelSlider;
    juce::Label ChordSelLabel;
    
    juce::Slider RootSelSlider;
    juce::Label RootSelLabel;
    
    juce::Slider DryWetSlider;
    juce::Label DryWetLabel;
    juce::Slider DecaySlider;
    juce::Label DecayLabel;
    
    juce::Slider LowPassSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> LowPassAttachment;
    juce::Label LowPassLabel;
    
    juce::Slider HighPassSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> HighPassAttachment;
    juce::Label HighPassLabel;
        
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> DryWetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> DecayAttachment;

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
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ChordSelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> RootSelAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> IRSelectAttachment;
    
    juce::TextButton loadBtn;
    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::Label irName;
    
    juce::Component pitchSelControls;
    juce::Component gainControls;
    juce::Component filterControls;
    juce::Component chordControls;
    juce::Component topPanel;
    juce::Component sidePanel;
    
    juce::Label TubeLengthLabel;
    juce::Label GainLabel;
    juce::Label TitleLabel;

    juce::TextButton chordHoldButton {"Hold"};
    juce::TextButton LoadButton {"Load"};
    
    juce::ComboBox IRSelect;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ChordHoldAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PipeDreamAudioProcessorEditor)
};

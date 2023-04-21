/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "stdio.h"

//==============================================================================
PipeDreamAudioProcessorEditor::PipeDreamAudioProcessorEditor (PipeDreamAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), variableTree(p.getVariableTree())
{
    
    juce::LookAndFeel::setDefaultLookAndFeel(&customStyle);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //addAndMakeVisible(loadBtn);
    //loadBtn.setButtonText("Load IR");
    DBG("variable tree in editor = " << static_cast<int>(variableTree.isValid()));
    LoadButton.onClick = [this]()
    {
        fileChooser = std::make_unique<juce::FileChooser>("Choose File",
                                                          audioProcessor.root,
                                                          "*");
        const auto fileChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::canSelectDirectories;
        
        fileChooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            juce::File result (chooser.getResult());
            
            if (result.getFileExtension() == ".wav" | result.getFileExtension() == ".mp3")
            {
                audioProcessor.savedFile = result;
                audioProcessor.root = result.getParentDirectory().getFullPathName();
                //variableTree.setProperty("FilePath1", audioProcessor.savedFile.getFullPathName(), nullptr);
                
                //variableTree.setProperty("FilePath2", "WAG1", nullptr);
                //audioProcessor.variableTree.setProperty("Root", audioProcessor.savedFile.getParentDirectory().getFullPathName(), nullptr);
                audioProcessor.conv1.reset();
//                audioProcessor.irLoader.loadImpulseResponse(audioProcessor.savedFile,
//                                                            juce::dsp::Convolution::Stereo::yes,
//                                                            juce::dsp::Convolution::Trim::yes,
//                                                            0);
//                
                irName.setText(result.getFileName(), juce::dontSendNotification);
            }
        });
    };
    
    ChordSelSlider.onDragEnd = [this]()
    {
        //lockFlag = true;
        if (chordHoldButton.getToggleState() == 0) {
            computeChords();
        }
        //lockFlag = false;
        //audioProcessor.setCurrentIRs();
        //updateDecayTimes();
    };
    
    RootSelSlider.onDragEnd = [this]() {
            
        if (chordHoldButton.getToggleState() == 1) {
            computeHeldChords();
        } else  if (chordHoldButton.getToggleState() == 0) {
            computeChords();

        }
        //audioProcessor.setCurrentIRs();
        //updateDecayTimes();

        //audioProcessor.setCurrentIRs();
    };
    
    addAndMakeVisible(irName);

    
    //pitch
    
    makeSlider(PitchSel1Slider, "");
    PitchSel1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_1", PitchSel1Slider);
    
    makeSlider(PitchSel2Slider, "");
    PitchSel2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_2", PitchSel2Slider);
    
    makeSlider(PitchSel3Slider, "");
    PitchSel3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_3", PitchSel3Slider);
    
    makeSlider(PitchSel4Slider, "");
    PitchSel4Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_4", PitchSel4Slider);
    
    makeSlider(PitchSel5Slider, "");
    PitchSel5Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_5", PitchSel5Slider);
    
    
    
    //gain
    
    makeSlider(GainOut1Slider, " dB");
    GainOut1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_Out_1", GainOut1Slider);
    
    makeSlider(GainOut2Slider, " dB");
    GainOut2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_Out_2", GainOut2Slider);
    
    makeSlider(GainOut3Slider, " dB");
    GainOut3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_Out_3", GainOut3Slider);
    
    makeSlider(GainOut4Slider, " dB");
    GainOut4Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_Out_4", GainOut4Slider);
    
    makeSlider(GainOut5Slider, " dB");
    GainOut5Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_Out_5", GainOut5Slider);
    

    
    
    //chords
    makeSlider(ChordSelSlider, "");
    makeLabel(ChordSelLabel, "Chord", &ChordSelSlider);

    ChordSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Chord_Sel", ChordSelSlider);
    
    makeSlider(RootSelSlider, "");
    makeLabel(RootSelLabel, "Root", &RootSelSlider);
    RootSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Root_Sel", RootSelSlider);

    
    
    //lock button
    
    ChordHoldAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Chord_Hold", chordHoldButton);

    
    chordHoldButton.setClickingTogglesState(true);

    

    chordHoldButton.onClick  = [&]() {
        const auto buttonMessage = chordHoldButton.getToggleState() ? "Held" : "Hold";
        chordHoldButton.setButtonText(buttonMessage);
        
            auto currentPitch1 = audioProcessor.apvts.getRawParameterValue("Pitch_Sel_1");
            int pitch1 = static_cast<int>(currentPitch1->load());
            auto currentPitch2 = audioProcessor.apvts.getRawParameterValue("Pitch_Sel_2");
            int pitch2 = static_cast<int>(currentPitch2->load());
            auto currentPitch3 = audioProcessor.apvts.getRawParameterValue("Pitch_Sel_3");
            int pitch3 = static_cast<int>(currentPitch3->load());
            auto currentPitch4 = audioProcessor.apvts.getRawParameterValue("Pitch_Sel_4");
            int pitch4 = static_cast<int>(currentPitch4->load());
            auto currentPitch5 = audioProcessor.apvts.getRawParameterValue("Pitch_Sel_5");
            int pitch5 = static_cast<int>(currentPitch5->load());
        
            HeldChordValues[0] = pitch1;
            HeldChordValues[1] = pitch2;
            HeldChordValues[2] = pitch3;
            HeldChordValues[3] = pitch4;
            HeldChordValues[4] = pitch5;

    };
                                                                                    
    addAndMakeVisible(chordHoldButton);
    addAndMakeVisible(LoadButton);
    //add menu item
    
    //dry wet slider    //decay
    
    makeSlider(DryWetSlider, " %");
    makeLabel(DryWetLabel, "Dry/Wet", &DryWetSlider);
    DryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Dry_Wet", DryWetSlider);
    
    makeSlider(DecaySlider, " s");
    makeLabel(DecayLabel, "Decay", &DecaySlider);
    DecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Decay_Time", DecaySlider);
    
    //filter slider
    
    makeSlider(LowPassSlider, " Hz");
    makeLabel(LowPassLabel, "LP", &LowPassSlider);
    LowPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Low_Pass_Cut_Off", LowPassSlider);
    
    
    makeSlider(HighPassSlider, " Hz");
    makeLabel(HighPassLabel, "HP", &HighPassSlider);
    HighPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "High_Pass_Cut_Off", HighPassSlider);

    
    DecaySlider.onDragEnd = [this]() {
        //updateDecayTimes();
        for (int i = 0; i < 5; i ++) {
            updateDecayTime(i);
        }
    };
    
    //if (lockFlag == false) {
        PitchSel1Slider.onDragEnd = [this]() {
            audioProcessor.setCurrentIR(0);
            updateDecayTime(0);
        };
        PitchSel2Slider.onDragEnd = [this]() {
            audioProcessor.setCurrentIR(1);
            updateDecayTime(1);
        };
        PitchSel3Slider.onDragEnd = [this]() {
            audioProcessor.setCurrentIR(2);
            updateDecayTime(2);
        };
        PitchSel4Slider.onDragEnd = [this]() {
            audioProcessor.setCurrentIR(3);
            updateDecayTime(3);
        };
        PitchSel5Slider.onDragEnd = [this]() {
            audioProcessor.setCurrentIR(4);
            updateDecayTime(4);
        };
   // }
    
    //PitchSel5Slider.on
    
    
    setSize (750, 275);
    

}

void PipeDreamAudioProcessorEditor::makeSlider(juce::Slider &slider,
                                               juce::String textValueSuffix) {
  addAndMakeVisible(slider);
  slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  slider.setTextValueSuffix(textValueSuffix);
  slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow,
                         false, 60, 15);
}


void PipeDreamAudioProcessorEditor::makeLabel(juce::Label &label,
                                             juce::String text,
                                             juce::Component *slider) {
    
  addAndMakeVisible(label);
  label.setText(text, juce::dontSendNotification);
  label.setJustificationType(juce::Justification::centred);
  label.setBorderSize(juce::BorderSize<int>(0));
  label.setBorderSize(juce::BorderSize<int>(0));
  label.attachToComponent(slider, false);
}


void PipeDreamAudioProcessorEditor::updateDecayTime(int voiceNumber) {

    IDArray =   {"Pitch_Sel_1",
                "Pitch_Sel_2",
                "Pitch_Sel_3",
                "Pitch_Sel_4",
                "Pitch_Sel_5"
    };
    
    auto currentVoice = audioProcessor.apvts.getRawParameterValue(IDArray[voiceNumber]);
    int voice = static_cast<int>(currentVoice->load());
    
    audioProcessor.setDecay(voice + 12);
    audioProcessor.setCurrentIR(voiceNumber);
}



PipeDreamAudioProcessorEditor::~PipeDreamAudioProcessorEditor()
{
}

void PipeDreamAudioProcessorEditor::computeChords() {
    //int currentChord = ChordSel->getCurrentChoiceName().getIntValue();
    auto ChordSel = audioProcessor.apvts.getRawParameterValue("Chord_Sel");
    int currentChord = static_cast<int>(ChordSel->load());
    
    auto offsetSel = audioProcessor.apvts.getRawParameterValue("Root_Sel");
    int offset = static_cast<int>(offsetSel->load());
    
    audioProcessor.apvts.getParameter("Pitch_Sel_1")->beginChangeGesture();
    audioProcessor.apvts.getParameter("Pitch_Sel_1")->setValueNotifyingHost(audioProcessor.apvts.getParameter("Pitch_Sel_1")->convertTo0to1(chordArray[currentChord][0] + offset - 12));
    audioProcessor.apvts.getParameter("Pitch_Sel_1")->endChangeGesture();
    
    audioProcessor.apvts.getParameter("Pitch_Sel_2")->beginChangeGesture();
    audioProcessor.apvts.getParameter("Pitch_Sel_2")->setValueNotifyingHost(audioProcessor.apvts.getParameter("Pitch_Sel_2")->convertTo0to1(chordArray[currentChord][1]+ offset - 12)) ;
    audioProcessor.apvts.getParameter("Pitch_Sel_2")->endChangeGesture();
    
    audioProcessor.apvts.getParameter("Pitch_Sel_3")->beginChangeGesture();
    audioProcessor.apvts.getParameter("Pitch_Sel_3")->setValueNotifyingHost(audioProcessor.apvts.getParameter("Pitch_Sel_3")->convertTo0to1(chordArray[currentChord][2]+ offset - 12)) ;
    audioProcessor.apvts.getParameter("Pitch_Sel_3")->endChangeGesture();
    
    audioProcessor.apvts.getParameter("Pitch_Sel_4")->beginChangeGesture();
    audioProcessor.apvts.getParameter("Pitch_Sel_4")->setValueNotifyingHost(audioProcessor.apvts.getParameter("Pitch_Sel_4")->convertTo0to1(chordArray[currentChord][3]+ offset - 12) );
    audioProcessor.apvts.getParameter("Pitch_Sel_4")->endChangeGesture();
    
    audioProcessor.apvts.getParameter("Pitch_Sel_5")->beginChangeGesture();
    audioProcessor.apvts.getParameter("Pitch_Sel_5")->setValueNotifyingHost(audioProcessor.apvts.getParameter("Pitch_Sel_5")->convertTo0to1(chordArray[currentChord][4]+ offset - 12) );
    audioProcessor.apvts.getParameter("Pitch_Sel_5")->endChangeGesture();
    

    updateDecayTime(0);
    updateDecayTime(1);
    updateDecayTime(2);
    updateDecayTime(3);
    updateDecayTime(4);



    //lockFlag = false;

}

void PipeDreamAudioProcessorEditor::computeHeldChords() {
    
    auto offsetSel = audioProcessor.apvts.getRawParameterValue("Root_Sel");
    int offset = static_cast<int>(offsetSel->load());

    
    audioProcessor.apvts.getParameter("Pitch_Sel_1")->beginChangeGesture();
    audioProcessor.apvts.getParameter("Pitch_Sel_1")->setValueNotifyingHost(audioProcessor.apvts.getParameter("Pitch_Sel_1")->convertTo0to1(HeldChordValues[0] + offset - 12));
    audioProcessor.apvts.getParameter("Pitch_Sel_1")->endChangeGesture();

    audioProcessor.apvts.getParameter("Pitch_Sel_2")->beginChangeGesture();
    audioProcessor.apvts.getParameter("Pitch_Sel_2")->setValueNotifyingHost(audioProcessor.apvts.getParameter("Pitch_Sel_2")->convertTo0to1(HeldChordValues[1] + offset - 12)) ;
    audioProcessor.apvts.getParameter("Pitch_Sel_2")->endChangeGesture();

    audioProcessor.apvts.getParameter("Pitch_Sel_3")->beginChangeGesture();
    audioProcessor.apvts.getParameter("Pitch_Sel_3")->setValueNotifyingHost(audioProcessor.apvts.getParameter("Pitch_Sel_3")->convertTo0to1(HeldChordValues[2] + offset - 12)) ;
    audioProcessor.apvts.getParameter("Pitch_Sel_3")->endChangeGesture();

    audioProcessor.apvts.getParameter("Pitch_Sel_4")->beginChangeGesture();
    audioProcessor.apvts.getParameter("Pitch_Sel_4")->setValueNotifyingHost(audioProcessor.apvts.getParameter("Pitch_Sel_4")->convertTo0to1(HeldChordValues[3] + offset - 12) );
    audioProcessor.apvts.getParameter("Pitch_Sel_4")->endChangeGesture();

    audioProcessor.apvts.getParameter("Pitch_Sel_5")->beginChangeGesture();
    audioProcessor.apvts.getParameter("Pitch_Sel_5")->setValueNotifyingHost(audioProcessor.apvts.getParameter("Pitch_Sel_5")->convertTo0to1(HeldChordValues[4] + offset - 12));
    audioProcessor.apvts.getParameter("Pitch_Sel_5")->endChangeGesture();
    
    updateDecayTime(0);
    updateDecayTime(1);
    updateDecayTime(2);
    updateDecayTime(3);
    updateDecayTime(4);

}
//==============================================================================
void PipeDreamAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto sidePaddingWidth = 120;
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    juce::Rectangle<int> sidePanelR = sidePanel.getBounds();
    juce::Rectangle<int> topPanelR = topPanel.getBounds();
    juce::Rectangle<int> pitchSelControlR = pitchSelControls.getBounds();
    juce::Rectangle<int> gainControlsR = gainControls.getBounds();

    g.fillAll(juce::Colour::fromRGB(252, 248, 237));
    g.setColour(juce::Colours::grey);
    g.setColour(juce::Colours::darkgrey);
    
    g.drawRect(pitchSelControlR);
    g.drawRect(gainControlsR);
    
    //g.setColour (juce::Colours::red);
    g.drawRect(sidePanelR);
    g.drawRect(topPanelR);
    
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    
    
}

void PipeDreamAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto sidePaddingWidth = 120;
    
    auto bounds = getLocalBounds();
    
    const auto btnX = getWidth() * 0.35;
    const auto btnY = getHeight() * 0.15;
    const auto btnWidth = getWidth() * 0.15;
    const auto btnHeight = btnWidth * 0.5;
    
    int spacerTop = 18;

    
   
    loadBtn.setBounds(btnX, btnY, btnWidth, btnHeight);
    irName.setBounds(loadBtn.getX() + loadBtn.getWidth(), btnY, btnWidth * 2, btnHeight);
    
    topPanel.setBounds(bounds.removeFromTop(40));
    sidePanel.setBounds(bounds.removeFromRight(sidePaddingWidth * 2));
    pitchSelControls.setBounds(bounds.removeFromTop(bounds.getHeight()/2));
    gainControls.setBounds(bounds);
    
    
    //main labels
    
    
//
//    TitleLabel.setText ("Pipe Dream", juce::dontSendNotification);
//    TitleLabel.setFont (juce::Font (32.0f, juce::Font::bold));
//    TitleLabel.setJustificationType (juce::Justification::left);
//    addAndMakeVisible (TitleLabel);
//    TitleLabel.setBounds(topPanel.getBounds());



    TubeLengthLabel.setText ("Tube Length", juce::dontSendNotification);
    TubeLengthLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (TubeLengthLabel);
    auto pitchTitleBounds = pitchSelControls.getBounds().removeFromTop(spacerTop);
    TubeLengthLabel.setBounds(pitchTitleBounds.removeFromBottom(pitchTitleBounds.getHeight() - 5));


    GainLabel.setText ("Gain", juce::dontSendNotification);
    GainLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (GainLabel);
    auto GainTitleBounds = gainControls.getBounds().removeFromTop(spacerTop);
    GainLabel.setBounds(GainTitleBounds.removeFromBottom(GainTitleBounds.getHeight() - 5));
         
    //pitch flex box
     //auto pitchBounds = pitchSelControls.getBounds().reduced(10);
     auto pitchBounds = pitchSelControls.getBounds().removeFromBottom(pitchSelControls.getHeight() - spacerTop).reduced(5);

     juce::FlexBox flexBoxpitch;
     flexBoxpitch.flexDirection = juce::FlexBox::Direction::row;
     flexBoxpitch.flexWrap = juce::FlexBox::Wrap::noWrap;
     
     auto spacer = juce::FlexItem().withWidth(2);
     //auto spacertop = juce::FlexItem().withWidth(10);
     
     flexBoxpitch.items.add(spacer);
     flexBoxpitch.items.add(juce::FlexItem(PitchSel1Slider).withFlex(1.f));
     flexBoxpitch.items.add(spacer);
     flexBoxpitch.items.add(juce::FlexItem(PitchSel2Slider).withFlex(1.f));
     flexBoxpitch.items.add(spacer);
     flexBoxpitch.items.add(juce::FlexItem(PitchSel3Slider).withFlex(1.f));
     flexBoxpitch.items.add(spacer);
     flexBoxpitch.items.add(juce::FlexItem(PitchSel4Slider).withFlex(1.f));
     flexBoxpitch.items.add(spacer);
     flexBoxpitch.items.add(juce::FlexItem(PitchSel5Slider).withFlex(1.f));
     flexBoxpitch.items.add(spacer);
    
    
     flexBoxpitch.performLayout(pitchBounds);
    
    //gain flexbox
    
    auto gainBounds = gainControls.getBounds().removeFromBottom(gainControls.getHeight() - spacerTop).reduced(5);
    //auto gainBounds = gainControls.getBounds().reduced(10);

    //gainBounds = gainBounds.re

    juce::FlexBox flexBoxgain;
    flexBoxgain.flexDirection = juce::FlexBox::Direction::row;
    flexBoxgain.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    //auto spacer = juce::FlexItem().withWidth(2);
    
    flexBoxpitch.items.add(spacer);
    flexBoxgain.items.add(juce::FlexItem(GainOut1Slider).withFlex(1.f));
    flexBoxgain.items.add(spacer);
    flexBoxgain.items.add(juce::FlexItem(GainOut2Slider).withFlex(1.f));
    flexBoxgain.items.add(spacer);
    flexBoxgain.items.add(juce::FlexItem(GainOut3Slider).withFlex(1.f));
    flexBoxgain.items.add(spacer);
    flexBoxgain.items.add(juce::FlexItem(GainOut4Slider).withFlex(1.f));
    flexBoxgain.items.add(spacer);
    flexBoxgain.items.add(juce::FlexItem(GainOut5Slider).withFlex(1.f));
    flexBoxgain.items.add(spacer);
    
    
    
//    flexBoxgain.items.add(juce::FlexItem().withHeight(15));
//    flexBoxgain.items.add(juce::FlexItem().withHeight(30));

   
    flexBoxgain.performLayout(gainBounds);
   
    //chords flexbox
    
    //auto chordBoundZ = sidePanel.getBounds(); //.removeFromTop(sidePanel.getY()/2).reduced(10);
    auto chordBounds = sidePanel.getBounds().removeFromTop(sidePanel.getBounds().getHeight()/2).reduced(10);
    //auto buttonBounds = chordBounds.removeFromTop(30);
    
    auto buttonBounds = topPanel.getBounds().removeFromRight(sidePaddingWidth * 2);
    
    chordHoldButton.setBounds(buttonBounds.removeFromRight(buttonBounds.getWidth()/2).reduced(2));
    LoadButton.setBounds(buttonBounds.reduced(2));
    //buttonBounds.getX() + buttonBounds.getX() / 2, buttonBounds.getY(), 50, 50)
    juce::FlexBox flexBoxChords;
    flexBoxChords.flexDirection = juce::FlexBox::Direction::row;
    flexBoxChords.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    flexBoxgain.items.add(spacer);
    flexBoxChords.items.add(juce::FlexItem(RootSelSlider).withFlex(1.f));
    flexBoxgain.items.add(spacer);
    flexBoxChords.items.add(juce::FlexItem(ChordSelSlider).withFlex(1.f));
    flexBoxgain.items.add(spacer);
    flexBoxChords.items.add(juce::FlexItem(DecaySlider).withFlex(1.f));
    flexBoxgain.items.add(spacer);

    
    flexBoxChords.performLayout(chordBounds);
    
    //filter flexbox
    
    auto filterBounds = sidePanel.getBounds().removeFromBottom(sidePanel.getBounds().getHeight()/2).reduced(10);
    
    //auto button2Bounds = filterBounds.removeFromTop(30);
    
    //buttonBounds.getX() + buttonBounds.getX() / 2, buttonBounds.getY(), 50, 50);
    juce::FlexBox flexBoxFilter;
    flexBoxFilter.flexDirection = juce::FlexBox::Direction::row;
    flexBoxFilter.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    flexBoxgain.items.add(spacer);
    flexBoxFilter.items.add(juce::FlexItem(LowPassSlider).withFlex(1.f));
    flexBoxgain.items.add(spacer);
    flexBoxFilter.items.add(juce::FlexItem(HighPassSlider).withFlex(1.f));
    flexBoxgain.items.add(spacer);
    flexBoxFilter.items.add(juce::FlexItem(DryWetSlider).withFlex(1.f));
    flexBoxgain.items.add(spacer);

    flexBoxFilter.performLayout(filterBounds);
    
    
}

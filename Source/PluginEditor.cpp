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
//        fileChooser = std::make_unique<juce::FileChooser>("Choose File",
//                                                          audioProcessor.root,
//                                                          "*");
//        const auto fileChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::canSelectDirectories;
//
//        fileChooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
//        {
//            juce::File result (chooser.getResult());
//
//            if (result.getFileExtension() == ".wav" | result.getFileExtension() == ".mp3")
//            {
//                audioProcessor.savedFile = result;
//                audioProcessor.UserIRFilePath.setValue(audioProcessor.savedFile.getFullPathName());
//                audioProcessor.loadUserIR();
//
//            }
//        });
    };
    
    
    //pitch
    
    makeSlider(PitchSel1Slider, " st");
    PitchSel1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_1", PitchSel1Slider);
    
    makeSlider(PitchSel2Slider, " st");
    PitchSel2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_2", PitchSel2Slider);
    
    makeSlider(PitchSel3Slider, " st");
    PitchSel3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_3", PitchSel3Slider);
    
    makeSlider(PitchSel4Slider, " st");
    PitchSel4Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_4", PitchSel4Slider);
    
    makeSlider(PitchSel5Slider, " st");
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
    
//    ChordHoldAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Chord_Hold", chordHoldButton);
//
//    chordHoldButton.setClickingTogglesState(true);
//
//    chordHoldButton.onClick  = [&]() {
//        const auto buttonMessage = chordHoldButton.getToggleState() ? "Held" : "Hold";
//        chordHoldButton.setButtonText(buttonMessage);
//    };
//
//    addAndMakeVisible(chordHoldButton);
    addAndMakeVisible(LoadButton);
    //add menu item
    
    //dry wet slider    //decay
    
    makeSlider(DryWetSlider, " %");
    makeLabel(DryWetLabel, "Dry/Wet", &DryWetSlider);
    DryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Dry_Wet", DryWetSlider);
    
    makeSlider(DecaySlider, " s");
    makeLabel(DecayLabel, "Size", &DecaySlider);
    DecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Decay_Time", DecaySlider);
    
    //filter slider
    
    makeSlider(LowPassSlider, " Hz");
    makeLabel(LowPassLabel, "LP", &LowPassSlider);
    LowPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Low_Pass_Cut_Off", LowPassSlider);
    
    
    makeSlider(HighPassSlider, " Hz");
    makeLabel(HighPassLabel, "HP", &HighPassSlider);
    HighPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "High_Pass_Cut_Off", HighPassSlider);
    
    //Master Gain slider
    
    makeSlider(GainInMSlider, " dB");
    makeLabel(GainInMLabel, "Gain In", &GainInMSlider);
    GainInMAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_In_Master", GainInMSlider);
    
    makeSlider(GainOutMSlider, " dB");
    makeLabel(GainOutMLabel, "Gain Out", &GainOutMSlider);
    GainOutMAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_Out_Master", GainOutMSlider);
    
    
    DecaySlider.onDragEnd = [this]() {
        //updateDecayTimes();
        for (int i = 0; i < 5; i ++) {
            audioProcessor.updateDecayTime(i);
        }
    };
    
    
//    addAndMakeVisible (IRSelect);
//
//    //IRsel
//    IRSelect.addItem ("PVC",  1);
//    IRSelect.addItem ("Glass",   2);
//    IRSelect.addItem ("Metal", 3);
//    IRSelect.addItem ("User", 4);
//
//    IRSelectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "IR_select", IRSelect);
//
//
//    IRSelect.onChange = [this] { IRChanged(); };
    
    
    setSize (750, 320);
    

}

void PipeDreamAudioProcessorEditor::IRChanged() {
    audioProcessor.setCurrentIRs();
    for (int i = 0; i < 5; i ++) {
        audioProcessor.setDecay(i);
    }
    
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




PipeDreamAudioProcessorEditor::~PipeDreamAudioProcessorEditor()
{
}

void PipeDreamAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto sidePaddingWidth = 200;
    int spacerTop = 18;
    int linespacer = 20;
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    juce::Rectangle<int> rightPanelR = rightPanel.getBounds();
    juce::Rectangle<int> leftPanelR = rightPanel.getBounds();
    juce::Rectangle<int> topPanelR = topPanel.getBounds();
    juce::Rectangle<int> botPanelR = bottomPanel.getBounds();
    juce::Rectangle<int> pitchSelControlR = pitchSelControls.getBounds();
    juce::Rectangle<int> gainControlsR = gainControls.getBounds();

    g.fillAll(juce::Colour::fromString("0xffDAF0EE"));
    
    //g.setColour(juce::Colours::grey);
    g.setColour(juce::Colour::fromString("0xff3B413C"));
    
    juce::Line<float> linePitch (juce::Point<float> (pitchSelControls.getX() + linespacer, pitchSelControls.getY() + spacerTop + 5),
                            juce::Point<float> (pitchSelControls.getX() + pitchSelControls.getWidth() - linespacer, pitchSelControls.getY() + spacerTop + 5));
    
    juce::Line<float> lineGain (juce::Point<float> (gainControls.getX() + linespacer, gainControls.getY() + spacerTop + 5),
                            juce::Point<float> (gainControls.getX() + gainControls.getWidth() - linespacer, gainControls.getY() + spacerTop + 5));
 
    g.drawLine (linePitch, 1.0f);
    g.drawLine (lineGain, 1.0f);
//
//    g.drawRect(pitchSelControlR);
//    g.drawRect(gainControlsR);
//
//    g.drawRect(leftPanelR);
//    g.drawRect(rightPanelR);
//    g.drawRect(topPanelR);
//    g.drawRect(botPanelR);
    
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    
    
}

void PipeDreamAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
  
    
    auto bounds = getLocalBounds();
    
    const auto btnX = getWidth() * 0.35;
    const auto btnY = getHeight() * 0.15;
    const auto btnWidth = getWidth() * 0.15;
    const auto btnHeight = btnWidth * 0.5;
    
    int spacerTop = 18;
    int topBotPanelWidth = 40;
    int sidePaddingWidth = 150;
   
//    loadBtn.setBounds(btnX, btnY, btnWidth, btnHeight);
//    irName.setBounds(loadBtn.getX() + loadBtn.getWidth(), btnY, btnWidth * 2, btnHeight);
    
    topPanel.setBounds(bounds.removeFromTop(topBotPanelWidth));
    bottomPanel.setBounds(bounds.removeFromBottom(topBotPanelWidth));
    rightPanel.setBounds(bounds.removeFromRight(sidePaddingWidth));
    leftPanel.setBounds(bounds.removeFromLeft(sidePaddingWidth));

    pitchSelControls.setBounds(bounds.removeFromTop(bounds.getHeight()/2));
    gainControls.setBounds(bounds);
    
    
    //main labels
    

    TitleLabel.setText ("Pipe Dream", juce::dontSendNotification);
    TitleLabel.setFont (juce::Font (32.0f, juce::Font::bold));
    TitleLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (TitleLabel);
    TitleLabel.setBounds(topPanel.getBounds());

    TubeLengthLabel.setText ("Pipe Length/Pitch", juce::dontSendNotification);
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
     auto pitchBounds = pitchSelControls.getBounds().removeFromBottom(pitchSelControls.getHeight() - spacerTop).reduced(5);

     juce::FlexBox flexBoxpitch;
     flexBoxpitch.flexDirection = juce::FlexBox::Direction::row;
     flexBoxpitch.flexWrap = juce::FlexBox::Wrap::noWrap;
     
     auto spacer = juce::FlexItem().withWidth(2);

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
    
//gain flex box
    
    
    auto gainBounds = gainControls.getBounds().removeFromBottom(gainControls.getHeight() - spacerTop).reduced(5);


    juce::FlexBox flexBoxgain;
    flexBoxgain.flexDirection = juce::FlexBox::Direction::row;
    flexBoxgain.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    
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
    

   
    flexBoxgain.performLayout(gainBounds);
    
    auto buttonBounds = bottomPanel.getBounds();//.removeFromRight(sidePaddingWidth * 3);
    
    //chordHoldButton.setBounds(buttonBounds.removeFromRight(buttonBounds.getWidth()/3).reduced(2));
    LoadButton.setBounds(buttonBounds.removeFromRight(buttonBounds.getWidth()/2).reduced(2));
    //IRSelect.setBounds(buttonBounds.reduced(2));
   
    
    //Right panel
    int sliderlabelpadding = 30;
    
    
    auto rightBounds = rightPanel.getBounds().reduced(10);
    auto rightBoundsRow1a = rightBounds.removeFromTop(rightBounds.getHeight()/2);
    auto rightBoundsRow1 = rightBoundsRow1a.removeFromTop(rightBoundsRow1a.getHeight() - sliderlabelpadding);
    auto rightBoundsRow2 = rightBounds.removeFromBottom(rightBounds.getHeight() - (sliderlabelpadding - 10));
    
    
    juce::FlexBox RightRow1;
    RightRow1.flexDirection = juce::FlexBox::Direction::row;
    RightRow1.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    RightRow1.items.add(spacer);
    RightRow1.items.add(juce::FlexItem(HighPassSlider).withFlex(1.f));
    RightRow1.items.add(spacer);
    RightRow1.items.add(juce::FlexItem(LowPassSlider).withFlex(1.f));
    RightRow1.items.add(spacer);

    RightRow1.performLayout(rightBoundsRow1);
    
    juce::FlexBox RightRow2;
    RightRow2.flexDirection = juce::FlexBox::Direction::row;
    RightRow2.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    RightRow2.items.add(spacer);
    RightRow2.items.add(juce::FlexItem(DecaySlider).withFlex(1.f));
    RightRow2.items.add(spacer);
    RightRow2.items.add(juce::FlexItem(DryWetSlider).withFlex(1.f));
    RightRow2.items.add(spacer);

    RightRow2.performLayout(rightBoundsRow2);
    
    //Left panel
    
    
    auto leftBounds = leftPanel.getBounds().reduced(10);
    auto leftBoundsRow1a = leftBounds.removeFromTop(leftBounds.getHeight()/2);
    auto leftBoundsRow1 = leftBoundsRow1a.removeFromTop(leftBoundsRow1a.getHeight() - sliderlabelpadding);
    auto leftBoundsRow2 = leftBounds.removeFromBottom(leftBounds.getHeight() - (sliderlabelpadding - 10));
    
    juce::FlexBox LeftRow1;
    LeftRow1.flexDirection = juce::FlexBox::Direction::row;
    LeftRow1.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    LeftRow1.items.add(spacer);
    LeftRow1.items.add(juce::FlexItem(RootSelSlider).withFlex(1.f));
    LeftRow1.items.add(spacer);
    LeftRow1.items.add(juce::FlexItem(ChordSelSlider).withFlex(1.f));
    LeftRow1.items.add(spacer);

    LeftRow1.performLayout(leftBoundsRow1);
    
    juce::FlexBox LeftRow2;
    LeftRow2.flexDirection = juce::FlexBox::Direction::row;
    LeftRow2.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    LeftRow2.items.add(spacer);
    LeftRow2.items.add(juce::FlexItem(GainInMSlider).withFlex(1.f));
    LeftRow2.items.add(spacer);
    LeftRow2.items.add(juce::FlexItem(GainOutMSlider).withFlex(1.f));
    LeftRow2.items.add(spacer);

    LeftRow2.performLayout(leftBoundsRow2);
    
    
}

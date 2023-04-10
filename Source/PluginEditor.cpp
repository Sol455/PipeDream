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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //addAndMakeVisible(loadBtn);
    loadBtn.setButtonText("Load IR");
    DBG("variable tree in editor = " << static_cast<int>(variableTree.isValid()));
    loadBtn.onClick = [this]()
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
        if (chordHoldButton.getToggleState() == 0) {
            computeChords();
        }
    };
    
    RootSelSlider.onDragEnd = [this]() {
            
        if (chordHoldButton.getToggleState() == 1) {
            computeHeldChords();
            std::cout << "High";
        } else  if (chordHoldButton.getToggleState() == 0) {
            computeChords();
            std::cout << "Low";

        }
    };
    
    addAndMakeVisible(irName);
    
    auto text_box_height = 50;
    auto text_box_width = 25;
    
    //pitch
    
    PitchSel1Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PitchSel2Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PitchSel3Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PitchSel4Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PitchSel5Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    
    PitchSel1Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    PitchSel2Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    PitchSel3Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    PitchSel4Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    PitchSel5Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    
    PitchSel1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_1", PitchSel1Slider);
    PitchSel2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_2", PitchSel2Slider);
    PitchSel3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_3", PitchSel3Slider);
    PitchSel4Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_4", PitchSel4Slider);
    PitchSel5Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_5", PitchSel5Slider);
    
    addAndMakeVisible(PitchSel1Slider);
    addAndMakeVisible(PitchSel2Slider);
    addAndMakeVisible(PitchSel3Slider);
    addAndMakeVisible(PitchSel4Slider);
    addAndMakeVisible(PitchSel5Slider);
    
    
    //gain

    GainOut1Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    GainOut2Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    GainOut3Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    GainOut4Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    GainOut5Slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    
    GainOut1Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    GainOut2Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    GainOut3Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    GainOut4Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    GainOut5Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);

    GainOut1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_Out_1", GainOut1Slider);
    GainOut2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_Out_2", GainOut2Slider);
    GainOut3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_Out_3", GainOut3Slider);
    GainOut4Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_Out_4", GainOut4Slider);
    GainOut5Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Gain_Out_5", GainOut5Slider);
    
    addAndMakeVisible(GainOut1Slider);
    addAndMakeVisible(GainOut2Slider);
    addAndMakeVisible(GainOut3Slider);
    addAndMakeVisible(GainOut4Slider);
    addAndMakeVisible(GainOut5Slider);
    
   
    
    //chords
    
    ChordSelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    RootSelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    
    ChordSelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    RootSelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    
    addAndMakeVisible(ChordSelSlider);
    addAndMakeVisible(RootSelSlider);
    
    ChordSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Chord_Sel", ChordSelSlider);
    RootSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Root_Sel", RootSelSlider);


    //lock button
    
//    
//    ChordHoldAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Chord_Hold", chordHoldButton);
    
    ChordHoldAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Chord_Hold", chordHoldButton);
    
    //chordHoldButton.changeWidthToFitText();
    
    chordHoldButton.setClickingTogglesState(true);

    

    chordHoldButton.onClick  = [&]() {
        const auto buttonMessage = chordHoldButton.getToggleState() ? "Held" : "Hold";
        chordHoldButton.setButtonText(buttonMessage);
        std::cout << chordHoldButton.getToggleState();
        
        
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
    //add menu item
    
    //dry wet slider
    
    DryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, text_box_height, text_box_width);
    
    
    
    DryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Dry_Wet", DryWetSlider);
 
    
    addAndMakeVisible(DryWetSlider);
    
    
    
    setSize (650, 240);
    

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

}
//==============================================================================
void PipeDreamAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto sidePaddingWidth = 120;
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    
    juce::Rectangle<int> filterControlsR = filterControls.getBounds();
    juce::Rectangle<int> chordControlsR = chordControls.getBounds();
    juce::Rectangle<int> pitchSelControlR = pitchSelControls.getBounds();
    juce::Rectangle<int> gainControlsR = gainControls.getBounds();

    g.setColour(juce::Colours::grey);
    g.drawRect(pitchSelControlR);
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(gainControlsR);
    
    g.setColour (juce::Colours::red);
    g.drawRect(filterControlsR);
    g.drawRect(chordControlsR);
    
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
    
    
    loadBtn.setBounds(btnX, btnY, btnWidth, btnHeight);
    irName.setBounds(loadBtn.getX() + loadBtn.getWidth(), btnY, btnWidth * 2, btnHeight);
    
    filterControls.setBounds(bounds.removeFromLeft(sidePaddingWidth));
    chordControls.setBounds(bounds.removeFromRight(sidePaddingWidth));
    pitchSelControls.setBounds(bounds.removeFromTop(bounds.getHeight()/2));
    gainControls.setBounds(bounds);

    //pitch flex box
     auto pitchBounds = pitchSelControls.getBounds().reduced(10);

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
    
    //gain flexbox
    
    auto gainBounds = gainControls.getBounds().reduced(10);

    juce::FlexBox flexBoxgain;
    flexBoxgain.flexDirection = juce::FlexBox::Direction::row;
    flexBoxgain.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    //auto spacer = juce::FlexItem().withWidth(2);
    
    flexBoxgain.items.add(spacer);
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
   
    //chords flexbox
    
    auto chordBounds = chordControls.getBounds().reduced(10);

    auto buttonBounds = chordBounds.removeFromTop(30);
    chordHoldButton.setBounds(buttonBounds);
    //buttonBounds.getX() + buttonBounds.getX() / 2, buttonBounds.getY(), 50, 50)
    juce::FlexBox flexBoxChords;
    flexBoxChords.flexDirection = juce::FlexBox::Direction::column;
    flexBoxChords.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    flexBoxChords.items.add(spacer);
    flexBoxChords.items.add(juce::FlexItem(RootSelSlider).withFlex(1.f));
    flexBoxChords.items.add(spacer);
    flexBoxChords.items.add(juce::FlexItem(ChordSelSlider).withFlex(1.f));
    flexBoxChords.items.add(spacer);
    
    flexBoxChords.performLayout(chordBounds);
    
    //filter flexbox
    
    auto filterBounds = filterControls.getBounds().reduced(10);

    //buttonBounds.getX() + buttonBounds.getX() / 2, buttonBounds.getY(), 50, 50)
    juce::FlexBox flexBoxFilter;
    flexBoxFilter.flexDirection = juce::FlexBox::Direction::column;
    flexBoxFilter.flexWrap = juce::FlexBox::Wrap::noWrap;
    
    flexBoxFilter.items.add(spacer);
    flexBoxFilter.items.add(juce::FlexItem(DryWetSlider).withFlex(1.f));
    flexBoxFilter.items.add(spacer);
    flexBoxFilter.performLayout(filterBounds);
    
    
    
}

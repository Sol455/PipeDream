/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
    
    
    addAndMakeVisible(irName);
    
    auto slider_width = 100;
    auto slider_height = 100;
    auto slider_spacer = 100;
    
    auto text_box_height = 50;
    auto text_box_width = 25;
    
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
    
    addAndMakeVisible(PitchSel1Slider);
    addAndMakeVisible(PitchSel2Slider);
    addAndMakeVisible(PitchSel3Slider);
    addAndMakeVisible(PitchSel4Slider);
    addAndMakeVisible(PitchSel5Slider);
    
    
    //200, 100
    

    PitchSel1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_1", PitchSel1Slider);
    
    PitchSel2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_2", PitchSel2Slider);

    PitchSel3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_3", PitchSel3Slider);

    PitchSel4Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_4", PitchSel4Slider);

    PitchSel5Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pitch_Sel_5", PitchSel5Slider);


    setSize (650, 240);
                                                                                
    PitchSel1Slider.setBounds (70, getHeight() /2 - 50 , slider_width, slider_height);
    PitchSel2Slider.setBounds (70 + slider_spacer * 1 , getHeight() /2 - 50 , slider_width, slider_height);
    PitchSel3Slider.setBounds (70 + slider_spacer * 2 , getHeight() /2 - 50 , slider_width, slider_height);
    PitchSel4Slider.setBounds (70 + slider_spacer * 3 , getHeight() /2 - 50 , slider_width, slider_height);
    PitchSel5Slider.setBounds (70 + slider_spacer * 4 , getHeight() /2 - 50 , slider_width, slider_height);

}

PipeDreamAudioProcessorEditor::~PipeDreamAudioProcessorEditor()
{
}

//==============================================================================
void PipeDreamAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void PipeDreamAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    const auto btnX = getWidth() * 0.35;
    const auto btnY = getHeight() * 0.15;
    const auto btnWidth = getWidth() * 0.15;
    const auto btnHeight = btnWidth * 0.5;
    
    loadBtn.setBounds(btnX, btnY, btnWidth, btnHeight);
    irName.setBounds(loadBtn.getX() + loadBtn.getWidth(), btnY, btnWidth * 2, btnHeight);
    
    PitchSel1Slider.setBounds (getWidth() / 2 - 100, getHeight() /2 - 50 , 200, 100);
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PipeDreamAudioProcessorEditor::PipeDreamAudioProcessorEditor (PipeDreamAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(loadBtn);
    loadBtn.setButtonText("Load IR");

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
//                audioProcessor.variableTree.setProperty("file1", result.getFullPathName(), nullptr);
//                audioProcessor.variableTree.setProperty("root", result.getParentDirectory().getFullPathName(), nullptr);
                audioProcessor.irLoader.reset();
                audioProcessor.irLoader.loadImpulseResponse(audioProcessor.savedFile,
                                                            juce::dsp::Convolution::Stereo::yes,
                                                            juce::dsp::Convolution::Trim::yes,
                                                            0);
                
                irName.setText(result.getFileName(), juce::dontSendNotification);
            }
        });
    };
    
    
    addAndMakeVisible(irName);
    
    setSize (400, 300);
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
}

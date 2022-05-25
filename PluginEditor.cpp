/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MuScriptAudioProcessorEditor::MuScriptAudioProcessorEditor (MuScriptAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    setResizable(true, false);
    addAndMakeVisible(inputLabel);
    inputLabel.setText("Text input:", juce::dontSendNotification);
    inputLabel.attachToComponent(&inputText, true);
    inputLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    inputLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(inputText);
    String fonts = ("Consolas");
    Font font(fonts, 16.0f, juce::Font::bold);
    inputText.setFont(font);
    inputText.setMultiLine(true, true);
    if(audioProcessor.s.size() > 1)
        inputText.setText(audioProcessor.s);
    inputText.setReturnKeyStartsNewLine(true);
    inputText.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    inputText.addListener(&listener);
    inputText.onTextChange = [&] {audioProcessor.s = inputText.getText().toStdString(); audioProcessor.updateNeeded = true; };

}

void MuScriptAudioProcessorEditor::updateString()
{
    inputText.setText(audioProcessor.s);
}

void MuScriptAudioProcessorEditor::textEditorTextChanged(TextEditor& textEditor)
{
    audioProcessor.s = textEditor.getText().toStdString();
    audioProcessor.updateNeeded = true;
}

MuScriptAudioProcessorEditor::~MuScriptAudioProcessorEditor()
{
}

//==============================================================================
void MuScriptAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    if (audioProcessor.s != inputText.getText())
        updateString();
}

void MuScriptAudioProcessorEditor::resized()
{
    inputText.setBounds(100, 50, getWidth() - 120, getHeight() - 70);
}

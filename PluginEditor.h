/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
using namespace juce;

//==============================================================================
/**
*/
class MuScriptAudioProcessorEditor  : public juce::AudioProcessorEditor, public TextEditor::Listener
{
public:
    void textEditorTextChanged(TextEditor& textEditor);

    MuScriptAudioProcessorEditor (MuScriptAudioProcessor&);
    ~MuScriptAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void updateString();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    MuScriptAudioProcessor& audioProcessor;
    Label inputLabel;
    Listener listener;
    TextEditor inputText;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MuScriptAudioProcessorEditor)
};


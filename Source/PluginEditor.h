/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Controls.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer, public juce::FileDragAndDropTarget
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

    void filesDropped(const juce::StringArray& files, int x, int y);
    bool isInterestedInFileDrag(const juce::StringArray& files) { return true; }
    void fileDragEnter(const juce::StringArray& files, int x, int y) {}
    void fileDragExit(const juce::StringArray& files) {}
    void fileDragMove(const juce::StringArray& files, int x, int y) {}

    bool keyPressed(const juce::KeyPress& key) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    void drawNextLineOfSpectrogram();

    Controls c;

    NewProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};

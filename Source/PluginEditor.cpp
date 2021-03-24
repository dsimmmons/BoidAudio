/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p), c(p.p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    startTimerHz(30);
    setWantsKeyboardFocus(true);

    addAndMakeVisible(c);

    setSize(800 * 1.2f, 800 * 1.2f);

    setResizable(true, true);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    stopTimer();
}

void NewProjectAudioProcessorEditor::drawNextLineOfSpectrogram()
{

    audioProcessor.p.forwardFFT.performFrequencyOnlyForwardTransform(audioProcessor.p.fftData.data());                   // [2]

    auto maxLevel = juce::FloatVectorOperations::findMinAndMax(audioProcessor.p.fftData.data(), audioProcessor.p.fftSize / 2); // [3]

    for (auto y = 1; y < audioProcessor.p.fftWeights.size(); ++y)                                              // [4]
    {
        const float indexA = float(y) / audioProcessor.p.fftWeights.size();
        const int index = indexA * indexA * indexA * audioProcessor.p.fftWeights.size() * 0.5f;
        audioProcessor.p.fftWeights[y] = audioProcessor.p.fftData[index] / maxLevel.getEnd();
    }
}

void NewProjectAudioProcessorEditor::timerCallback()
{
    // This timer callback is used to repaint the screen
    audioProcessor.getParam()->updateBirdPosition = true;
    if (audioProcessor.p.nextFFTBlockReady)
    {
        drawNextLineOfSpectrogram();
        audioProcessor.p.nextFFTBlockReady = false;
    }
    repaint();
}



//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Paint Background
    g.fillAll(juce::Colours::black);

    
    // Update the number of birds if the Num Birds Slider has been changed
    while (audioProcessor.birds.size() > audioProcessor.p.numBirds)
    {
        audioProcessor.birds[audioProcessor.birds.size() - 1].disconnect();
        audioProcessor.birds.pop_back();
    }
    while (audioProcessor.birds.size() < audioProcessor.p.numBirds)
    {
        audioProcessor.birds.push_back(Bird(audioProcessor.birds.size(), &audioProcessor.p, &audioProcessor.birds, &audioProcessor.birdMap, &audioProcessor.birdMapCount));
    }

    if (audioProcessor.p.updateBirdMapNum)
    {
        audioProcessor.p.xNumSpots = int(1.0f / (audioProcessor.p.birdVision));
        audioProcessor.p.yNumSpots = int(1.0f / (audioProcessor.p.birdVision));
        if (audioProcessor.birdMap.size() > 0)
        {
            for (int i = 0; i < audioProcessor.birds.size(); i++)
            {
                audioProcessor.birds[i].disconnect();
                audioProcessor.birds[i].xMapIndex = -1;
                audioProcessor.birds[i].yMapIndex = -1;
            }
        }
        audioProcessor.birdMap.clear();
        audioProcessor.birdMap.resize(audioProcessor.p.yNumSpots);

        audioProcessor.birdMapCount.clear();
        audioProcessor.birdMapCount.resize(audioProcessor.p.yNumSpots);

        audioProcessor.p.loadImIntoGrid();

        for (int i = 0; i < audioProcessor.p.yNumSpots; i++)
        {
            audioProcessor.birdMap[i].resize(audioProcessor.p.xNumSpots, nullptr);
            audioProcessor.birdMapCount[i].resize(audioProcessor.p.xNumSpots, 0);
        }
        audioProcessor.p.updateBirdMapNum = false;
    }

    // Iterate through each birds, update the position and paint.
    for (int i = 0; i < audioProcessor.birds.size(); i++)
    {
        // Changing the slider values calls the repaint function, so we only want to update the position of the birds when this is called from the framerate timer , NOT from the slider.
        // This updateBirdPosition flag is used to achieve that
        if (audioProcessor.p.updateBirdPosition)
        {
            audioProcessor.birds[i].updatePosition();
        }
        audioProcessor.birds[i].paintBird(g, 0, 0, getWidth(), getHeight());
    }
    audioProcessor.p.updateBirdPosition = false;
}

void NewProjectAudioProcessorEditor::resized()
{
    audioProcessor.getParam()->fftWeights.clear();
    audioProcessor.getParam()->fftWeights.resize(getHeight(), 0.0f);
    c.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
}

void NewProjectAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    audioProcessor.getParam()->mouseX = float(e.x) / getWidth();
    audioProcessor.getParam()->mouseY = float(e.y) / getHeight();
    audioProcessor.getParam()->mouseDown = true;
}

void NewProjectAudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
    audioProcessor.getParam()->mouseX = float(e.x) / getWidth();
    audioProcessor.getParam()->mouseY = float(e.y) / getHeight();
    audioProcessor.getParam()->mouseDown = true;
}

void NewProjectAudioProcessorEditor::mouseUp(const juce::MouseEvent& e)
{
    audioProcessor.getParam()->mouseDown = false;
}

bool NewProjectAudioProcessorEditor::keyPressed(const juce::KeyPress& key)
{
    if (key.getTextCharacter() == 'l')
    {
        audioProcessor.getParam()->colorsOn = !audioProcessor.getParam()->colorsOn;
    }

    if (key.getTextCharacter() == 'c')
    {
        c.setEnabled(!c.isEnabled());
        c.setVisible(!c.isVisible());
    }

    return false;
}

void NewProjectAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::File f(files[0]);

    audioProcessor.getParam()->im = juce::ImageFileFormat::loadFrom(f);
    audioProcessor.getParam()->loadImIntoGrid();

}

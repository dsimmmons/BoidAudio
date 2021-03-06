/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p), c(p.p), Thread("Velocity Update Thread")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    startTimerHz(30);
    setWantsKeyboardFocus(true);

    addAndMakeVisible(c);

    setSize(800 * 1.2f, 800 * 1.2f);

    setResizable(true, true);
    startThread();
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    stopThread(500);
    stopTimer();
}

void NewProjectAudioProcessorEditor::run() 
{
    while (!threadShouldExit())
    {
        {
            ScopedLock lk(lock);
            for (int i = 0; i < audioProcessor.birds.size(); i++)
            {
                audioProcessor.birds[i].updateVelocity();
            }
            // Update the number of birds if the Num Birds Slider has been changed
            while (audioProcessor.birds.size() > audioProcessor.p.numBirds)
            {
                audioProcessor.birds[audioProcessor.birds.size() - 1].disconnect();
                audioProcessor.birds.pop_back();
            }
            while (audioProcessor.birds.size() < audioProcessor.p.numBirds)
            {
                audioProcessor.birds.emplace_back(audioProcessor.birds.size(), &audioProcessor.p, &audioProcessor.birds, &audioProcessor.birdMap, &audioProcessor.birdMapCount);
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
        }
        wait(50);
    }
}

void NewProjectAudioProcessorEditor::timerCallback()
{
    // This timer callback is used to repaint the screen
    audioProcessor.getParam()->updateBirdPosition = true;
    repaint();
}



//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Paint Background
    g.fillAll(juce::Colours::black);

    ScopedLock lk(lock);
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
    const int width = getWidth();
    setSize(width, width);
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

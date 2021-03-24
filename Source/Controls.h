/*
  ==============================================================================

    Controls.h
    Created: 19 Mar 2021 6:20:46pm
    Author:  denve

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "parameters.h"

//==============================================================================
/*
*/
class Controls : public juce::Component
{
public:
    Controls(parameters& param) : p(param)
    {
        numBirdsSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        numBirdsSlider.onValueChange = [this]()->void {numBirdsChanged(); };
        numBirdsSlider.setRange(0.0f, 7000, 1.0f);
        numBirdsSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        numBirdsSlider.setValue(p.numBirds);
        addAndMakeVisible(numBirdsSlider);
        numBirdsLabel.setJustificationType(juce::Justification::centred);
        numBirdsLabel.setText("Num Birds", juce::dontSendNotification);
        addAndMakeVisible(numBirdsLabel);

        velocitySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        velocitySlider.onValueChange = [this]()->void {velocityChanged(); };
        velocitySlider.setRange(0.00001f, p.velocityMax);
        velocitySlider.setSkewFactorFromMidPoint(0.02);
        velocitySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        velocitySlider.setValue(p.defaultVelocity);
        addAndMakeVisible(velocitySlider);
        velocityLabel.setJustificationType(juce::Justification::centred);
        velocityLabel.setText("Speed", juce::dontSendNotification);
        addAndMakeVisible(velocityLabel);

        avoidSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        avoidSlider.onValueChange = [this]()->void {avoidChanged(); };
        avoidSlider.setRange(0.0f, p.avoidMax);
        avoidSlider.setSkewFactorFromMidPoint(0.05);
        avoidSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        avoidSlider.setValue(p.avoidWeight);
        addAndMakeVisible(avoidSlider);
        avoidLabel.setJustificationType(juce::Justification::centred);
        avoidLabel.setText("Avoid", juce::dontSendNotification);
        addAndMakeVisible(avoidLabel);

        matchSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        matchSlider.onValueChange = [this]()->void {matchChanged(); };
        matchSlider.setRange(0.0f, p.matchVelocityMax);
        matchSlider.setSkewFactorFromMidPoint(0.1);
        matchSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        matchSlider.setValue(p.matchVelocityWeight);
        addAndMakeVisible(matchSlider);
        matchLabel.setJustificationType(juce::Justification::centred);
        matchLabel.setText("Match", juce::dontSendNotification);
        addAndMakeVisible(matchLabel);

        groupSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        groupSlider.onValueChange = [this]()->void {groupChanged(); };
        groupSlider.setRange(0.0f, p.groupMax);
        groupSlider.setSkewFactorFromMidPoint(0.005);
        groupSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        groupSlider.setValue(p.groupWeight);
        addAndMakeVisible(groupSlider);
        groupLabel.setJustificationType(juce::Justification::centred);
        groupLabel.setText("Group", juce::dontSendNotification);
        addAndMakeVisible(groupLabel);

        visionSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        visionSlider.onValueChange = [this]()->void {visionChanged(); };
        visionSlider.setRange(0.001f, 0.33f);
        visionSlider.setSkewFactorFromMidPoint(0.07f);
        visionSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        visionSlider.setValue(p.birdVision);
        visionSlider.addMouseListener(this, true);
        visionSlider.onDragStart = [this]()->void { p.visionMoving = true; };
        visionSlider.onDragEnd = [this]()->void { p.visionMoving = false; p.updateBirdMapNum = true; };
        addAndMakeVisible(visionSlider);
        visionLabel.setJustificationType(juce::Justification::centred);
        visionLabel.setText("Vision", juce::dontSendNotification);
        addAndMakeVisible(visionLabel);

        sizeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        sizeSlider.onValueChange = [this]()->void {p.birdSize = sizeSlider.getValue(); };
        sizeSlider.setRange(0.0f, 0.1);
        sizeSlider.setSkewFactorFromMidPoint(0.01);
        sizeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        sizeSlider.setValue(p.birdSize);
        sizeSlider.setDoubleClickReturnValue(0.001, juce::NotificationType::sendNotification);
        addAndMakeVisible(sizeSlider);
        sizeLabel.setJustificationType(juce::Justification::centred);
        sizeLabel.setText("Bird Size", juce::dontSendNotification);
        addAndMakeVisible(sizeLabel);

        volumeSpeedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        volumeSpeedSlider.onValueChange = [this]()->void {p.volumeSpeedMod = volumeSpeedSlider.getValue(); };
        volumeSpeedSlider.setRange(-1.0f, 1.0f);
        volumeSpeedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        volumeSpeedSlider.setValue(0.0f);
        volumeSpeedSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(volumeSpeedSlider);
        volumeSpeedLabel.setJustificationType(juce::Justification::centred);
        volumeSpeedLabel.setText("Volume Speed Mod", juce::dontSendNotification);
        addAndMakeVisible(volumeSpeedLabel);

        volumeAvoidSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        volumeAvoidSlider.onValueChange = [this]()->void {p.volumeAvoidMod = volumeAvoidSlider.getValue(); };
        volumeAvoidSlider.setRange(-1.0f, 1.0f);
        volumeAvoidSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        volumeAvoidSlider.setValue(0.0f);
        volumeAvoidSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(volumeAvoidSlider);
        volumeAvoidLabel.setJustificationType(juce::Justification::centred);
        volumeAvoidLabel.setText("Volume Avoid Mod", juce::dontSendNotification);
        addAndMakeVisible(volumeAvoidLabel);

        volumeMatchSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        volumeMatchSlider.onValueChange = [this]()->void {p.volumeMatchMod = volumeMatchSlider.getValue(); };
        volumeMatchSlider.setRange(-1.0f, 1.0f);
        volumeMatchSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        volumeMatchSlider.setValue(0.0f);
        volumeMatchSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(volumeMatchSlider);
        volumeMatchLabel.setJustificationType(juce::Justification::centred);
        volumeMatchLabel.setText("Volume Match Mod", juce::dontSendNotification);
        addAndMakeVisible(volumeMatchLabel);

        volumeGroupSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        volumeGroupSlider.onValueChange = [this]()->void {p.volumeGroupMod = volumeGroupSlider.getValue(); };
        volumeGroupSlider.setRange(-1.0f, 1.0f);
        volumeGroupSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        volumeGroupSlider.setValue(0.0f);
        volumeGroupSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(volumeGroupSlider);
        volumeGroupLabel.setJustificationType(juce::Justification::centred);
        volumeGroupLabel.setText("Volume Group Mod", juce::dontSendNotification);
        addAndMakeVisible(volumeGroupLabel);

        volumeColorSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        volumeColorSlider.onValueChange = [this]()->void {p.volumeColorMod = volumeColorSlider.getValue(); };
        volumeColorSlider.setRange(0.0f, 1.0f);
        volumeColorSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        volumeColorSlider.setValue(0.0f);
        addAndMakeVisible(volumeColorSlider);
        volumeColorLabel.setJustificationType(juce::Justification::centred);
        volumeColorLabel.setText("Volume Color Mod", juce::dontSendNotification);
        addAndMakeVisible(volumeColorLabel);

        volumeWeightSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        volumeWeightSlider.onValueChange = [this]()->void {p.volumeTotalWeight = volumeWeightSlider.getValue(); };
        volumeWeightSlider.setRange(0.0f, 5.0f);
        volumeWeightSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        volumeWeightSlider.setDoubleClickReturnValue(true, 0.0f);
        volumeWeightSlider.setValue(0.0f);
        addAndMakeVisible(volumeWeightSlider);
        volumeWeightLabel.setJustificationType(juce::Justification::centred);
        volumeWeightLabel.setText("Volume Weight", juce::dontSendNotification);
        addAndMakeVisible(volumeWeightLabel);


        lpHzSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        lpHzSlider.onValueChange = [this]()->void {p.lpCuttoff = lpHzSlider.getValue(); };
        lpHzSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        lpHzSlider.setRange(20, 19999.0);
        lpHzSlider.setValue(2000);
        lpHzSlider.setSkewFactorFromMidPoint(700);
        addAndMakeVisible(lpHzSlider);
        lpHzLabel.setJustificationType(juce::Justification::centred);
        lpHzLabel.setText("LP Cutoff", juce::dontSendNotification);
        addAndMakeVisible(lpHzLabel);

        lpSpeedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        lpSpeedSlider.onValueChange = [this]()->void {p.lpSpeedMod = lpSpeedSlider.getValue(); };
        lpSpeedSlider.setRange(-1.0f, 1.0f);
        lpSpeedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        lpSpeedSlider.setValue(0.0f);
        lpSpeedSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(lpSpeedSlider);
        lpSpeedLabel.setJustificationType(juce::Justification::centred);
        lpSpeedLabel.setText("LP Speed Mod", juce::dontSendNotification);
        addAndMakeVisible(lpSpeedLabel);

        lpAvoidSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        lpAvoidSlider.onValueChange = [this]()->void {p.lpAvoidMod = lpAvoidSlider.getValue(); };
        lpAvoidSlider.setRange(-1.0f, 1.0f);
        lpAvoidSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        lpAvoidSlider.setValue(0.0f);
        lpAvoidSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(lpAvoidSlider);
        lpAvoidLabel.setJustificationType(juce::Justification::centred);
        lpAvoidLabel.setText("LP Avoid Mod", juce::dontSendNotification);
        addAndMakeVisible(lpAvoidLabel);

        lpMatchSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        lpMatchSlider.onValueChange = [this]()->void {p.lpMatchMod = lpMatchSlider.getValue(); };
        lpMatchSlider.setRange(-1.0f, 1.0f);
        lpMatchSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        lpMatchSlider.setValue(0.0f);
        lpMatchSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(lpMatchSlider);
        lpMatchLabel.setJustificationType(juce::Justification::centred);
        lpMatchLabel.setText("LP Match Mod", juce::dontSendNotification);
        addAndMakeVisible(lpMatchLabel);

        lpGroupSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        lpGroupSlider.onValueChange = [this]()->void {p.lpGroupMod = lpGroupSlider.getValue(); };
        lpGroupSlider.setRange(-1.0f, 1.0f);
        lpGroupSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        lpGroupSlider.setValue(0.0f);
        lpGroupSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(lpGroupSlider);
        lpGroupLabel.setJustificationType(juce::Justification::centred);
        lpGroupLabel.setText("LP Group Mod", juce::dontSendNotification);
        addAndMakeVisible(lpGroupLabel);

        lpWeightSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        lpWeightSlider.onValueChange = [this]()->void {p.lpTotalWeight = lpWeightSlider.getValue(); };
        lpWeightSlider.setRange(0.0f, 5.0f);
        lpWeightSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        lpWeightSlider.setValue(0.0f);
        addAndMakeVisible(lpWeightSlider);
        lpWeightLabel.setJustificationType(juce::Justification::centred);
        lpWeightLabel.setText("LP Weight", juce::dontSendNotification);
        addAndMakeVisible(lpWeightLabel);



        processRatioSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        processRatioSlider.onValueChange = [this]()->void {p.processRatio = processRatioSlider.getValue(); };
        processRatioSlider.setRange(0.0f, 1.0f);
        processRatioSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        processRatioSlider.setValue(1.0f);
        addAndMakeVisible(processRatioSlider);
        processRatioLabel.setJustificationType(juce::Justification::centred);
        processRatioLabel.setText("Process Ratio", juce::dontSendNotification);
        addAndMakeVisible(processRatioLabel);

        gridCompSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        gridCompSlider.onValueChange = [this]()->void {p.gridCompensation = gridCompSlider.getValue(); };
        gridCompSlider.setRange(0.0f, 1.0f);
        gridCompSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        gridCompSlider.setValue(0.0f);
        addAndMakeVisible(gridCompSlider);
        gridCompLabel.setJustificationType(juce::Justification::centred);
        gridCompLabel.setText("Grid Compensation", juce::dontSendNotification);
        addAndMakeVisible(gridCompLabel);

        imagePullSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        imagePullSlider.onValueChange = [this]()->void {p.imagePull = imagePullSlider.getValue(); };
        imagePullSlider.setRange(-p.imagePullMax, p.imagePullMax);
        imagePullSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        imagePullSlider.setValue(0.0f);
        imagePullSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(imagePullSlider);
        imagePullLabel.setJustificationType(juce::Justification::centred);
        imagePullLabel.setText("Image Pull", juce::dontSendNotification);
        addAndMakeVisible(imagePullLabel);

        imageSlideSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        imageSlideSlider.onValueChange = [this]()->void {p.imageSlide = imageSlideSlider.getValue(); };
        imageSlideSlider.setRange(-p.imageSlideMax, p.imageSlideMax);
        imageSlideSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        imageSlideSlider.setValue(0.0f);
        imageSlideSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(imageSlideSlider);
        imageSlideLabel.setJustificationType(juce::Justification::centred);
        imageSlideLabel.setText("Image Slide", juce::dontSendNotification);
        addAndMakeVisible(imageSlideLabel);


        circleWeightSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        circleWeightSlider.onValueChange = [this]()->void {p.circleWeight = circleWeightSlider.getValue(); };
        circleWeightSlider.setRange(0.0f, p.circleWeightMax);
        circleWeightSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        circleWeightSlider.setValue(0.0f);
        circleWeightSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(circleWeightSlider);
        circleWeightLabel.setJustificationType(juce::Justification::centred);
        circleWeightLabel.setText("Circle Weight", juce::dontSendNotification);
        addAndMakeVisible(circleWeightLabel);

        circlePullSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        circlePullSlider.onValueChange = [this]()->void {p.circlePullMod = circlePullSlider.getValue(); };
        circlePullSlider.setRange(-p.circlePullMax, p.circlePullMax);
        circlePullSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        circlePullSlider.setValue(0.0f);
        circlePullSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(circlePullSlider);
        circlePullLabel.setJustificationType(juce::Justification::centred);
        circlePullLabel.setText("Circle Pull", juce::dontSendNotification);
        addAndMakeVisible(circlePullLabel);

        circleSlideSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        circleSlideSlider.onValueChange = [this]()->void {p.circleSlideMod = circleSlideSlider.getValue(); };
        circleSlideSlider.setRange(-p.circleSlideMax, p.circleSlideMax);
        circleSlideSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        circleSlideSlider.setValue(0.0f);
        circleSlideSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(circleSlideSlider);
        circleSlideLabel.setJustificationType(juce::Justification::centred);
        circleSlideLabel.setText("Circle Slide", juce::dontSendNotification);
        addAndMakeVisible(circleSlideLabel);

        circleSpeedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        circleSpeedSlider.onValueChange = [this]()->void {p.circleSpeedMod = circleSpeedSlider.getValue(); };
        circleSpeedSlider.setRange(-p.circleSpeedMax, p.circleSpeedMax);
        circleSpeedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        circleSpeedSlider.setValue(0.0f);
        circleSpeedSlider.setDoubleClickReturnValue(true, 0.0f);
        addAndMakeVisible(circleSpeedSlider);
        circleSpeedLabel.setJustificationType(juce::Justification::centred);
        circleSpeedLabel.setText("Circle Speed", juce::dontSendNotification);
        addAndMakeVisible(circleSpeedLabel);

        saveButton.setButtonText("Save");
        saveButton.onClick = [this]()->void {saveClicked(); };
        addAndMakeVisible(saveButton);

        loadButton.setButtonText("Load");
        loadButton.onClick = [this]()->void {loadClicked(); };
        addAndMakeVisible(loadButton);

        tpZeroButton.setButtonText("TP Zero");
        tpZeroButton.setClickingTogglesState(true);
        tpZeroButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::red);
        tpZeroButton.onClick = [this]()->void {p.tpToZero = tpZeroButton.getToggleState(); };
        addAndMakeVisible(tpZeroButton);

    }

    ~Controls() override
    {
    }

    void paint(juce::Graphics& g) override
    {

    }

    void resized() override
    {
        const float width = 1.0f / 7;
        const float height = 1.0f / 7;

        saveButton.setBoundsRelative(0.02f, 0.02f, 0.05f, 0.025f);
        loadButton.setBoundsRelative(0.08f, 0.02f, 0.05f, 0.025f);
        tpZeroButton.setBoundsRelative(0.15f, 0.02f, 0.05f, 0.025f);

        numBirdsSlider.setBoundsRelative(0.0f, height, width, height * 0.9f);
        numBirdsLabel.setBoundsRelative(0.0f, height * 1.9f, width, height * 0.1f);
        velocitySlider.setBoundsRelative(width, height, width, height * 0.9f);
        velocityLabel.setBoundsRelative(width, height * 1.9f, width, height * 0.1f);
        visionSlider.setBoundsRelative(width * 2, height, width, height * 0.9f);
        visionLabel.setBoundsRelative(width * 2, height * 1.9f, width, height * 0.1f);
        avoidSlider.setBoundsRelative(width * 3, height, width, height * 0.9f);
        avoidLabel.setBoundsRelative(width * 3, height * 1.9f, width, height * 0.1f);
        matchSlider.setBoundsRelative(width * 4, height, width, height * 0.9f);
        matchLabel.setBoundsRelative(width * 4, height * 1.9f, width, height * 0.1f);
        groupSlider.setBoundsRelative(width * 5, height, width, height * 0.9f);
        groupLabel.setBoundsRelative(width * 5, height * 1.9f, width, height * 0.1f);
        sizeSlider.setBoundsRelative(width * 6, height, width, height * 0.9f);
        sizeLabel.setBoundsRelative(width * 6, height * 1.9f, width, height * 0.1f);

        volumeWeightSlider.setBoundsRelative(0.0f, height * 2.0f, width, height * 0.9f);
        volumeWeightLabel.setBoundsRelative(0.0f, height * 2.9f, width, height * 0.1f);
        volumeColorSlider.setBoundsRelative(width, height * 2.0f, width, height * 0.9f);
        volumeColorLabel.setBoundsRelative(width, height * 2.9f, width, height * 0.1f);
        volumeSpeedSlider.setBoundsRelative(width * 2, height * 2.0f, width, height * 0.9f);
        volumeSpeedLabel.setBoundsRelative(width * 2, height * 2.9f, width, height * 0.1f);
        volumeAvoidSlider.setBoundsRelative(width * 3, height * 2.0f, width, height * 0.9f);
        volumeAvoidLabel.setBoundsRelative(width * 3, height * 2.9f, width, height * 0.1f);
        volumeMatchSlider.setBoundsRelative(width * 4, height * 2.0f, width, height * 0.9f);
        volumeMatchLabel.setBoundsRelative(width * 4, height * 2.9f, width, height * 0.1f);
        volumeGroupSlider.setBoundsRelative(width * 5, height * 2.0f, width, height * 0.9f);
        volumeGroupLabel.setBoundsRelative(width * 5, height * 2.9f, width, height * 0.1f);
        

        lpWeightSlider.setBoundsRelative(0.0, height * 3, width, height * 0.9f);
        lpWeightLabel.setBoundsRelative(0.0, height * 3.9f, width, height * 0.1f);
        lpHzSlider.setBoundsRelative(width, height * 3, width, height * 0.9f);
        lpHzLabel.setBoundsRelative(width, height * 3.9f, width, height * 0.1f);
        lpSpeedSlider.setBoundsRelative(width * 2, height * 3, width, height * 0.9f);
        lpSpeedLabel.setBoundsRelative(width * 2, height * 3.9f, width, height * 0.1f);
        lpAvoidSlider.setBoundsRelative(width * 3, height * 3, width, height * 0.9f);
        lpAvoidLabel.setBoundsRelative(width * 3, height * 3.9f, width, height * 0.1f);
        lpMatchSlider.setBoundsRelative(width * 4, height * 3, width, height * 0.9f);
        lpMatchLabel.setBoundsRelative(width * 4, height * 3.9f, width, height * 0.1f);
        lpGroupSlider.setBoundsRelative(width * 5, height * 3, width, height * 0.9f);
        lpGroupLabel.setBoundsRelative(width * 5, height * 3.9f, width, height * 0.1f);

        circleWeightSlider.setBoundsRelative(0.0, height * 5, width, height * 0.9f);
        circleWeightLabel.setBoundsRelative(0.0, height * 5.9f, width, height * 0.1f);
        circleSpeedSlider.setBoundsRelative(width, height * 5, width, height * 0.9f);
        circleSpeedLabel.setBoundsRelative(width, height * 5.9f, width, height * 0.1f);
        circleSlideSlider.setBoundsRelative(width * 2, height * 5, width, height * 0.9f);
        circleSlideLabel.setBoundsRelative(width * 2, height * 5.9f, width, height * 0.1f);
        circlePullSlider.setBoundsRelative(width * 3, height * 5, width, height * 0.9f);
        circlePullLabel.setBoundsRelative(width * 3, height * 5.9f, width, height * 0.1f);

        processRatioSlider.setBoundsRelative(0.0, height * 6, width, height * 0.9f);
        processRatioLabel.setBoundsRelative(0.0, height * 6.9f, width, height * 0.1f);
        gridCompSlider.setBoundsRelative(width, height * 6, width, height * 0.9f);
        gridCompLabel.setBoundsRelative(width, height * 6.9f, width, height * 0.1f);
        imagePullSlider.setBoundsRelative(width * 2, height * 6, width, height * 0.9f);
        imagePullLabel.setBoundsRelative(width * 2, height * 6.9f, width, height * 0.1f);
        imageSlideSlider.setBoundsRelative(width * 3, height * 6, width, height * 0.9f);
        imageSlideLabel.setBoundsRelative(width * 3, height * 6.9f, width, height * 0.1f);
    }

    juce::Slider numBirdsSlider;
    juce::Label numBirdsLabel;
    juce::Slider velocitySlider;
    juce::Label velocityLabel;
    juce::Slider avoidSlider;
    juce::Label avoidLabel;
    juce::Slider matchSlider;
    juce::Label matchLabel;
    juce::Slider groupSlider;
    juce::Label groupLabel;
    juce::Slider visionSlider;
    juce::Label visionLabel;


    juce::Slider volumeSpeedSlider;
    juce::Label volumeSpeedLabel;
    juce::Slider volumeAvoidSlider;
    juce::Label volumeAvoidLabel;
    juce::Slider volumeMatchSlider;
    juce::Label volumeMatchLabel;
    juce::Slider volumeGroupSlider;
    juce::Label volumeGroupLabel;
    juce::Slider volumeColorSlider;
    juce::Label volumeColorLabel;
    juce::Slider volumeWeightSlider;
    juce::Label volumeWeightLabel;

    juce::Slider lpHzSlider;
    juce::Label lpHzLabel;
    juce::Slider lpSpeedSlider;
    juce::Label lpSpeedLabel;
    juce::Slider lpAvoidSlider;
    juce::Label lpAvoidLabel;
    juce::Slider lpMatchSlider;
    juce::Label lpMatchLabel;
    juce::Slider lpGroupSlider;
    juce::Label lpGroupLabel;
    juce::Slider lpColorSlider;
    juce::Label lpColorLabel;
    juce::Slider lpWeightSlider;
    juce::Label lpWeightLabel;

    juce::Slider sizeSlider;
    juce::Label sizeLabel;

    juce::Slider processRatioSlider;
    juce::Label processRatioLabel;
    juce::Slider gridCompSlider;
    juce::Label gridCompLabel;
    juce::Slider imagePullSlider;
    juce::Label imagePullLabel;
    juce::Slider imageSlideSlider;
    juce::Label imageSlideLabel;

    juce::Slider circleWeightSlider;
    juce::Label circleWeightLabel;
    juce::Slider circleSpeedSlider;
    juce::Label circleSpeedLabel;
    juce::Slider circleSlideSlider;
    juce::Label circleSlideLabel;
    juce::Slider circlePullSlider;
    juce::Label circlePullLabel;

    juce::TextButton saveButton;
    juce::TextButton loadButton;
    juce::TextButton tpZeroButton;

    void saveClicked()
    {
        juce::FileChooser chooser("Save Preset File", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.pv");
        if (chooser.browseForFileToSave(false))
        {
            p.savePreset(chooser.getResult());
        }
    }

    void loadClicked()
    {
        juce::FileChooser chooser("Load Preset File", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.pv");
        if (chooser.browseForFileToOpen())
        {
            p.loadPreset(chooser.getResult());
        }

        updateSliderPositions();
    }

    void updateSliderPositions()
    {
        numBirdsSlider.setValue(p.numBirds, juce::NotificationType::sendNotification);
        velocitySlider.setValue(p.defaultVelocity, juce::NotificationType::sendNotification);
        avoidSlider.setValue(p.avoidWeight, juce::NotificationType::sendNotification);
        matchSlider.setValue(p.matchVelocityWeight, juce::NotificationType::sendNotification);
        groupSlider.setValue(p.groupWeight, juce::NotificationType::sendNotification);
        visionSlider.setValue(p.birdVision, juce::NotificationType::sendNotification);

        volumeSpeedSlider.setValue(p.volumeSpeedMod, juce::NotificationType::sendNotification);
        volumeAvoidSlider.setValue(p.volumeAvoidMod, juce::NotificationType::sendNotification);
        volumeMatchSlider.setValue(p.volumeMatchMod, juce::NotificationType::sendNotification);
        volumeGroupSlider.setValue(p.volumeGroupMod, juce::NotificationType::sendNotification);
        volumeColorSlider.setValue(p.volumeColorMod, juce::NotificationType::sendNotification);
        volumeWeightSlider.setValue(p.volumeTotalWeight, juce::NotificationType::sendNotification);

        lpHzSlider.setValue(p.lpCuttoff, juce::NotificationType::sendNotification);
        lpSpeedSlider.setValue(p.lpSpeedMod, juce::NotificationType::sendNotification);
        lpAvoidSlider.setValue(p.lpAvoidMod, juce::NotificationType::sendNotification);
        lpMatchSlider.setValue(p.lpMatchMod, juce::NotificationType::sendNotification);
        lpGroupSlider.setValue(p.lpGroupMod, juce::NotificationType::sendNotification);
        lpColorSlider.setValue(p.lpColorMod, juce::NotificationType::sendNotification);
        lpWeightSlider.setValue(p.lpTotalWeight, juce::NotificationType::sendNotification);

        sizeSlider.setValue(p.birdSize, juce::NotificationType::sendNotification);

        processRatioSlider.setValue(p.processRatio, juce::NotificationType::sendNotification);
        gridCompSlider.setValue(p.gridCompensation, juce::NotificationType::sendNotification);

        tpZeroButton.setToggleState(p.tpToZero, false);
        
    }

    void velocityChanged()
    {
        p.defaultVelocity = velocitySlider.getValue();
    }
    void avoidChanged()
    {
        p.avoidWeight = avoidSlider.getValue();
    }
    void matchChanged()
    {
        p.matchVelocityWeight = matchSlider.getValue();
    }
    void groupChanged()
    {
        p.groupWeight = groupSlider.getValue();
    }
    void visionChanged()
    {
        p.birdVision = visionSlider.getValue();
    }

    void numBirdsChanged()
    {
        p.numBirds = numBirdsSlider.getValue();
    }


private:

    parameters& p;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Controls)
};

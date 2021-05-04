/*
  ==============================================================================

    parameters.h
    Created: 17 Mar 2021 2:19:10am
    Author:  denve

  ==============================================================================
*/

#pragma once
#include "Bird.h"

struct parameters
{
    parameters()
    {
        defaultVelocity = 0.005f;
        velocityMax = 0.05f;
        avoidWeight = 0.0033f;
        avoidMax = 0.8f;
        matchVelocityWeight = 0.55f;
        matchVelocityMax = 2.0f;
        groupWeight = 0.0023f;
        groupMax = 0.25f;
        birdVision = 0.005f;
        numBirds = 1600;
    }

    float defaultVelocity = 0.01f;
    float velocityMax = 0.02f;
    float moddedVelocity = 0.01f;

    float avoidWeight = 0.0033f;
    float avoidMax = 0.8f;
    float moddedAvoid = 0.0033f;

    float matchVelocityWeight = 0.55f;
    float matchVelocityMax = 2.0f;
    float moddedMatch = 0.55f;

    float groupWeight = 0.0023f;
    float groupMax = 0.25f;
    float moddedGroup = 0.0023f;

    float moddedColor = 0.0f;

    float birdVision = 0.03f;
    int numBirds = 1600;
    int maxNumBirds = 10000;
    float birdSize = 0.002f;
    bool visionMoving = false;

    bool mouseDown = false;
    float mouseX = 0.0f;
    float mouseY = 0.0f;

    float widthRatio = 1.0f;
    float heightRatio = 1.0f;

    int xNumSpots = 0;
    int yNumSpots = 0;

    bool colorsOn = true;

    float fastSqrt[10000];

    bool updateBirdMapNum = true;
    bool updateBirdPosition = false;

    float lastBufferMaxAmp = 0.0f;

    float volumeTotalWeight = 0.0f;
    float volumeSpeedMod = 0.0f;
    float volumeAvoidMod = 0.0f;
    float volumeMatchMod = 0.0f;
    float volumeGroupMod = 0.0f;
    float volumeColorMod = 0.0f;

    float lpCuttoff = 400.0f;

    float lpMaxAmp = 0.0f;

    float lpTotalWeight = 0.0f;
    float lpSpeedMod = 0.0f;
    float lpAvoidMod = 0.0f;
    float lpMatchMod = 0.0f;
    float lpGroupMod = 0.0f;
    float lpColorMod = 0.0f;

    bool tpToZero = false;
    

    float imagePull = 0.0f;
    const float imagePullMax = 1.0f;
    float imageSlide = 0.0f;
    const float imageSlideMax = 1.0f;
    juce::Image im;
    std::vector<std::vector<std::vector<std::pair<std::vector<float>, juce::Colour>>>> imagePoints;


    void loadImIntoGrid()
    {
        if (im.isValid())
        {
            imagePoints.clear();
            imagePoints.resize(yNumSpots);
            for (int i = 0; i < yNumSpots; i++)
            {
                imagePoints[i].resize(xNumSpots);
            }
            
            for (int y = 0; y < im.getHeight(); y++)
            {
                for (int x = 0; x < im.getWidth(); x++)
                {
                    if (im.getPixelAt(x, y).getBrightness() > 0.3f)
                    {
                        std::vector<float> gridPoints(2, 0);
                        gridPoints[0] = float(y) / im.getHeight();
                        gridPoints[1] = float(x) / im.getWidth();

                        const int yGrid = int(gridPoints[0] * yNumSpots);
                        const int xGrid = int(gridPoints[1] * xNumSpots);

                        std::pair<std::vector<float>, juce::Colour> imagePoint;
                        imagePoint.first = gridPoints;
                        imagePoint.second = im.getPixelAt(x, y);
                        imagePoints[yGrid][xGrid].push_back(imagePoint);
                    }
                }
            }
        }
    }

    void savePreset(juce::File f)
    {
        juce::FileOutputStream output(f);
        if (output.openedOk())
        {
            output.writeFloat(numBirds);
            output.writeFloat(defaultVelocity);
            output.writeFloat(birdVision);
            output.writeFloat(avoidWeight);
            output.writeFloat(matchVelocityWeight);
            output.writeFloat(groupWeight);
            output.writeFloat(birdSize);
            output.writeFloat(volumeTotalWeight);
            output.writeFloat(volumeColorMod);
            output.writeFloat(volumeSpeedMod);
            output.writeFloat(volumeAvoidMod);
            output.writeFloat(volumeMatchMod);
            output.writeFloat(volumeGroupMod);
            output.writeFloat(lpTotalWeight);
            output.writeFloat(lpCuttoff);
            output.writeFloat(lpSpeedMod);
            output.writeFloat(lpAvoidMod);
            output.writeFloat(lpMatchMod);
            output.writeFloat(lpGroupMod);
            output.writeFloat(processRatio);
            output.writeFloat(gridCompensation);
            output.writeBool(tpToZero);
        }
    }

    void loadPreset(juce::File f)
    {
        juce::FileInputStream input(f);
        if (input.openedOk())
        {
            numBirds = input.readFloat();
            defaultVelocity = input.readFloat();
            birdVision = input.readFloat();
            avoidWeight = input.readFloat();
            matchVelocityWeight = input.readFloat();
            groupWeight = input.readFloat();
            birdSize = input.readFloat();
            volumeTotalWeight = input.readFloat();
            volumeColorMod = input.readFloat();
            volumeSpeedMod = input.readFloat();
            volumeAvoidMod = input.readFloat();
            volumeMatchMod = input.readFloat();
            volumeGroupMod = input.readFloat();
            lpTotalWeight = input.readFloat();
            lpCuttoff = input.readFloat();
            lpSpeedMod = input.readFloat();
            lpAvoidMod = input.readFloat();
            lpMatchMod = input.readFloat();
            lpGroupMod = input.readFloat();
            processRatio = input.readFloat();
            gridCompensation = input.readFloat();
            tpToZero = input.readBool();
        }
    }


    // Optimization Parameters

    float processRatio = 1.0f;
    float gridCompensation = 0.0f;

    bool protectFramerate = false;

    float getFastSqrt(const float val)
    {
        return sqrtf(val);
        /*
        const int lookup = int((val / 10.0f) * 10000.0f);
        if (lookup > 10000)
        {
            return sqrtf(val);
        }
        else
        {
            return fastSqrt[lookup];
        }
        */
    }


};
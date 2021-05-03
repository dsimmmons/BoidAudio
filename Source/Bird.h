/*
  ==============================================================================

    Bird.h
    Created: 16 Mar 2021 10:48:06pm
    Author:  denve

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "parameters.h"

class Bird
{
public:
    Bird(int i, parameters* param, std::vector<Bird>* b, std::vector<std::vector<Bird*>>* bMap, std::vector<std::vector<int>>* c)
    {
        p = param;
        birds = b;
        birdMap = bMap;
        birdMapCount = c;


        id = i;
        const float randHue = float(rand()) / RAND_MAX;
        color = juce::Colour(randHue, 1.0f, 1.0f, 1.0f);

        setRandomPosition();


        xVelocity = (float(rand()) / RAND_MAX) - 0.5f;
        yVelocity = (float(rand()) / RAND_MAX) - 0.5f;
        setVelocity(p->defaultVelocity);
    }

    void disconnect()
    {
        // Remove this bird from birdmap
        if (yMapIndex > -1 && xMapIndex > -1)
        {
            birdMapCount->at(yMapIndex).at(xMapIndex)--;
            if (birdMap->at(yMapIndex).at(xMapIndex) == this)
            {
                birdMap->at(yMapIndex).at(xMapIndex) = next;
                if (next != nullptr)
                {
                    next->prev = nullptr;
                }
            }
            else
            {
                if (next != nullptr)
                {
                    next->prev = prev;
                }
                if (prev != nullptr)
                {
                    prev->next = next;
                }
            }
            next = nullptr;
            prev = nullptr;
        }
    }

    void reconnect()
    {
        birdMapCount->at(yMapIndex).at(xMapIndex)++;
        next = birdMap->at(yMapIndex).at(xMapIndex);
        if (next != nullptr)
        {
            next->prev = this;
        }
        birdMap->at(yMapIndex).at(xMapIndex) = this;
    }

    void updateVelocity()
    {
        float averageX = 0.0f;
        float averageY = 0.0f;

        int numBirdsInView = 0;
        int numBirdsInZone = 0;
        int numAllBirdsInZones = 0;
        const float vision = p->birdVision;

        const float oldXVelocity = xVelocity;
        const float oldYVelocity = yVelocity;

        const float avoidBirdVal = p->moddedAvoid;
        const float matchBirdVal = p->moddedMatch;
        const float groupBirdVal = p->moddedGroup;

        float accumulatedXVelocity = 0.0f;
        float accumulatedYVelocity = 0.0f;
        if (xMapIndex > -1 && yMapIndex > -1)
        {
            for (int yInd = -1; yInd < 2; yInd++)
            {
                const int thisYPos = (yMapIndex + yInd);
                if (thisYPos > -1 && thisYPos < p->yNumSpots)
                {
                    for (int xInd = -1; xInd < 2; xInd++)
                    {
                        const int thisXPos = (xMapIndex + xInd);

                        if (thisXPos > -1 && thisXPos < p->xNumSpots)
                        {
                            Bird* b = birdMap->at(thisYPos).at(thisXPos);
                            numBirdsInZone = 0;
                            numAllBirdsInZones += birdMapCount->at(thisYPos).at(thisXPos);
                            while (b != nullptr && numBirdsInZone < birdMapCount->at(thisYPos).at(thisXPos) * p->processRatio)
                            {
                                if (b->id != id)
                                {
                                    const float distance = getDistance(b->xPos, b->yPos);

                                    // If the distance is less than the birds vision, and the other bird is in front of the current bird apply avoidance and direction matching rule
                                    if (distance < vision)
                                    {
                                        // This dot product is used to determine if the other bird is in front of the current bird
                                        const float d = dot(b->xPos, xPos, b->yPos, yPos);
                                        if (d > 0.0f)
                                        {
                                            const float xDiff = (b->xPos - xPos);
                                            const float yDiff = (b->yPos - yPos);
                                            const float distXVec = xDiff / distance;
                                            const float distYVec = yDiff / distance;

                                            // Change velocity to avoid the other bird
                                            accumulatedXVelocity -= distXVec * avoidBirdVal * (vision - distance) / vision;
                                            accumulatedYVelocity -= distYVec * avoidBirdVal * (vision - distance) / vision;

                                            // Change velocity to match the other birds velocity
                                            accumulatedXVelocity += b->xVelocity * matchBirdVal * (vision - distance) / vision;
                                            accumulatedYVelocity += b->yVelocity * matchBirdVal * (vision - distance) / vision;


                                            // Pressing c turns on pretty colors!
                                            if (p->colorsOn)
                                            {
                                                const float birdCorolation = (vision - distance) / vision;

                                                const float hue = color.getHue();
                                                const float otherHue = b->color.getHue();
                                                const float dist = minDistWrap(hue, otherHue);
                                                float newHue = fmodf(hue + dist * birdCorolation * 0.05f, 1.0f);
                                                if (newHue < 0.0f)
                                                {
                                                    newHue += 1.0f;
                                                }
                                                else if (newHue >= 1.0f)
                                                {
                                                    newHue -= 1.0f;
                                                }

                                                color = juce::Colour(newHue, 1.0f, 1.0f, 1.0f);
                                            }

                                            // Collect the average position for group rule later
                                            averageX += b->xPos;
                                            averageY += b->yPos;
                                            numBirdsInView++;
                                        }
                                    }
                                }
                                numBirdsInZone++;
                                b = b->next;
                            }

                            if (p->im.isValid() && p->imagePoints.size() == birdMap->size() && p->imagePoints[0].size() == birdMap->at(0).size())
                            {
                                for (int i = 0; i < p->imagePoints[yMapIndex + yInd][xMapIndex + xInd].size(); i++)
                                {
                                    const float imgY = p->imagePoints[yMapIndex + yInd][xMapIndex + xInd][i].first[0];
                                    const float imgX = p->imagePoints[yMapIndex + yInd][xMapIndex + xInd][i].first[1];
                                    const juce::Colour imgC = p->imagePoints[yMapIndex + yInd][xMapIndex + xInd][i].second;

                                    const float imgDist = getDistance(imgX, imgY);

                                    color = imgC;

                                    accumulatedXVelocity += (imgY - yPos) * imgDist * p->imageSlide;
                                    accumulatedYVelocity += (imgX - xPos) * imgDist * p->imageSlide;

                                    accumulatedXVelocity += (imgX - xPos) * imgDist * p->imagePull;
                                    accumulatedYVelocity += (imgY - yPos) * imgDist * p->imagePull;

                                }
                            }
                        }
                    }
                }
            }
            if (numBirdsInView > 0)
            {
                // Get average position
                averageX /= (numBirdsInView);
                averageY /= (numBirdsInView);

                const float distance = getDistance(averageX, averageY);
                const float xDiff = (averageX - xPos);
                const float yDiff = (averageY - yPos);
                const float distXVec = xDiff / distance;
                const float distYVec = yDiff / distance;


                // Shift velocity towards the average position
                accumulatedXVelocity += distXVec * groupBirdVal * (vision - distance) / vision;
                accumulatedYVelocity += distYVec * groupBirdVal * (vision - distance) / vision;

                xVelocity += accumulatedXVelocity * ((1.0f - p->gridCompensation) + (float(numAllBirdsInZones) / numBirdsInView) * p->gridCompensation);
                yVelocity += accumulatedYVelocity * ((1.0f - p->gridCompensation) + (float(numAllBirdsInZones) / numBirdsInView) * p->gridCompensation);
            }
        }

        // Here we implement mouse interaction. A mouse down is similar to thr group rule except it has more range and power.
        if (p->mouseDown)
        {
            const float distance = getDistance(p->mouseX, p->mouseY);
            if (distance < (vision * 6) && distance > 0.0f)
            {
                const float xDiff = (p->mouseX - xPos);
                const float yDiff = (p->mouseY - yPos);
                const float distXVec = xDiff / distance;
                const float distYVec = yDiff / distance;

                // Mouse interaction is always on, so the power starts at 0.5 and scales with the group slider weight
                xVelocity += distXVec * (3.0f + p->moddedGroup) * ((vision * 6) - distance) / (vision * 6);
                yVelocity += distYVec * (3.0f + p->moddedGroup) * ((vision * 6) - distance) / (vision * 6);

                if (p->colorsOn)
                {
                    color = color.withRotatedHue(((vision * 2) - distance) / (vision * 6) * 0.03f);
                }
            }
        }
        // Finally we normalize the velocity to the value determined by the speed slider
        float newVel = p->moddedVelocity;
        setVelocity(abs(newVel));
        float birdXVel = xVelocity;
        float birdYVel = yVelocity;
        if (newVel < 0.0f)
        {
            birdXVel *= -1;
            birdYVel *= -1;
        }

        if (p->colorsOn)
        {
            const float velocityCorolation = dot(birdXVel, birdYVel, (xPos - 0.5f) * 2, (yPos - 0.5f) * 2);
            color = color.withRotatedHue(velocityCorolation * 0.4f);
            color = color.withRotatedHue(p->moddedColor * 0.1f);
        }

        xVelocitySaved = xVelocity;
        yVelocitySaved = yVelocity;
    }

    void updatePosition()
    {
        // Update the bird position
        const float birdXVel = xVelocitySaved;
        const float birdYVel = yVelocitySaved;
        xPos += birdXVel;
        yPos += birdYVel;


        // If the bird has gone out of bounds, teleport to opposite side of screen

        if (xPos >= p->widthRatio)
        {
            if (p->tpToZero)
            {
                xPos = 0.5f - birdXVel;
                yPos = 0.5f - birdYVel;
            }
            else
            {
                xPos -= p->widthRatio;
            }
        }
        else if (xPos < 0.0f)
        {
            if (p->tpToZero)
            {
                xPos = 0.5f - birdXVel;
                yPos = 0.5f - birdYVel;
            }
            else
            {
                xPos += p->widthRatio;
            }
        }

        if (yPos >= p->heightRatio)
        {
            if (p->tpToZero)
            {
                xPos = 0.5f - birdXVel;
                yPos = 0.5f - birdYVel;
            }
            else
            {
                yPos -= p->heightRatio;
            }
        }
        else if (yPos < 0.0f)
        {
            if (p->tpToZero)
            {
                xPos = 0.5f - birdXVel;
                yPos = 0.5f - birdYVel;
            }
            else
            {
                yPos += p->heightRatio;
            }
        }

        // Add this bird to new birdmap
        const int newXMap = int(xPos * p->xNumSpots);
        const int newYMap = int(yPos * p->yNumSpots);

        if (newXMap != xMapIndex || newYMap != yMapIndex)
        {
            if (newXMap > -1 && newXMap < p->xNumSpots && newYMap > -1 && newYMap < p->yNumSpots)
            {
                disconnect();
                xMapIndex = newXMap;
                yMapIndex = newYMap;
                reconnect();
            }
        }
    }

    void paintBird(juce::Graphics& g, const int x, const int y, const int width, const int height)
    {

        const float xRatio = xPos / p->widthRatio;
        const float yRatio = yPos / p->heightRatio;

        if (xRatio > 0.0f && xRatio < 1.0f && yRatio > 0.0f && yRatio < 1.0f)
        {
            g.setColour(color);
            //g.drawLine(noseX * width, noseY * height, tailX * width, tailY * height, 1.2f);
            //g.drawEllipse((xPos - p->birdSize * 0.5f) * width, (yPos - p->birdSize * 0.5f) * width, p->birdSize * width, p->birdSize * width, 2.0f);
            g.drawRect((xPos - p->birdSize * 0.5f) * width, (yPos - p->birdSize * 0.5f) * width, p->birdSize * width, p->birdSize * width, 2.0f);
        }
    }

    float xPos = 0.0;
    float yPos = 0.0;

    float xVelocity = 0.0;
    float yVelocity = 0.0;

    juce::Colour color;

    Bird* next;
    Bird* prev;

    int xMapIndex = -1;
    int yMapIndex = -1;

private:

    void setRandomPosition()
    {
        xPos = float(rand()) / RAND_MAX;
        yPos = float(rand()) / RAND_MAX;
    }

    float getNorm(const float x, const float y)
    {
        return p->getFastSqrt(x * x + y * y);
    }

    void setVelocity(const float newVelocity)
    {
        const float norm = getNorm(xVelocity, yVelocity);
        xVelocity *= (newVelocity / norm);
        yVelocity *= (newVelocity / norm);
    }

    float getDistance(const float x, const float y)
    {
        return p->getFastSqrt((x - xPos) * (x - xPos) + (y - yPos) * (y - yPos));
    }

    float minDistWrap(const float a, const float b)
    {
        if (a > b)
        {
            if ((a - b) > 0.5f)
            {
                return b + 1 - a;
            }
            else
            {
                return -a + b;
            }
        }
        else
        {
            if ((b - a) > 0.5f)
            {
                return -a - 1 + b;
            }
            else
            {
                return b - a;
            }
        }
    }

    float dot(const float x1, const float y1, const float x2, const float y2)
    {
        return x1 * x2 + y1 * y2;
    }


    void rotate(float& x, float& y, const float rad)
    {
        const float newX = x * cos(rad * piTwo) + y * sin(rad * piTwo);
        const float newY = y * cos(rad * piTwo) - x * sin(rad * piTwo);

        x = newX;
        y = newY;
    }

    int id = 0;

    const float piTwo = 3.14159265359f * 2.0f;
    std::vector<Bird>* birds = nullptr;
    std::vector<std::vector<Bird*>>* birdMap = nullptr;
    std::vector<std::vector<int>>* birdMapCount = nullptr;
    float xVelocitySaved = 0.0f;
    float yVelocitySaved = 0.0f;
    parameters* p;
};

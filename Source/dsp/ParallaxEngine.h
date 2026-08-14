#pragma once

#include <JuceHeader.h>

class ParallaxEngine
{
public:
    void prepare (double sampleRate, int maximumBlockSize);
    void reset();

    void setOffsetMs (float milliseconds);
    void setDelayedSide (bool delayRight);
    void setSpread (float amount);
    void setWow (float amount);

    void process (juce::AudioBuffer<float>& buffer);
};

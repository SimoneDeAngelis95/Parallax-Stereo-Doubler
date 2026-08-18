#pragma once
#include <JuceHeader.h>

class WowModulator
{
public:
    void prepare(double sampleRate, float maximumDepthMs);
    void reset();

    void setAmount(float normalizedAmount);
    float getNextOffsetSamples();

private:
    static constexpr float rateHz = 0.35f;

    double sampleRate = 44100.0;

    float phase = 0.0f;
    float phaseIncrement = 0.0f;
    float maximumDepthSamples = 0.0f;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedAmount;
};

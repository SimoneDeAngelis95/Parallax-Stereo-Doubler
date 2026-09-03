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
    static constexpr float wowRateHz = 0.45f;
    static constexpr float driftRateHz = 1.13f;
    static constexpr float flutterRateHz = 5.40f;

    static constexpr float wowWeight = 0.55f;
    static constexpr float driftWeight = 0.25f;
    static constexpr float flutterWeight = 0.20f;

    double sampleRate = 44100.0;

    float wowPhase = 0.0f;
    float driftPhase = 0.0f;
    float flutterPhase = 0.0f;

    float wowPhaseIncrement = 0.0f;
    float driftPhaseIncrement = 0.0f;
    float flutterPhaseIncrement = 0.0f;

    float maximumDepthSamples = 0.0f;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedAmount;
};

#pragma once
#include <JuceHeader.h>
#include "WowModulator.h"

class ParallaxEngine
{
public:
    void prepare(double samplerate, bool monoIn, float initialOffsetMs, float initialSpreadPercent);
    void reset();
    
    void setOffsetMs(float milliseconds);             // given ms convert to samples
    void setDelayedSide(bool side);
    void setSpread(float amount);
    void setWow(float amount);
    
    static double getTailLengthSeconds();
    
    void process(juce::AudioBuffer<float>& buffer);
    
private:
    float readInterpolatedSample (int channel, int newerPosition, int olderPosition, float fraction) const noexcept; // function for interpolate samples
    void applySpread (float& left, float& right, float amount) const noexcept;
    
    static constexpr float maximumOffsetMs = 50.0f;
    static constexpr float maximumWowDepthMs = 10.0f;
    
    double sampleRate = 44100.0;
    
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedDelaySamples;  // value of the delay in sample but smoothed
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedSpread;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedSide;          // for smoothing delayed side changes
    
    bool delaySide = true;                                                               // false = left side, true = right side
    WowModulator wowModulator;
    
    juce::AudioBuffer<float> delayBuffer;
    int delayBufferSize = 0;
    int writePosition = 0;
    
    bool isMonoIn = true;
};

#pragma once
#include <JuceHeader.h>

#define MAX_DELAY_MS 50.0

class ParallaxEngine
{
public:
    void prepare(double samplerate);
    void reset();
    
    void setOffsetMs(float milliseconds);             // given ms convert to samples
    void setDelayedSide(bool side);
    void setSpread(float amount);
    void setWow(float amount);
    
    void process(juce::AudioBuffer<float>& buffer);
    
private:
    double sampleRate = 44100.0;
    
    int delaySamples = 0;
    bool delaySide = true;                          // false = left side, true = right side
    float spreadAmount = 1.0f;
    float wowAmount = 0.0f;
    
    juce::AudioBuffer<float> delayBuffer;
    int delayBufferSize = 0;
    int writePosition = 0;
};

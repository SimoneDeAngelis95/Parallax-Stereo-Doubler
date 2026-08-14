#include "ParallaxEngine.h"
#include <cmath>

void ParallaxEngine::prepare(double samplerate)
{
    this->sampleRate = samplerate;
    delayBufferSize = static_cast<int> (std::ceil(sampleRate * MAX_DELAY_MS / 1000.0)) + 1;
    delayBuffer.setSize (2, delayBufferSize);
    reset();
}

void ParallaxEngine::reset()
{
    delayBuffer.clear();
    writePosition = 0;
}

void ParallaxEngine::setOffsetMs(float milliseconds)
{
    delaySamples = static_cast<int>(std::round (milliseconds * sampleRate / 1000.0));
}

void ParallaxEngine::setDelayedSide(bool side)
{
    delaySide = side;
}

void ParallaxEngine::setSpread(float amount)
{
    spreadAmount = amount;
}

void ParallaxEngine::setWow(float amount)
{
    wowAmount = amount;
}

void ParallaxEngine::process(juce::AudioBuffer<float>& buffer)
{
    
}

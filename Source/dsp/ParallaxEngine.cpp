#include "ParallaxEngine.h"
#include <cmath>

void ParallaxEngine::prepare(double samplerate)
{
    this->sampleRate = samplerate;
    delayBufferSize = static_cast<int> (std::ceil(sampleRate * MAX_DELAY_MS / 1000.0)) + 1; // set the maximum bufferSize in sample
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
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
        for(int channel = 0; channel < buffer.getNumChannels(); channel++)
            delayBuffer.setSample(channel, writePosition, buffer.getSample(channel, sampleIndex));
     
        const int readPosition = (writePosition - delaySamples + delayBufferSize) % delayBufferSize; // Wrap the delayed read position into the valid buffer range
        buffer.setSample(delaySide, sampleIndex, delayBuffer.getSample(delaySide, readPosition));

        writePosition = ++writePosition % delayBufferSize;
    }
}

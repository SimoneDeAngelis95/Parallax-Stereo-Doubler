#include "ParallaxEngine.h"
#include <cmath>

void ParallaxEngine::prepare(double samplerate, bool monoIn, float initialOffsetMs, float initialSpreadPercent)
{
    this->sampleRate = samplerate;

    // CALCULATE THE SIZE OF THE DELAY BUFFER
    const float maximumDelayMs = maximumOffsetMs + maximumWowDepthMs;
    delayBufferSize = static_cast<int>(std::ceil(sampleRate * maximumDelayMs / 1000.0)) + 1;
    delayBuffer.setSize (2, delayBufferSize);
    
    this->isMonoIn = monoIn;

    // SMOOTH SIDE CHANGE
    smoothedSide.reset(sampleRate, 0.005); // 5ms
    
    // SMOOTH OFFSET DELAY CHANGE
    smoothedDelaySamples.reset(sampleRate, 0.020); // 20ms
    const float initialDelaySamples = initialOffsetMs * static_cast<float>(sampleRate) / 1000.0f;
    smoothedDelaySamples.setCurrentAndTargetValue(initialDelaySamples);
    
    // SMOOTH SPREAD CHANGE
    smoothedSpread.reset (sampleRate, 0.020);
    const float initialSpread = initialSpreadPercent / 100.0f;
    smoothedSpread.setCurrentAndTargetValue(initialSpread);
    
    wowModulator.prepare (sampleRate, maximumWowDepthMs);
    
    reset();
}

void ParallaxEngine::reset()
{
    delayBuffer.clear();
    writePosition = 0;
    
    smoothedSide.setCurrentAndTargetValue(delaySide ? 1.0f : 0.0f); // don't call .reset() because we already set it in prepare
    smoothedDelaySamples.setCurrentAndTargetValue(smoothedDelaySamples.getTargetValue());
    smoothedSpread.setCurrentAndTargetValue(smoothedSpread.getTargetValue());
    
    wowModulator.reset();
}

void ParallaxEngine::setOffsetMs(float milliseconds)
{
    const float targetDelaySamples = milliseconds * static_cast<float>(sampleRate) / 1000.0f;
    smoothedDelaySamples.setTargetValue(targetDelaySamples);
}

void ParallaxEngine::setDelayedSide(bool side)
{
    delaySide = side;
    smoothedSide.setTargetValue(delaySide ? 1.0f : 0.0f);
}

void ParallaxEngine::setSpread(float amount)
{
    const float normalizedSpread = amount / 100.0f;

    smoothedSpread.setTargetValue(normalizedSpread);
}

void ParallaxEngine::setWow(float amount)
{
    wowModulator.setAmount (amount / 100.0f);
}

void ParallaxEngine::process(juce::AudioBuffer<float>& buffer)
{
    // =========== SECURITY ===========
    jassert (buffer.getNumChannels() >= 2);    // at least two outputs
    jassert (delayBufferSize > 0);             // make sure that prepare() function was called
    
    if (buffer.getNumChannels() < 2 || delayBufferSize <= 0)
        return;
    // ================================
    
    const int numSamples = buffer.getNumSamples();
    
    
    for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
                                                                                                     // GET INPUT
        const float inputLeft = buffer.getSample (0, sampleIndex);
        const float inputRight = isMonoIn ? inputLeft : buffer.getSample(1, sampleIndex);            // Duplicate the mono input into the logical right channel
        
        delayBuffer.setSample (0, writePosition, inputLeft);                                         // set delay buffer
        delayBuffer.setSample (1, writePosition, inputRight);
        
        // SMOOTHING DELAY TIME AND WOW
        const float baseDelaySamples = smoothedDelaySamples.getNextValue();
        const float wowOffsetSamples = wowModulator.getNextOffsetSamples();
        const float currentDelaySamples = baseDelaySamples + wowOffsetSamples;
        const int delayFloor = static_cast<int> (std::floor (currentDelaySamples));                  // get the integer part of it
        const float fraction = currentDelaySamples - static_cast<float> (delayFloor);                // get the decimal part of it

                                                                                                     // Locate the two samples surrounding the fractional position.
        const int newerPosition = (writePosition - delayFloor + delayBufferSize) % delayBufferSize;  // 0 <= newerPosition < delayBufferSize
        const int olderPosition = (newerPosition - 1 + delayBufferSize) % delayBufferSize;
        
        const float delayedLeft = readInterpolatedSample (0, newerPosition, olderPosition, fraction);
        const float delayedRight = readInterpolatedSample (1, newerPosition, olderPosition, fraction);
        
        
        const float sideMix = smoothedSide.getNextValue();                                           // Advance the crossfade between the L-delayed n R-delayed states

        float outputLeft = delayedLeft * (1.0f - sideMix) + inputLeft * sideMix;                     // Crossfade each channel between its direct and delayed versions
        float outputRight = inputRight * (1.0f - sideMix) + delayedRight * sideMix;                  // to avoid clicks when the delayed side changes
        
        // SPREAD
        const float currentSpread = smoothedSpread.getNextValue();
        applySpread(outputLeft, outputRight, currentSpread);

        // OUTPUT
        buffer.setSample (0, sampleIndex, outputLeft);
        buffer.setSample (1, sampleIndex, outputRight);
        
        writePosition = ++writePosition % delayBufferSize;                                           // increase write index
    }
}

float ParallaxEngine::readInterpolatedSample(int channel, int newerPosition, int olderPosition, float fraction) const noexcept
{
    const float newerSample = delayBuffer.getSample (channel, newerPosition);
    const float olderSample = delayBuffer.getSample (channel, olderPosition);

    return newerSample * (1.0f - fraction) + olderSample * fraction;
}

void ParallaxEngine::applySpread (float& left, float& right, float amount) const noexcept
{
                                                // * 0.5 to compensate volume increase
    const float mid = (left + right) * 0.5f;    // L + R = (Mid/2 + Side/2) + (Mid/2 − Side/2) = Mid
    const float side = (left - right) * 0.5f;   // L - R = (Mid/2 + Side/2) - (Mid/2 − Side/2) = Side

    left = mid + side * amount;                 // mid + side = (left/2 - right/2) + (left/2 -right/2) = left
    right = mid - side * amount;                // mid - side = (left/2 - right/2) - (left/2 -right/2) = right
}

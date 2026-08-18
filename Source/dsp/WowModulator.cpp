#include "WowModulator.h"

void WowModulator::prepare (double newSampleRate, float maximumDepthMs)
{
    this->sampleRate = newSampleRate;

    maximumDepthSamples = maximumDepthMs * static_cast<float>(sampleRate) / 1000.0f;

    phaseIncrement = juce::MathConstants<float>::twoPi * rateHz / static_cast<float>(sampleRate);

    smoothedAmount.reset(sampleRate, 0.02);
    smoothedAmount.setCurrentAndTargetValue(0.0f);

    reset();
}

void WowModulator::reset()
{
    phase = -juce::MathConstants<float>::halfPi;

    smoothedAmount.setCurrentAndTargetValue(smoothedAmount.getTargetValue());
}

void WowModulator::setAmount (float amountZeroToOne)
{
    const auto clampedAmount = juce::jlimit (0.0f, 1.0f, amountZeroToOne);
    smoothedAmount.setTargetValue(clampedAmount);
}

float WowModulator::getNextOffsetSamples()
{
    const auto amount = smoothedAmount.getNextValue();

    const auto sineValue = std::sin(phase);

    // convert the sinusoid from -1...+1 to 0...1.
    const auto unipolarValue = (sineValue + 1.0f) * 0.5f;

    const auto offsetSamples = unipolarValue * maximumDepthSamples * amount;

    phase += phaseIncrement;

    if (phase >= juce::MathConstants<float>::pi * 1.5f)
        phase -= juce::MathConstants<float>::twoPi;

    return offsetSamples;
}

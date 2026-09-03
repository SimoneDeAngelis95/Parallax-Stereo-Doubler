#include "WowModulator.h"
#include <cmath>

void WowModulator::prepare (double newSampleRate, float maximumDepthMs)
{
    this->sampleRate = newSampleRate;

    maximumDepthSamples = maximumDepthMs * static_cast<float>(sampleRate) / 1000.0f;

    const auto phaseIncrementPerHz = juce::MathConstants<float>::twoPi / static_cast<float> (sampleRate);
    
    wowPhaseIncrement = phaseIncrementPerHz * wowRateHz;
    driftPhaseIncrement = phaseIncrementPerHz * driftRateHz;
    flutterPhaseIncrement = phaseIncrementPerHz * flutterRateHz;

    smoothedAmount.reset(sampleRate, 0.02);
    smoothedAmount.setCurrentAndTargetValue(0.0f);

    reset();
}

void WowModulator::reset()
{
    const auto startingPhase = -juce::MathConstants<float>::halfPi;

    wowPhase = startingPhase;
    driftPhase = startingPhase;
    flutterPhase = startingPhase;

    smoothedAmount.setCurrentAndTargetValue(smoothedAmount.getTargetValue()
    );
}

void WowModulator::setAmount (float amountZeroToOne)
{
    const auto clampedAmount = juce::jlimit (0.0f, 1.0f, amountZeroToOne);
    smoothedAmount.setTargetValue(clampedAmount);
}

float WowModulator::getNextOffsetSamples()
{
    const auto amount = smoothedAmount.getNextValue();

    const auto combinedWave =
          std::sin (wowPhase)     * wowWeight
        + std::sin (driftPhase)   * driftWeight
        + std::sin (flutterPhase) * flutterWeight;

    // Convert the combined waveform from -1...+1 to 0...1.
    const auto unipolarValue = (combinedWave + 1.0f) * 0.5f;

    const auto offsetSamples = unipolarValue * maximumDepthSamples * amount;

    wowPhase += wowPhaseIncrement;
    driftPhase += driftPhaseIncrement;
    flutterPhase += flutterPhaseIncrement;

    const auto endOfCycle = juce::MathConstants<float>::pi * 1.5f; // = 3/2π

    if (wowPhase >= endOfCycle)
        wowPhase -= juce::MathConstants<float>::twoPi;

    if (driftPhase >= endOfCycle)
        driftPhase -= juce::MathConstants<float>::twoPi;

    if (flutterPhase >= endOfCycle)
        flutterPhase -= juce::MathConstants<float>::twoPi;

    return offsetSamples;
}

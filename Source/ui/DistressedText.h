#pragma once

#include <JuceHeader.h>

namespace DistressedText
{
void draw (juce::Graphics& graphics,
           const juce::String& text,
           juce::Rectangle<float> bounds,
           const juce::Font& font,
           juce::Colour textColour,
           float distressAmount,
           int seed,
           juce::Justification justification = juce::Justification::centred);
}

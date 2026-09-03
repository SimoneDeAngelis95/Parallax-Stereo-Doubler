#include "ParallaxFonts.h"

#include <BinaryData.h>

namespace
{
const auto bebas = juce::Typeface::createSystemTypefaceFor (
    BinaryData::BebasNeueRegular_ttf, BinaryData::BebasNeueRegular_ttfSize);
const auto cormorant = juce::Typeface::createSystemTypefaceFor (
    BinaryData::CormorantGaramondVariable_ttf,
    BinaryData::CormorantGaramondVariable_ttfSize);
const auto dseg = juce::Typeface::createSystemTypefaceFor (
    BinaryData::DSEG7ClassicRegular_ttf, BinaryData::DSEG7ClassicRegular_ttfSize);
}

juce::Font ParallaxFonts::displaySans (float height)
{
    return juce::Font { juce::FontOptions { bebas }.withHeight (height) };
}

juce::Font ParallaxFonts::editorialSerif (float height)
{
    return juce::Font { juce::FontOptions { cormorant }.withHeight (height) };
}

juce::Font ParallaxFonts::sevenSegment (float height)
{
    return juce::Font { juce::FontOptions { dseg }.withHeight (height) };
}

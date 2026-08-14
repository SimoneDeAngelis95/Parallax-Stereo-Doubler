#include "FilmstripKnob.h"

FilmstripKnob::FilmstripKnob (const void* imageData, int imageDataSize, int numberOfFrames)
    : filmstrip (juce::ImageCache::getFromMemory (imageData, imageDataSize)),
      frames (numberOfFrames)
{
    setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
                         juce::MathConstants<float>::pi * 2.75f, true);
}

void FilmstripKnob::paint (juce::Graphics& g)
{
    if (! filmstrip.isValid() || frames <= 0)
        return;

    const auto range = juce::jmax (0.0001, getMaximum() - getMinimum());
    const auto proportion = juce::jlimit (0.0, 1.0, (getValue() - getMinimum()) / range);
    const auto frame = juce::jlimit (0, frames - 1,
        juce::roundToInt (proportion * static_cast<double> (frames - 1)));
    const auto sourceHeight = filmstrip.getHeight() / frames;

    g.drawImage (filmstrip,
                 0, 0, getWidth(), getHeight(),
                 0, frame * sourceHeight, filmstrip.getWidth(), sourceHeight);
}

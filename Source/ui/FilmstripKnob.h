#pragma once

#include <JuceHeader.h>

class FilmstripKnob final : public juce::Slider
{
public:
    FilmstripKnob (const void* imageData, int imageDataSize, int numberOfFrames);
    void paint (juce::Graphics&) override;

private:
    juce::Image filmstrip;
    int frames = 1;
};

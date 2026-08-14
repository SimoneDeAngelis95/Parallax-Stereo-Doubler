#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ui/DelayedSideSwitch.h"
#include "ui/FilmstripKnob.h"

class ParallaxAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit ParallaxAudioProcessorEditor (ParallaxAudioProcessor&);
    ~ParallaxAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = ParallaxAudioProcessor::APVTS::SliderAttachment;
    using ButtonAttachment = ParallaxAudioProcessor::APVTS::ButtonAttachment;

    void drawScale (juce::Graphics&, juce::Rectangle<float>, int tickCount,
                    float highlightedFrom, juce::Colour highlightColour) const;

    juce::Image panelBackground;

    FilmstripKnob offsetSlider;
    FilmstripKnob spreadSlider;
    FilmstripKnob wowSlider;
    DelayedSideSwitch delayedSideSwitch;

    SliderAttachment offsetAttachment;
    SliderAttachment spreadAttachment;
    SliderAttachment wowAttachment;
    ButtonAttachment sideAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParallaxAudioProcessorEditor)
};

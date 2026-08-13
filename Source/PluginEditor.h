#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ParallaxAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit ParallaxAudioProcessorEditor (ParallaxAudioProcessor&);
    ~ParallaxAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Label titleLabel;
    juce::Label taglineLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParallaxAudioProcessorEditor)
};

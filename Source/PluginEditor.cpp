#include "PluginEditor.h"

ParallaxAudioProcessorEditor::ParallaxAudioProcessorEditor (ParallaxAudioProcessor& p)
    : AudioProcessorEditor (&p)
{
    titleLabel.setText ("PARALLAX", juce::dontSendNotification);
    titleLabel.setJustificationType (juce::Justification::centred);
    titleLabel.setFont (juce::FontOptions { 32.0f, juce::Font::bold });
    addAndMakeVisible (titleLabel);

    taglineLabel.setText ("Two perspectives. One signal.", juce::dontSendNotification);
    taglineLabel.setJustificationType (juce::Justification::centred);
    taglineLabel.setColour (juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible (taglineLabel);

    setSize (520, 260);
}

void ParallaxAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromRGB (20, 22, 26));
}

void ParallaxAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (32);
    titleLabel.setBounds (area.removeFromTop (64));
    taglineLabel.setBounds (area.removeFromTop (32));
}

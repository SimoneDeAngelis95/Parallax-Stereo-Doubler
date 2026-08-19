#pragma once

#include <JuceHeader.h>

class InfoButton final : public juce::Button
{
public:
    InfoButton();

    void paintButton (juce::Graphics&, bool isMouseOver, bool isButtonDown) override;
};

class InfoPanel final : public juce::Component
{
public:
    InfoPanel();

    void paint (juce::Graphics&) override;
    void mouseDown (const juce::MouseEvent&) override;
    bool keyPressed (const juce::KeyPress&) override;

    std::function<void()> onClose;

private:
    juce::Rectangle<float> getPanelBounds() const;
    juce::Rectangle<float> getCloseBounds() const;
    void close();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfoPanel)
};

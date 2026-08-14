#pragma once

#include <JuceHeader.h>

class DelayedSideSwitch final : public juce::Button
{
public:
    DelayedSideSwitch();
    void paintButton (juce::Graphics&, bool isMouseOver, bool isButtonDown) override;

private:
    juce::Image toggleLeft;
    juce::Image toggleRight;
    juce::Image ledOff;
    juce::Image ledOn;
};

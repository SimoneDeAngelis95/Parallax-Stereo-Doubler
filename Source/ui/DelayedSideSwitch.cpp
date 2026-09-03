#include "DelayedSideSwitch.h"
#include "ParallaxFonts.h"
#include "ParallaxStyle.h"

#include <BinaryData.h>

DelayedSideSwitch::DelayedSideSwitch()
    : Button ("Delayed Side"),
      toggleLeft (juce::ImageCache::getFromMemory (
          BinaryData::toggleleft_png, BinaryData::toggleleft_pngSize)),
      toggleRight (juce::ImageCache::getFromMemory (
          BinaryData::toggleright_png, BinaryData::toggleright_pngSize)),
      ledOff (juce::ImageCache::getFromMemory (
          BinaryData::ledoff_png, BinaryData::ledoff_pngSize)),
      ledOn (juce::ImageCache::getFromMemory (
          BinaryData::ledon_png, BinaryData::ledon_pngSize))
{
    setClickingTogglesState (true);
    setTitle ("Delayed Side");
    setDescription ("Chooses whether the left or right channel receives the time offset");
}

void DelayedSideSwitch::paintButton (juce::Graphics& g, bool isMouseOver, bool isButtonDown)
{
    const auto rightSelected = getToggleState();
    constexpr auto referenceWidth = 170.0f;
    constexpr auto referenceHeight = 170.0f;
    const auto scaleX = static_cast<float> (getWidth()) / referenceWidth;
    const auto scaleY = static_cast<float> (getHeight()) / referenceHeight;
    g.addTransform (juce::AffineTransform::scale (scaleX, scaleY));

    const auto bounds = juce::Rectangle<float> { 0.0f, 0.0f,
                                                  referenceWidth, referenceHeight };

    g.setColour (ParallaxStyle::ivory);
    g.setFont (ParallaxFonts::displaySans (25.0f));
    g.drawText ("L", 15, 23, 28, 30, juce::Justification::centred);
    g.drawText ("R", 127, 23, 28, 30, juce::Justification::centred);

    constexpr auto ledSize = 27.0f;
    const auto leftLed = juce::Rectangle<float> { 15.0f, 54.0f, ledSize, ledSize };
    const auto rightLed = juce::Rectangle<float> { bounds.getRight() - 42.0f, 54.0f,
                                                   ledSize, ledSize };
    const auto drawLed = [&g] (const juce::Image& image, juce::Rectangle<float> area)
    {
        g.drawImageWithin (image,
                           juce::roundToInt (area.getX()), juce::roundToInt (area.getY()),
                           juce::roundToInt (area.getWidth()), juce::roundToInt (area.getHeight()),
                           juce::RectanglePlacement::centred);
    };

    drawLed (rightSelected ? ledOff : ledOn, leftLed);
    drawLed (rightSelected ? ledOn : ledOff, rightLed);

    auto switchArea = juce::Rectangle<float> {
        bounds.getCentreX() - 61.0f, 18.0f, 122.0f, 122.0f
    };
    switchArea.translate (rightSelected ? -6.0f : 6.0f, 0.0f);
    const auto& image = rightSelected ? toggleRight : toggleLeft;
    g.setOpacity (isButtonDown ? 0.88f : (isMouseOver ? 1.0f : 0.96f));
    g.drawImage (image, switchArea, juce::RectanglePlacement::centred);
    g.setOpacity (1.0f);
}

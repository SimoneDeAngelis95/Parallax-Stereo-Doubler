#include "PluginEditor.h"
#include "ui/DistressedText.h"
#include "ui/ParallaxFonts.h"
#include "ui/ParallaxStyle.h"
#include <BinaryData.h>

namespace
{
juce::Image loadImage (const void* data, int size)
{
    return juce::ImageCache::getFromMemory (data, size);
}
}

ParallaxAudioProcessorEditor::ParallaxAudioProcessorEditor (ParallaxAudioProcessor& p)
    : AudioProcessorEditor (&p),
      panelBackground (loadImage (BinaryData::panelbackground_png, BinaryData::panelbackground_pngSize)),
      offsetSlider (BinaryData::knoboffsetfilmstrip_png, BinaryData::knoboffsetfilmstrip_pngSize, 128),
      spreadSlider (BinaryData::knobsmallfilmstrip_png, BinaryData::knobsmallfilmstrip_pngSize, 101),
      wowSlider (BinaryData::knobsmallfilmstrip_png, BinaryData::knobsmallfilmstrip_pngSize, 101),
      offsetAttachment (p.getParameters(), "offset", offsetSlider),
      spreadAttachment (p.getParameters(), "spread", spreadSlider),
      wowAttachment (p.getParameters(), "wow", wowSlider),
      sideAttachment (p.getParameters(), "delayedSide", delayedSideSwitch)
{
    offsetSlider.setRange (0.0, 50.0, 1.0);
    spreadSlider.setRange (0.0, 200.0, 1.0);
    wowSlider.setRange (0.0, 100.0, 1.0);

    offsetSlider.setDoubleClickReturnValue (true, 20.0);
    spreadSlider.setDoubleClickReturnValue (true, 100.0);
    wowSlider.setDoubleClickReturnValue (true, 0.0);

    offsetSlider.setTitle ("Offset");
    offsetSlider.setTextValueSuffix (" ms");
    spreadSlider.setTitle ("Spread");
    spreadSlider.setTextValueSuffix (" %");
    wowSlider.setTitle ("Wow");
    wowSlider.setTextValueSuffix (" %");

    offsetSlider.onValueChange = [this] { repaint(); };
    spreadSlider.onValueChange = [this] { repaint(); };
    wowSlider.onValueChange = [this] { repaint(); };

    addAndMakeVisible (offsetSlider);
    addAndMakeVisible (spreadSlider);
    addAndMakeVisible (wowSlider);
    addAndMakeVisible (delayedSideSwitch);

    setResizable (true, true);
    setResizeLimits (720, 480, 1440, 960);
    getConstrainer()->setFixedAspectRatio (1.5);
    setSize (720, 480);
}

void ParallaxAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.drawImage (panelBackground, getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit);

    const auto sx = static_cast<float> (getWidth()) / 960.0f;
    const auto sy = static_cast<float> (getHeight()) / 640.0f;
    g.addTransform (juce::AffineTransform::scale (sx, sy));

    DistressedText::draw (g, "PARALLAX", { 220.0f, 42.0f, 520.0f, 104.0f },
                          ParallaxFonts::displaySans (103.0f), ParallaxStyle::ivory,
                          0.72f, 0x50415241);
    DistressedText::draw (g, "One signal. Two perspectives.",
                          { 270.0f, 139.0f, 420.0f, 38.0f },
                          ParallaxFonts::editorialSerif (35.0f), ParallaxStyle::ivory,
                          0.16f, 0x5349474e);
    DistressedText::draw (g, "SPREAD", { 84.0f, 265.0f, 184.0f, 34.0f },
                          ParallaxFonts::displaySans (31.0f), ParallaxStyle::ivory,
                          0.30f, 0x53505244);
    DistressedText::draw (g, "OFFSET", { 315.0f, 234.0f, 240.0f, 34.0f },
                          ParallaxFonts::displaySans (31.0f), ParallaxStyle::ivory,
                          0.30f, 0x4f464653);
    DistressedText::draw (g, "DELAYED SIDE", { 555.0f, 277.0f, 190.0f, 34.0f },
                          ParallaxFonts::displaySans (31.0f), ParallaxStyle::ivory,
                          0.30f, 0x44454c59);
    DistressedText::draw (g, "WOW", { 750.0f, 265.0f, 140.0f, 34.0f },
                          ParallaxFonts::displaySans (31.0f), ParallaxStyle::ivory,
                          0.30f, 0x574f5721);

    drawScale (g, { 98.0f, 333.0f, 154.0f, 154.0f }, 21, 0.525f,
               ParallaxStyle::beyondRed);
    drawScale (g, { 323.0f, 283.0f, 224.0f, 224.0f }, 26, 2.0f,
               ParallaxStyle::beyondRed);
    drawScale (g, { 748.0f, 333.0f, 144.0f, 144.0f }, 21, 2.0f,
               ParallaxStyle::beyondRed);

    g.setFont (ParallaxFonts::displaySans (24.0f));
    g.setColour (ParallaxStyle::ivory);
    g.drawText ("0", 82, 486, 42, 28, juce::Justification::centred);
    g.drawText ("100%", 142, 295, 72, 28, juce::Justification::centred);
    g.drawText ("200%", 224, 486, 72, 28, juce::Justification::centred);
    g.setColour (ParallaxStyle::beyondRed);
    g.drawText ("BEYOND", 219, 510, 82, 27, juce::Justification::centred);

    g.setColour (ParallaxStyle::ivory);
    g.drawText ("0", 306, 494, 42, 28, juce::Justification::centred);
    g.drawText ("50", 523, 494, 42, 28, juce::Justification::centred);
    g.drawText ("0", 730, 482, 42, 28, juce::Justification::centred);
    g.drawText ("100%", 850, 482, 72, 28, juce::Justification::centred);

    const auto display = juce::Rectangle<float> { 350.0f, 525.0f, 170.0f, 54.0f };
    g.setColour (ParallaxStyle::displayBackground);
    g.fillRoundedRectangle (display, 7.0f);
    g.setColour (ParallaxStyle::displayBorder);
    g.drawRoundedRectangle (display, 7.0f, 1.4f);
    g.setColour (ParallaxStyle::amber);
    g.setFont (ParallaxFonts::sevenSegment (29.0f));
    g.drawText (juce::String (juce::roundToInt (offsetSlider.getValue())),
                363, 532, 108, 39, juce::Justification::centredRight);
    g.setFont (ParallaxFonts::editorialSerif (22.0f));
    g.drawText ("ms", 478, 535, 34, 35, juce::Justification::centredLeft);
}

void ParallaxAudioProcessorEditor::drawScale (juce::Graphics& g,
                                               juce::Rectangle<float> bounds,
                                               int tickCount,
                                               float highlightedFrom,
                                               juce::Colour highlightColour) const
{
    const auto centre = bounds.getCentre();
    const auto outerRadius = bounds.getWidth() * 0.56f;
    const auto innerRadius = bounds.getWidth() * 0.49f;
    constexpr auto start = juce::MathConstants<float>::pi * 1.25f;
    constexpr auto end = juce::MathConstants<float>::pi * 2.75f;

    for (int i = 0; i < tickCount; ++i)
    {
        const auto proportion = static_cast<float> (i) / static_cast<float> (tickCount - 1);
        const auto angle = juce::jmap (proportion, start, end);
        const auto direction = juce::Point<float> { std::sin (angle), -std::cos (angle) };
        const auto major = i == 0 || i == tickCount - 1;
        const auto colour = proportion >= highlightedFrom
            ? highlightColour : ParallaxStyle::ivory;
        g.setColour (colour.withAlpha (major ? 1.0f : 0.82f));
        g.drawLine ({ centre + direction * (innerRadius - (major ? 4.0f : 0.0f)),
                      centre + direction * outerRadius }, major ? 3.0f : 1.8f);
    }
}

void ParallaxAudioProcessorEditor::resized()
{
    const auto sx = static_cast<float> (getWidth()) / 960.0f;
    const auto sy = static_cast<float> (getHeight()) / 640.0f;
    const auto scaled = [sx, sy] (juce::Rectangle<int> r)
    {
        return juce::Rectangle<int> { juce::roundToInt (static_cast<float> (r.getX()) * sx),
                                      juce::roundToInt (static_cast<float> (r.getY()) * sy),
                                      juce::roundToInt (static_cast<float> (r.getWidth()) * sx),
                                      juce::roundToInt (static_cast<float> (r.getHeight()) * sy) };
    };

    spreadSlider.setBounds (scaled ({ 100, 335, 150, 150 }));
    offsetSlider.setBounds (scaled ({ 325, 285, 220, 220 }));
    delayedSideSwitch.setBounds (scaled ({ 565, 312, 170, 170 }));
    wowSlider.setBounds (scaled ({ 750, 335, 140, 140 }));
}

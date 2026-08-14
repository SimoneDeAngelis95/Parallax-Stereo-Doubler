#include "DistressedText.h"

void DistressedText::draw (juce::Graphics& g,
                           const juce::String& text,
                           juce::Rectangle<float> bounds,
                           const juce::Font& font,
                           juce::Colour textColour,
                           float distressAmount,
                           int seed,
                           juce::Justification justification)
{
    juce::GlyphArrangement glyphs;
    glyphs.addFittedText (font, text,
                          bounds.getX(), bounds.getY(),
                          bounds.getWidth(), bounds.getHeight(),
                          justification, 1, 0.9f);

    juce::Path textPath;
    glyphs.createPath (textPath);

    g.setColour (textColour);
    g.fillPath (textPath);

    if (distressAmount <= 0.0f || textPath.isEmpty())
        return;

    juce::Graphics::ScopedSaveState savedState (g);
    g.reduceClipRegion (textPath);

    juce::Random random (seed);
    const auto area = textPath.getBounds();
    const auto speckCount = juce::roundToInt (area.getWidth() * distressAmount * 0.48f);
    const auto scratchCount = juce::roundToInt (distressAmount * 13.0f);
    const auto wearColour = juce::Colour { 0xff211c16 };

    g.setColour (wearColour.withAlpha (0.72f));

    for (int i = 0; i < speckCount; ++i)
    {
        const auto x = area.getX() + random.nextFloat() * area.getWidth();
        const auto y = area.getY() + random.nextFloat() * area.getHeight();
        const auto width = 0.7f + random.nextFloat() * (1.5f + distressAmount * 2.2f);
        const auto height = 0.5f + random.nextFloat() * (0.9f + distressAmount * 1.3f);
        g.fillEllipse (x, y, width, height);
    }

    g.setColour (wearColour.withAlpha (0.55f));

    for (int i = 0; i < scratchCount; ++i)
    {
        const auto x = area.getX() + random.nextFloat() * area.getWidth();
        const auto y = area.getY() + random.nextFloat() * area.getHeight();
        const auto length = 3.0f + random.nextFloat() * 12.0f;
        const auto slope = (random.nextFloat() - 0.5f) * 2.2f;
        g.drawLine (x, y, x + length, y + slope, 0.45f + random.nextFloat() * 0.65f);
    }
}

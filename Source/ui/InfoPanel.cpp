#include "InfoPanel.h"
#include "DistressedText.h"
#include "ParallaxFonts.h"
#include "ParallaxStyle.h"

namespace
{
constexpr float designWidth = 960.0f;
constexpr float designHeight = 640.0f;

const auto panelFill = juce::Colour { 0xff171612 };
const auto panelInner = juce::Colour { 0xff29251c };
const auto brass = juce::Colour { 0xff8f7346 };
}

InfoButton::InfoButton()
    : Button ("Info")
{
    setTooltip ("About Parallax");
    setWantsKeyboardFocus (false);
}

void InfoButton::paintButton (juce::Graphics& g,
                              bool isMouseOver,
                              bool isButtonDown)
{
    const auto bounds = getLocalBounds().toFloat().reduced (0.75f);
    const auto alpha = isButtonDown ? 0.92f : (isMouseOver ? 0.78f : 0.55f);

    g.setColour (ParallaxStyle::displayBackground.withAlpha (isMouseOver ? 0.58f : 0.34f));
    g.fillRoundedRectangle (bounds, 3.0f);
    g.setColour (ParallaxStyle::displayBorder.withAlpha (alpha));
    g.drawRoundedRectangle (bounds, 3.0f, 1.0f);
    g.setColour (ParallaxStyle::ivory.withAlpha (alpha));
    const auto buttonHeight = static_cast<float> (getHeight());
    g.setFont (ParallaxFonts::displaySans (juce::jmax (10.0f, buttonHeight * 0.72f)));
    g.drawText ("INFO", getLocalBounds(), juce::Justification::centred, false);
}

InfoPanel::InfoPanel()
{
    setOpaque (false);
    setWantsKeyboardFocus (true);
}

void InfoPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.withAlpha (0.74f));

    const auto sx = static_cast<float> (getWidth()) / designWidth;
    const auto sy = static_cast<float> (getHeight()) / designHeight;
    g.addTransform (juce::AffineTransform::scale (sx, sy));

    const auto panel = juce::Rectangle<float> { 120.0f, 82.0f, 720.0f, 476.0f };

    g.setColour (juce::Colours::black.withAlpha (0.55f));
    g.fillRoundedRectangle (panel.translated (7.0f, 9.0f), 11.0f);
    g.setColour (panelFill);
    g.fillRoundedRectangle (panel, 11.0f);
    g.setColour (brass.withAlpha (0.88f));
    g.drawRoundedRectangle (panel, 11.0f, 2.0f);
    g.setColour (panelInner);
    g.drawRoundedRectangle (panel.reduced (9.0f), 8.0f, 1.0f);

    DistressedText::draw (g, "PARALLAX", { 245.0f, 104.0f, 470.0f, 64.0f },
                          ParallaxFonts::displaySans (61.0f), ParallaxStyle::ivory,
                          0.46f, 0x494e464f);

    g.setColour (ParallaxStyle::mutedIvory);
    g.setFont (ParallaxFonts::editorialSerif (22.0f));
    g.drawText ("STEREO TEMPORAL DOUBLER", 300, 166, 360, 28,
                juce::Justification::centred);

    g.setColour (ParallaxStyle::ivory.withAlpha (0.90f));
    g.setFont (ParallaxFonts::editorialSerif (22.0f));
    g.drawFittedText ("Parallax creates two temporal perspectives of the same signal. "
                      "A short, continuously variable offset separates the channels, "
                      "building width and movement without becoming a conventional echo.",
                      { 190, 205, 580, 69 }, juce::Justification::centred, 3, 0.9f);

    g.setColour (brass.withAlpha (0.55f));
    g.drawHorizontalLine (289, 184.0f, 776.0f);

    const auto drawControl = [&g] (const juce::String& title,
                                   const juce::String& description,
                                   juce::Rectangle<int> area)
    {
        g.setColour (ParallaxStyle::ivory);
        g.setFont (ParallaxFonts::displaySans (23.0f));
        g.drawText (title, area.removeFromTop (27), juce::Justification::centredLeft);
        g.setColour (ParallaxStyle::mutedIvory.withAlpha (0.90f));
        g.setFont (ParallaxFonts::editorialSerif (18.0f));
        g.drawFittedText (description, area, juce::Justification::topLeft, 3, 0.85f);
    };

    drawControl ("OFFSET", "Sets the temporal distance between the direct and delayed perspectives (0-50 ms).",
                 { 178, 311, 270, 77 });
    drawControl ("DELAYED SIDE", "Selects which side, left or right, receives the delayed perspective.",
                 { 512, 311, 270, 77 });
    drawControl ("SPREAD", "Controls the Side component: mono at 0%, natural stereo at 100%, beyond at 200%.",
                 { 178, 404, 270, 77 });
    drawControl ("WOW", "Adds organic wow, drift and flutter to the delayed perspective.",
                 { 512, 404, 270, 77 });

    g.setColour (brass.withAlpha (0.42f));
    g.drawHorizontalLine (501, 184.0f, 776.0f);

    g.setColour (ParallaxStyle::ivory.withAlpha (0.72f));
    g.setFont (ParallaxFonts::editorialSerif (18.0f));
    g.drawText ("One signal. Two perspectives.", 190, 512, 360, 25,
                juce::Justification::centredLeft);
    g.setColour (ParallaxStyle::mutedIvory.withAlpha (0.55f));
    g.drawText ("v" + juce::String (JucePlugin_VersionString) + "  |  Simone De Angelis  |  GPLv3",
                500, 512, 270, 25, juce::Justification::centredRight);

    const auto closeBounds = juce::Rectangle<float> { 785.0f, 105.0f, 30.0f, 30.0f };
    g.setColour (ParallaxStyle::mutedIvory.withAlpha (0.62f));
    g.drawEllipse (closeBounds, 1.2f);
    g.drawLine (closeBounds.getX() + 9.0f, closeBounds.getY() + 9.0f,
                closeBounds.getRight() - 9.0f, closeBounds.getBottom() - 9.0f, 1.3f);
    g.drawLine (closeBounds.getRight() - 9.0f, closeBounds.getY() + 9.0f,
                closeBounds.getX() + 9.0f, closeBounds.getBottom() - 9.0f, 1.3f);
}

void InfoPanel::mouseDown (const juce::MouseEvent& event)
{
    const auto sx = static_cast<float> (getWidth()) / designWidth;
    const auto sy = static_cast<float> (getHeight()) / designHeight;
    const auto designPosition = juce::Point<float> {
        event.position.x / sx,
        event.position.y / sy
    };

    if (getCloseBounds().contains (designPosition)
        || ! getPanelBounds().contains (designPosition))
        close();
}

bool InfoPanel::keyPressed (const juce::KeyPress& key)
{
    if (key == juce::KeyPress::escapeKey)
    {
        close();
        return true;
    }

    return false;
}

juce::Rectangle<float> InfoPanel::getPanelBounds() const
{
    return { 120.0f, 82.0f, 720.0f, 476.0f };
}

juce::Rectangle<float> InfoPanel::getCloseBounds() const
{
    return { 775.0f, 95.0f, 50.0f, 50.0f };
}

void InfoPanel::close()
{
    if (onClose)
        onClose();
}

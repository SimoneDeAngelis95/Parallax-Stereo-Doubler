#include "PluginProcessor.h"
#include "PluginEditor.h"

ParallaxAudioProcessor::ParallaxAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr, "ParallaxState", createParameterLayout())
{
}

ParallaxAudioProcessor::APVTS::ParameterLayout ParallaxAudioProcessor::createParameterLayout()
{
    APVTS::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "offset", 1 }, "Offset",
        juce::NormalisableRange<float> { 0.0f, 50.0f, 1.0f }, 20.0f,
        juce::AudioParameterFloatAttributes().withLabel ("ms")));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { "delayedSide", 1 }, "Delayed Side", true));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "spread", 1 }, "Spread",
        juce::NormalisableRange<float> { 0.0f, 200.0f, 1.0f }, 100.0f,
        juce::AudioParameterFloatAttributes().withLabel ("%")));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "wow", 1 }, "Wow",
        juce::NormalisableRange<float> { 0.0f, 100.0f, 1.0f }, 0.0f,
        juce::AudioParameterFloatAttributes().withLabel ("%")));

    return layout;
}

void ParallaxAudioProcessor::prepareToPlay (double, int)
{
}

void ParallaxAudioProcessor::releaseResources()
{
}

bool ParallaxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto inputLayout = layouts.getMainInputChannelSet();
    const auto outputLayout = layouts.getMainOutputChannelSet();

    if (inputLayout != outputLayout)
        return false;

    return outputLayout == juce::AudioChannelSet::mono()
        || outputLayout == juce::AudioChannelSet::stereo();
}

void ParallaxAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                           juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    for (auto channel = getTotalNumInputChannels();
         channel < getTotalNumOutputChannels();
         ++channel)
    {
        buffer.clear (channel, 0, buffer.getNumSamples());
    }

    // Pass-through checkpoint: the input audio intentionally remains unchanged.
}

juce::AudioProcessorEditor* ParallaxAudioProcessor::createEditor()
{
    return new ParallaxAudioProcessorEditor (*this);
}

bool ParallaxAudioProcessor::hasEditor() const
{
    return true;
}

const juce::String ParallaxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ParallaxAudioProcessor::acceptsMidi() const
{
    return false;
}

bool ParallaxAudioProcessor::producesMidi() const
{
    return false;
}

bool ParallaxAudioProcessor::isMidiEffect() const
{
    return false;
}

double ParallaxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ParallaxAudioProcessor::getNumPrograms()
{
    return 1;
}

int ParallaxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ParallaxAudioProcessor::setCurrentProgram (int)
{
}

const juce::String ParallaxAudioProcessor::getProgramName (int)
{
    return {};
}

void ParallaxAudioProcessor::changeProgramName (int, const juce::String&)
{
}

void ParallaxAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = parameters.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void ParallaxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParallaxAudioProcessor();
}

#include "PluginProcessor.h"
#include "PluginEditor.h"

ParallaxAudioProcessor::ParallaxAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
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

void ParallaxAudioProcessor::getStateInformation (juce::MemoryBlock&)
{
}

void ParallaxAudioProcessor::setStateInformation (const void*, int)
{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParallaxAudioProcessor();
}


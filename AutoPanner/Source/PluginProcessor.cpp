/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AutoPannerAudioProcessor::AutoPannerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treestate(*this, nullptr, "Parameters", createParameterLayout()), autoPan()
#endif
{
    treestate.addParameterListener("rate", this);
    treestate.addParameterListener("depth", this);
    treestate.addParameterListener("pan", this);
}

AutoPannerAudioProcessor::~AutoPannerAudioProcessor()
{
    treestate.removeParameterListener("rate", this);
    treestate.removeParameterListener("depth", this);
    treestate.removeParameterListener("pan", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout AutoPannerAudioProcessor::createParameterLayout()
{
    // make a rate parameter
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("rate", "Rate", 0.1f, 1000.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("depth", "Depth", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("pan", "Pan", -1.0f, 1.0f, 0.0f));
    return { params.begin(), params.end() };

     
}

void AutoPannerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "rate")
    {
        autoPan.setRate(newValue);
    }
    if (parameterID == "depth")
	{
		autoPan.setDepth(newValue);
	}
    if (parameterID == "pan")
	{
		autoPan.setPan(newValue);
	}
}

//==============================================================================
const juce::String AutoPannerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AutoPannerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AutoPannerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AutoPannerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AutoPannerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AutoPannerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AutoPannerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AutoPannerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AutoPannerAudioProcessor::getProgramName (int index)
{
    return {};
}

void AutoPannerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AutoPannerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    autoPan.prepareToPlay(spec);

    rate = *treestate.getRawParameterValue("rate");
    depth = *treestate.getRawParameterValue("depth");
    pan = *treestate.getRawParameterValue("pan");

    autoPan.setRate(rate);
    autoPan.setDepth(depth);
    autoPan.setPan(pan);
}

void AutoPannerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AutoPannerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AutoPannerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    autoPan.processBlock(buffer);
   
}

//==============================================================================
bool AutoPannerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AutoPannerAudioProcessor::createEditor()
{
    // return new AutoPannerAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void AutoPannerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream stream(destData, false);
    treestate.state.writeToStream(stream);
}

void AutoPannerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    if (tree.isValid())
	{
		treestate.state = tree;
        autoPan.setRate(*treestate.getRawParameterValue("rate"));
        autoPan.setDepth(*treestate.getRawParameterValue("depth"));
        autoPan.setPan(*treestate.getRawParameterValue("pan"));
	}
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AutoPannerAudioProcessor();
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct CustomRotarySlider : juce::Slider
{
    CustomRotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow)
    {

    }
};

struct CustomLinearSlider : juce::Slider
{
    CustomLinearSlider() : juce::Slider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxBelow)
    {

    }
};

//==============================================================================
/**
*/
class AutoPannerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AutoPannerAudioProcessorEditor (AutoPannerAudioProcessor&);
    ~AutoPannerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AutoPannerAudioProcessor& audioProcessor;

    CustomRotarySlider depthSlider, rateSlider;
    CustomLinearSlider panSlider;
    juce::ComboBox lfoTypeBox;
    juce::ToggleButton lfoButton, syncButton;

    juce::Label depthLabel, rateLabel, panLabel, lfoTypeLabel, lfoOnLabel, syncLabel;

    juce::AudioProcessorValueTreeState::SliderAttachment depthAttachment, rateAttachment, panAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfoTypeAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment lfoButtonAttachment, syncButtonAttachment;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AutoPannerAudioProcessorEditor)
};

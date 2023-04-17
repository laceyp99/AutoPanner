/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AutoPannerAudioProcessorEditor::AutoPannerAudioProcessorEditor(AutoPannerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), panAttachment(audioProcessor.treestate, "pan", panSlider),
    depthAttachment(audioProcessor.treestate, "depth", depthSlider), rateAttachment(audioProcessor.treestate, "rate", rateSlider),
    lfoButtonAttachment(audioProcessor.treestate, "lfo on", lfoButton), syncButtonAttachment(audioProcessor.treestate, "tempo sync", syncButton)
{
    addAndMakeVisible(panSlider);
    panSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(panLabel);
    panLabel.setText("Pan", juce::dontSendNotification);
    panLabel.attachToComponent(&panSlider, false);
    panLabel.setJustificationType(juce::Justification::centredTop);

    addAndMakeVisible(depthSlider);
    depthSlider.setNormalisableRange(juce::NormalisableRange<double>(0.0, 100.0, 1.0, 0.5));
    depthSlider.setTextValueSuffix("%");
    depthSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(depthLabel);
    depthLabel.setText("Depth", juce::dontSendNotification);
    depthLabel.attachToComponent(&depthSlider, false);
    depthLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(rateSlider);
    rateSlider.setNormalisableRange(juce::NormalisableRange<double>(0.0, 7.0, 0.1, 0.5));
    rateSlider.setTextValueSuffix("Hz");
    rateSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(rateLabel);
    rateLabel.setText("Rate", juce::dontSendNotification);
    rateLabel.attachToComponent(&rateSlider, false);
    rateLabel.setJustificationType(juce::Justification::centredTop);

    addAndMakeVisible(lfoTypeBox);
    lfoTypeBox.addItem("Sine", 1);
    lfoTypeBox.addItem("Square", 2);
    lfoTypeBox.addItem("Saw", 3);
    lfoTypeBox.setSelectedId(1);
    lfoTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        (audioProcessor.treestate, "lfo type", lfoTypeBox);

    addAndMakeVisible(lfoButton);
    lfoButton.setButtonText("LFO");
    
    addAndMakeVisible(syncButton);
    syncButton.setButtonText("BPM Sync");

    setResizable(true, true);
    setResizeLimits(300, 225, 600, 400);
    getConstrainer()->setFixedAspectRatio(2.0);
    setSize(400, 300);
}

AutoPannerAudioProcessorEditor::~AutoPannerAudioProcessorEditor()
{

}

//==============================================================================
void AutoPannerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void AutoPannerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto x = 0.0;
    auto y = 0.0;
    auto bounds = getLocalBounds();
    auto width = bounds.getWidth();
    auto height = bounds.getHeight();
    
    auto panSection = bounds.removeFromBottom(height * 0.4);
    auto lfoSection = bounds.removeFromTop(height * 0.4);

    panSlider.setBounds(panSection.removeFromLeft(width * 0.5));
    depthSlider.setBounds(panSection.removeFromLeft(width * 0.25));
    rateSlider.setBounds(panSection);
    lfoTypeBox.setBounds(lfoSection.removeFromRight(width * 0.5));
    lfoButton.setBounds(lfoSection.removeFromRight(width * 0.25));
    syncButton.setBounds(lfoSection);
}

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
    lfoButtonAttachment(audioProcessor.treestate, "lfo on", lfoButton), syncButtonAttachment(audioProcessor.treestate, "tempo sync", synceButton)
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
    auto halfwidth = width * 0.5;
    auto halfheight = height * 0.5;

    panSlider.setBounds(x, halfheight, halfwidth, halfheight);
    depthSlider.setBounds(halfwidth, halfheight, halfwidth, halfheight);


}

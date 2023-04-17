#pragma once
#include <JuceHeader.h>

using namespace juce;

class AutoPan
{
public:
	AutoPan() = default;
	~AutoPan() = default;

	void prepareToPlay(dsp::ProcessSpec spec)
	{
		lfo.reset();
		lfoBuffer.clear();
		panner.reset();

		lfo.prepare({ spec.sampleRate, (uint32)spec.maximumBlockSize, 1 });
		initializeLFO(lfoTypeIndex, spec.sampleRate);
		lfoBuffer.setSize(1, spec.maximumBlockSize);

		panner.prepare(spec);
		panner.setRule(dsp::Panner<float>::Rule::linear);
		panner.setPan(panValue);
	}
	void processBlock(AudioBuffer<float>& buffer)
	{
		dsp::AudioBlock<float> block(buffer);
		auto context = dsp::ProcessContextReplacing<float>(block);

		dsp::AudioBlock<float> lfoBlock(lfoBuffer);
		auto lfoContext = dsp::ProcessContextReplacing<float>(lfoBlock);
		lfo.process(lfoContext);

		for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
		{
			auto* channelData = buffer.getWritePointer(channel);
			for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
			{
				float lfoValue = lfoBuffer.getSample(0, sample);
				if (lfoOn)
					panValue = lfoValue * depth;

				else
					panValue = pan;

				panner.setPan(panValue);
			}
		}
		panner.process(context);
		lfoBlock.clear();
	}

	void initializeLFO(float lfoTypeIndex, float samplerate)
	{
		if (lfoTypeIndex == 1)
		{
			// create a square wave lfo
			lfo.initialise([](float x) {return (juce::dsp::FastMathApproximations::sin(x)) >= 0 ? 1 : -1; }, samplerate);
		}
		if (lfoTypeIndex == 2)
		{
			// create a sawtooth wave lfo
			lfo.initialise([](float x)
				{
					return juce::jmap(x,
					float(-juce::MathConstants<double>::pi), float(juce::MathConstants<double>::pi),
					float(-1), float(1));
				}, samplerate);
		}
		else if (lfoTypeIndex == 0) // sine wave
		{
			lfo.initialise([](float x) {return juce::dsp::FastMathApproximations::sin(x); }, samplerate);
		}
	}

	void tempoSync(bool tempoSyncOn)
	{
		if (tempoSyncOn)
		{
			BPM = (float)lastPosInfo.bpm;
			bpmRate = BPM / 60.0f;
			lfo.setFrequency(bpmRate);
		}
		else
			lfo.setFrequency(rate);
	}

	void setRate(float newRate)
	{
		rate = newRate;
	}

	void setDepth(float newDepth)
	{
		depth = newDepth;
	}

	void setPan(float newPan)
	{
		pan = newPan;
	}

	void setLFO(bool onoroff)
	{
		lfoOn = onoroff;
	}

private:

	dsp::Oscillator<float> lfo;
	AudioBuffer<float> lfoBuffer;
	dsp::Panner<float> panner;
	AudioPlayHead::CurrentPositionInfo lastPosInfo;

	float BPM = 120.0f;
	float bpmRate = 0.0f;
	float rate = 0.1f;
	float depth = 0.1f;
	float pan = 0.0f;
	float panValue = 0.0f;
	bool lfoOn = 0;
	float lfoTypeIndex = 0.0f;
};
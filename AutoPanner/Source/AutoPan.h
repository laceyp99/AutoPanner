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
		lfo.initialise([](float x) {return std::sin(x); }, spec.sampleRate);
		lfo.setFrequency(rate);
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
				{
					panValue = lfoValue * depth;
				}
				else
				{
					panValue = pan;
				}
				panner.setPan(panValue);
			}
		}
		panner.process(context);
		lfoBlock.clear();
	}
	void setRate(float newRate)
	{
		rate = newRate;
		lfo.setFrequency(rate);
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

	float rate = 0.1f;
	float depth = 0.1f;
	float pan = 0.0f;
	float panValue = 0.0f;
	bool lfoOn = 0;
};
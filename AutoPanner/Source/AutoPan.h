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
		//panner.reset();

		lfo.prepare({ spec.sampleRate, (uint32)spec.maximumBlockSize, 1 });
		lfo.initialise([](float x) {return std::sin(x); }, spec.sampleRate);
		lfo.setFrequency(rate);
		lfoBuffer.setSize(1, spec.maximumBlockSize);

		//panner.prepare(spec);
		//panner.setRule(dsp::Panner<float>::Rule::linear);
		//panner.setPan(pan * depth);
	}
	void processBlock(AudioBuffer<float>& buffer)
	{
		dsp::AudioBlock<float> block(buffer);
		auto context = dsp::ProcessContextReplacing<float>(block);

		dsp::AudioBlock<float> lfoBlock(lfoBuffer);
		auto lfoContext = dsp::ProcessContextReplacing<float>(lfoBlock);
		// lfo.process(lfoContext);	

		
		

	}
	void setRate(float newRate)
	{
		rate = newRate;
		lfo.setFrequency(rate);
	}

	void setDepth(float newDepth)
	{
		depth = newDepth;
		//panner.setPan(pan * depth);
	}

	void setPan(float newPan)
	{
		pan = newPan;
		//panner.setPan(pan * depth);
	}

private:

	dsp::Oscillator<float> lfo;
	AudioBuffer<float> lfoBuffer;
	//dsp::Panner<float> panner;

	float rate = 0.1f;
	float depth = 0.1f;
	float pan = 0.0f;
	
};
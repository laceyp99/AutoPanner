#pragma once

#include <JuceHeader.h>

using namespace juce;

class PatsPanner
{
public:
	PatsPanner() // : dsp::Panner<float>()
	{
		setPan(0.0f);
	}
	
	void setPan(float newPan)
	{
		pan = newPan;
	}

	float processSample(int channel, float sample)
	{
		float output = 0.0f;
		if (channel == 0) // left channel (-1 to 0)
		{
			// if pan is -1, output = sample // if pan is 0, output = sample // if pan is 1, output = 0
			if (pan <= 0.0f)
			{
				output = sample;
			}
			else if (pan > 0.0f)
			{
				output = sample * (1.0f - pan);
			}
		}
		else if (channel == 1) // right channel (0 to 1)
		{
			// if pan is 1, output = sample // if pan is 0, output = sample // if pan is -1, output = 0
			if (pan >= 0.0f)
			{
				output = sample;
			}
			else if (pan < 0.0f)
			{
				output = sample * (1.0f + pan);
			}
		}
		
		return output;
	}

private:

	float pan = 0.0f;
};

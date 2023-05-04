# AutoPanner

This AutoPanner plugin was created using GitHub CoPilot to help generate the code. The plugin is designed to automatically pan the audio signal from left to right at a specified rate. It includes an LFO (low frequency oscillator) that can modulate the panner to create a more dynamic stereo effect. The rate of the LFO can be synced to the tempo of the music using different beat denominators.

**Requirements**

To use this audio plugin, you will need a Digital Audio Workstation (DAW) that supports VST, VST3, or AU plugin formats. The plugin was built and tested on Windows, but should also work on Mac and Linux systems.

**Installation**

To install the plugin, simply take the source files and replace them in your own project. Build the vst then copy the plugin file to your DAW's plugin folder. The plugin file will have a .vst3 extension depending on the format you choose. Once the plugin is installed, you can use it in your DAW like any other audio effect plugin.

**Usage**

The plugin has three waveform options that create different panning movements. The sine waveform smoothly moves back and forth from left to right, while the square waveform has sharp jumps between hard left and right. The saw waveform creates a typewriter effect, smoothly moving from left to right and then jumping back to hard left.

The panning rate is controlled by the pan slider, and the depth knob controls the depth of the LFO modulation. When the LFO active button is on, the rate knob controls the LFO rate, which can be synced to the tempo of the music using different beat denominators.

Read more about my process and my opinions on GitHub CoPilot on my writeup [here](https://github.com/laceyp99/AutoPanner/blob/main/AutoPanner/Source/AutoPanner%20with%20CoPilot.pdf)

#include "../JuceLibraryCode/JuceHeader.h"
#include <sndfile.h>
#ifndef LOADF_H_
#define LOADF_H_

float* load_file(const juce::String filepath, long* length, int* chn, int* f_fs);



#endif




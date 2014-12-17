#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <sndfile.h>
#include <cmath>
#include "../JuceLibraryCode/JuceHeader.h"
#include "loadfile.h"



float* load_file(const juce::String filepath, long* length, int* chn, int* f_fs){
    SF_INFO in_info;
    SNDFILE* in_file;


	int i=0;
    while(filepath[i] != '\0') {
			printf("%c\n", filepath[i]);
        i++;
    }

	char* fp = new char[i]; 
     i = 0;
	printf("start reading\n");
    while(filepath[i] != '\0') {
			printf("%c\n", filepath[i]);
        fp[i] = (const char) filepath[i];
        i++;
    }



    in_file  = sf_open(fp, SFM_READ, &in_info);
    if(in_file == NULL) {
        printf("error opening file!\n");
        return NULL;
    }

    unsigned long int num_frames = in_info.frames;
    unsigned int chn_num = in_info.channels;
	int fs = in_info.samplerate;

    printf("num_frames: %li\n", num_frames);
    printf("channels: %i\n", chn_num);

    float buffer[num_frames*chn_num];
    unsigned long int frames_read = sf_readf_float(in_file, buffer, num_frames);
    printf("read %li frames form file\n", frames_read);

	float* out_buf = new float[num_frames*chn_num];
	for(long n=0; n<num_frames*chn_num; n++) {
		out_buf[n] = buffer[n];
	}
 
    sf_close(in_file);
	*f_fs = fs;
	*length = num_frames;
	*chn = chn_num;
    return out_buf;
}

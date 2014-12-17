#include <vector>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#ifndef STEMPLER_H_
#define STEMPLER_H_

class key {
public:
	key(float n_freq);
	float get_freq();
private:
	float freq;
};

class tuning {
public:
	tuning(float* mX, long N, int keys, bool mirror, float tresh, int f_fs);
	float get_key(int which_key);
private:
	std::vector<key> keys;
	float* get_peaks(float* mX, long N, bool smooth, float tresh);
	float* get_partials(float* peaks, long N, int f_fs, long* f0bin);
	float calc_ERB(long bin, int fs, long N);
	float calc_ERB_freq(float freq, int fs, long N);
	float** dissonance_curve(float* partials, long f0bin, long N, int f_fs, long* len);
};




// common complex procedures:
float c_abs(float a, float b);
float c_arg(float a, float b);

#endif

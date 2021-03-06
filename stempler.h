#include <vector>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#ifndef STEMPLER_H_
#define STEMPLER_H_

class vals {
public:
	vals(float n_val1, float n_val2) { val1 = n_val1; val2 = n_val2;};
	float get_val1() {return val1;}
	float get_val2() { return val2;}
private:
	float val1;
	float val2;
};

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
	void make_tuning(int n_keys, bool mirror);
	float get_key(int which_key);
	std::vector<float>interv;
private:
	std::vector<key> keys;
	float* get_peaks(float* mX, long N, bool smooth, float tresh);
	float* get_partials(float* peaks, long N, int f_fs, long* f0bin);
	float calc_ERB(long bin, int fs, long N);
	float calc_ERB_freq(float freq, int fs, long N);
	float** dissonance_curve(float* partials, long f0bin, long N, int f_fs, long* len);
	void sort(float** diss, long len);

	std::vector<vals>sorted;

};




// common complex procedures:
float c_abs(float a, float b);
float c_arg(float a, float b);

#endif

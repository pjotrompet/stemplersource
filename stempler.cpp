#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include "stempler.h"

tuning::tuning(float* mX, long N, int n_keys, bool mirror, float tresh, int f_fs) {	// calculate tuning:
	float* peaks = get_peaks(mX, N, true, tresh);					//	1: find peaks in spectrum
	for(long k=0; k<N; k++) {
		if(peaks[k] > 0) {
			//std::cout<<"bin: "<<k<<" val: "<<peaks[k]<<std::endl;
		}
	}
	
	long len;
	
	float** diss_curve = dissonance_curve(peaks, N, f_fs, &len);	//	2: calculate dissonance-curve
	for(long i=0; i<len; i++) {
		std::cout<<"interval: "<<diss_curve[i][0]<<" dissonance: "<<diss_curve[i][1]<<std::endl;
	}
	
			
										//	3: sort intervals on amount of dissonance
										//	4: distill tuning from dissonance curve, n_keys and mirror
}// tuning()

float diss_curve_interp(int perc) {
	float diss_curve[] = {0, 0.7, 1, 0.9, 0.8, 0.6, 0.4, 0.2, 0.1, 0, 0, 0};
	float u_val = diss_curve[(int)((float)(perc/10)+1)];
	float d_val = diss_curve[(int)((float)(perc/10))];
	float val = d_val + ((u_val-d_val) * (perc/10-(int)(perc/10)));
	return val;
}

int tuning::calc_ERB(long bin, int fs, long N) {
	float freq = (float)bin * ((float)fs/(float)N);
	return (int)(24.7 * (4.37 * freq/1000 + 1));
}

float** tuning::dissonance_curve(float* peaks, long N, int f_fs, long* len) {
	//!!!! GET PARTIALS (f0!!!!) (door phase-check??)


	//calculate dissonance of critical band around peaks:
	float* band_diss = new float[N+calc_ERB(N, f_fs, N)];			
	for(long k=1; k<N/2; k++) band_diss[k] = 0;	
	for(long k=1; k<N/2; k++) {		
		if(peaks[k] > 0) {
			int ERB = calc_ERB(k, f_fs, N);
			for(int i=0; i<ERB; i++) {
				band_diss[k-i]+=peaks[k]*diss_curve_interp(100/ERB * i);
				band_diss[k+i]+=peaks[k]*diss_curve_interp(100/ERB * i);
			}
		}
	}

	//calculate dissonance curve:
	int diss_curve_len = 0;
	for(float interv=1; interv<2; interv+=(0.5*(float)calc_ERB(interv, f_fs, N))) diss_curve_len++;
	float* diss_curve = new float[diss_curve_len];		// allocate memory
	float* intervals = new float[diss_curve_len];
	// calculate dissonance per interval:	
	int index = 0;
	for(float interv=1; interv<2; interv+=(0.5*(float)calc_ERB(interv, f_fs, N))) {
		diss_curve[index] = 0;
		for(long n=0; n<(N+calc_ERB(N, f_fs, N))*2; n++) {				//	-	band_diss[] will be static, peaks will move against it
			if((n-(int)interv) >= 0) diss_curve[index]+=band_diss[n]*peaks[n-(int)interv];
		}
		intervals[index] = interv;
		index++;
	}															
	float** diss_per_interv = new float*[diss_curve_len];
	for(long i=0; i<diss_curve_len; i++) {
		diss_per_interv[i] = new float[2];
	}
	for(int i=0; i<diss_curve_len; i++) {
		diss_per_interv[i][0] = intervals[i];
		diss_per_interv[i][1] = diss_curve[i];
	}
	*len = diss_curve_len;
	return diss_per_interv;
}

float* tuning::get_peaks(float* mX, long N, bool smooth, float tresh) {
	float* peaks = new float[N];						// init peak buffer

	if(smooth) {
		for(long k=1; k<N; k++) {
			mX[k] = (mX[k] + mX[k-1])/2;					// smooth spectrum
		}
	}
	int peak_count = 0;
	float th = 20*std::log10(tresh);
	peaks[0] = 0; peaks[N] = 0;											// set bin 0 and N to 0.0
	for(long k=1; k<N-1; k++) {											// iterate trough spectrum
		peaks[k] = 0;													// init bin to 0
		if((mX[k-1] < mX[k]) && (mX[k+1] < mX[k]) && (mX[k] > th)) {	// is bin peak?
			peaks[k] = mX[k];											// set magnitude of peak to bin
			peak_count++;
		}
	}
	std::cout<<"peaks found: "<<peak_count<<std::endl;
	return peaks;
}


float tuning::get_key(int nth_key) {

}// get_key()



key::key(float n_freq) {
	this->freq = n_freq;
}// key()

float key::get_freq() {
	return freq;
}// get_freq()


// common complex procedures:-----------------------------------------------|
float c_abs(float a, float b) {
	return std::sqrt(a*a+b*b);
}

float c_arg(float a, float b) {
	return std::atan2(a, b);
}
// -------------------------------------------------------------------------|

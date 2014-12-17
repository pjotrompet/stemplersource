
#include <iostream>
#include <cmath>
#define TWOPI (M_PI+M_PI)
struct sig {
	float* re;
	float* im;
	unsigned long N;
	//~sig() {std::cout<<"deleting\n"; delete[] re; delete[] im;};
};

float c_abs(float a, float b) {
	return std::sqrt(a*a + b*b);
}

sig decimate(sig const& x, int odd) {
	sig out;
	out.re = new float[x.N/2];
	out.im = new float[x.N/2];
	for(unsigned long n=0; n<x.N/2; n++) {
		out.re[n] = x.re[2*n+odd];
		out.im[n] = x.im[2*n+odd];
	}
	out.N = x.N/2;
	return out;	
}

sig sig_add(sig const& x1, sig const& x2) {
	if(x1.N != x2.N) {
		std::cout<<"wrong lenght\n";	
		return x1;
	}
		sig out;
	out.re = new float[x1.N];
	out.im = new float[x1.N];
	for(unsigned long n=0; n<x1.N; n++) {
		out.re[n] = x1.re[n] + x2.re[n];
		out.im[n] = x1.im[n] + x2.im[n];
	}
	out.N = x1.N;
	return out;
}

sig sig_mul(sig const& x1, sig const& x2) {
	if(x1.N != x2.N) {
		std::cout<<"wrong lenght\n";	
		return x1;
	}
	sig out;
	out.re = new float[x1.N];
	out.im = new float[x1.N];
	for(unsigned long n=0; n<x1.N; n++) {
		out.re[n] = x1.re[n]*x2.re[n] - x1.im[n]*x2.im[n];
		out.im[n] = x1.re[n]*x2.im[n] + x1.im[n]*x2.re[n];
	}
	out.N = x1.N;
	return out;
}

sig dft(sig const& x, unsigned long N, unsigned long d, sig* &exp) {
	unsigned long len = x.N;
	if(exp == NULL) {
		exp = new sig[N];
		for(unsigned long k=0; k<N; k++) {
			exp[k].re = new float[len];
			exp[k].im = new float[len];
			for(unsigned long n=0; n<len; n++) {
				exp[k].re[n] = std::cos(TWOPI * (float)k * std::pow(2, d)*(float)n / (float)N);
				exp[k].im[n] = -std::sin(TWOPI * (float)k * std::pow(2, d)*(float)n / (float)N);
			}
			exp[k].N = len;
		}
	}

	sig X;
	X.re = new float[N];
	X.im = new float[N];
	X.N = N;

	for(unsigned long k=0; k<N; k++) {
		X.re[k] = 0;
		X.im[k] = 0;
		for(unsigned long n=0; n<len; n++) {
			X.re[k] += x.re[n] * exp[k].re[n] - x.im[k] * exp[k].im[n];
			X.im[k] += x.re[n] * exp[k].im[n] + x.im[k] * exp[k].re[n];
		}
	}
	return X;
}

sig fft(sig const& x, unsigned long N, unsigned long d, sig* &exp) {
	if(x.N < 2) {
		return dft(x, N, d, exp);
	} else {
		sig W;
		W.re = new float[N];
		W.im = new float[N];
		
		for(unsigned long k=0; k<N; k++) {
			W.re[k] = std::cos(TWOPI * std::pow(2, d)*(float)k / (float)N);
			W.im[k] = -std::sin(TWOPI * std::pow(2, d)*(float)k / (float)N);
		}
		W.N = N;
		sig xe = decimate(x, 0);
		sig xo = decimate(x, 1);

		return sig_add(fft(xe, N, d+1, exp), sig_mul(fft(xo, N, d+1, exp), W));
	}
	
}



int main() {
	sig x;
	x.N = std::pow(2, 4);
	x.re = new float[x.N];
	x.im = new float[x.N];

	for(int n=0; n<x.N; n++) {
		x.re[n] = std::pow(-1, n % 2);
		x.im[n] = 0;
	}

	sig* exp = NULL;
	sig X = fft(x, x.N, 0, exp);
	

	if(exp == NULL) std::cout<<"help null!\n";
	for(unsigned long k=0; k<X.N; k++) {
/*		std::cout<<"k: "<<k<<std::endl;
		for(unsigned long n=0; n<exp[k].N; n++) {
			std::cout<<"\tre: "<<exp[k].re[n]<<"\tim: "<<exp[k].im[n]<<std::endl;
		}
		*/
		std::cout<<"bin: "<<k<<" re: "<<X.re[k];
		std::cout<<" im: "<<X.im[k]<<std::endl;
		
	}	
}

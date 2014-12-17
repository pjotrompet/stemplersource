#include "ztrans.h"

struct sig {
	float* re;
	float* im;
	unsigned long N;
};

sig mag_spec(sig const& x, unsigned long N, unsigned long d, sig* &exp) {
	
	
}

sig dft(sig const& x, unsigned long N, unsigned d, float*** &exp) {
	unsigned long len = x.N;
	if(exp == NULL) {
		exp = new sig[N];
		for(unsigned long k=0; k<N; k++) {
			exp[k] = new sig;
			
		}
	}

}

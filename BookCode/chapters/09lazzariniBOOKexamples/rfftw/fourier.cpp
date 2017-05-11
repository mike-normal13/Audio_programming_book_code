/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////

//////////////////////////////////////////////
// rfftw interface functions
//////////////////////////////////////////////

#include "fourier.h"

bool ft=true, ift=true;
rfftw_plan forward, inverse;


void fft(float *in, float *out, int N){
int i, k;
if(ft){
forward= 
rfftw_create_plan(N, FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
ft=false;
}

for(i =0; i < N; i++) out[i] = in[i];
rfftw_one(forward, out, in);
out[0] = in[0]/N;
out[1] = in[N/2]/N;
for(i = 2, k=1; i < N; k++, i+=2) 
{
out[i] = in[k]/N;
out[i+1] = in[N-k]/N;
}


}


void ifft(float *in, float *out, int N){
int i, k;
if(ift){
inverse= 
rfftw_create_plan(N, FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);
ift=false;
}
	for(i = 2, k=1; i < N; k++, i+=2) 
{
out[k] = in[i];
out[N-k] = in[i+1];
}
out[0] = in[0];
out[N/2] = in[1];
rfftw_one(inverse, out, in);
for(i =0; i < N; i++) out[i] = in[i];
}

int formats_equal(SF_INFO a, SF_INFO b){
   if(a.channels != b.channels || 
     a.channels > 1 ||
     a.samplerate != b.samplerate ||
     a.format != b.format) return 0;
     else 
     return 1; 
}    

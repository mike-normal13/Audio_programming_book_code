/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////

//////////////////////////////////////////////
// additive synthesis function 
//////////////////////////////////////////////

#include "spec.h"
#include <stdio.h>

const int tablen = 10000;

int
addsyn(float* input, float* output, int inputsize, 
	   float thresh, float pitch, float scale, 
	   int fftsize,int hopsize, float sr) {
	
	int n, i, k, posin, posout, output_size, bins;
	float  *amp, *freq, *tab, *outsum, ratio;
	float ampnext, freqnext, *phase,incra,incrf;
	
	bins = fftsize/2;
	outsum = new float[hopsize];
	amp = new float[bins];
	phase = new float[bins];
	freq = new float[bins];
	tab = new float[tablen];
	output_size = inputsize*hopsize/fftsize;
	
	for(i=0; i <bins; i++) amp[i]= freq[i] = phase[i] = 0.f;   
	
	ratio = (float)tablen/sr;
	
	for(n=0; n<tablen; n++) tab[n] = (float)sin(n*twopi/tablen);
	
	for(posin=0, posout=0; posout < output_size; posout+=hopsize, posin+=fftsize){
		// zero outsum vector
		for(n=0; n<hopsize; n++)  outsum[n] = 0.f;
		// for each bin 
		for(i=1,k=2; i < bins; i++, k+=2){
			// get the amps & freqs from input
			ampnext =  scale*input[k+posin];
			freqnext = pitch*input[k+posin+1];
			// calculate the interpolation increment
			incra = (ampnext - amp[i])/hopsize;
			incrf = (freqnext - freq[i])/hopsize; 
			// the amplitude is above a threshold
			if(ampnext > thresh){
                // synthesise and mix in the partial
				for(n=0; n < hopsize; n++){
					phase[i] += freq[i]*ratio;
					while(phase[i] < 0) phase[i] += tablen;
					while(phase[i] >= tablen) phase[i] -= tablen;
					outsum[n] += amp[i]*tab[(int)phase[i]];	
					amp[i] += incra;
					freq[i] += incrf;
				}
			} 
			// otherwise zero the amplitude
			else amp[i] =  0.f;
		}
		for(n=0; n<hopsize; n++)  output[posout+n] = outsum[n];
	}
	
	delete[] outsum;
	delete[] tab;
	delete[] phase;
	delete[] freq;
	delete[] amp;
	return posout;
}


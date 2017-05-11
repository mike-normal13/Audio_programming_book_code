/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////

//////////////////////////////////////////////
// ifd function
//////////////////////////////////////////////

#include "fourier.h"
#include "spec.h"

int ifd(float *input, float *window, float *output, 
		int input_size, int fftsize, int hopsize, float sr){
	
	int posin, posout, i, k;
	float *sigframe, *specframe1, *specframe2, *diffwin;
	double a,b,c,d,powerspec;
	double fac = (float)(sr/twopi), fund = sr/fftsize;
	sigframe = new float[fftsize];
	specframe1 = new float[fftsize];
	specframe2 = new float[fftsize];
	diffwin = new float[fftsize];
	
	for(i=0; i < fftsize; i++){
		diffwin[i] =  window[i] - (i+1 < fftsize ? window[i+1] : 0.f);
	}
	

	for(posin=posout=0; posin < input_size; posin+=hopsize){
		
		// multiply an extracted signal frame
		// by the derivative of the window
		for(i=0; i < fftsize; i++) 
			if(posin+i < input_size)
				sigframe[i] = input[posin+i]*diffwin[i];
			else sigframe[i] = 0;
			// transform it
			fft(sigframe, specframe1, fftsize);
			
			// multiply the same signal frame
			// by the window
			for(i=0; i < fftsize; i++) 
				if(posin+i < input_size)
					sigframe[i] = input[posin+i]*window[i];
				else sigframe[i] = 0;
				// transform it
				fft(sigframe, specframe2, fftsize);
				// take care of 0Hz and Nyquist freqs	    
				   output[posout++] = specframe2[i];  
				   output[posout++] = specframe2[i+1];

				for(i=2, k=1; i < fftsize; i+=2, k++, posout+=2){

						a = specframe1[i];
						b = specframe1[i+1];
						c = specframe2[i];     
						d = specframe2[i+1];
						powerspec = c*c+d*d;
						
						// compute the amplitudes 
						output[posout] = (float) sqrt(powerspec); 
						// compute the IFD
						if(powerspec)
							output[posout+1] = (float)(((b*c - a*d)/powerspec)*fac + k*fund);
						else
							output[posout+1] = (float)(k*fund);
					
				}
				
				
	}
	
	delete[] diffwin;
	delete[] sigframe;
	delete[] specframe2;
	delete[] specframe1;
	
	return	posout;
	
}


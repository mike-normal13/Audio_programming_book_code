/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////

//////////////////////////////////////////////
// convolution function
//////////////////////////////////////////////

#include "spec.h"
#include "fourier.h"
#include <stdio.h>

void 
convol(float* impulse, float* input, float* output, 
       int impulse_size, int input_size){
	
float *impspec, *inspec, *outspec; // spectral vectors
float *insig, *outsig, *overlap;  // time-domain vectors		  
int fftsize=1, convsize; // transform and convolution sizes
int overlap_size;  // overlap size		  	
int count, i, j;   // counter and loop variables
	  
overlap_size= impulse_size - 1;
convsize = impulse_size + overlap_size;

while(fftsize < convsize) fftsize *= 2;
      
impspec = new float[fftsize]; // allocate memory for
inspec = new float[fftsize];   // spectral vectors
outspec = new float[fftsize];

insig = new float[fftsize];
outsig = new float[fftsize];
overlap = new float[overlap_size];

// get the impulse into the FFT input vector
// pad with zeros
for(i = 0; i < fftsize; i++){
      if(i < impulse_size) insig[i] = impulse[i];
      else insig[i] = 0.f;
  }


// Take the DFT of impulse
fft(insig, impspec, fftsize); 

// processing loop
for(i = count = 0; i < input_size+convsize; i++, count++){

   // if an input block is ready
    if(count == impulse_size && i < (input_size+impulse_size)){

	// copy overlapping block 
      for(j = 0; j < overlap_size ; j++)
              overlap[j] = outsig[j+impulse_size];

      // pad input signal with zeros 
      for(j = impulse_size; j < fftsize; j++) 
	           insig[j] = 0.f;

    
	// Take the DFT of input signal block   
      fft(insig, inspec, fftsize);

	// complex multiplication
      // first pair is re[0Hz] and re[Nyquist]      
      outspec[0] = inspec[0]*impspec[0];
      outspec[1] = inspec[1]*impspec[1];

      // (a+ib)*(c+id) = (ac - bd) + (ad + bc)i
      for(j = 2; j < fftsize; j+=2){
       outspec[j] = inspec[j]*impspec[j]
                -   inspec[j+1]*impspec[j+1];
       outspec[j+1] = inspec[j]*impspec[j+1]
			  +  inspec[j+1]*impspec[j];
      }

      // IDFT of the spectral product
      ifft(outspec, outsig, fftsize);

      // zero the sample counter
	count = 0;

    }

   // get the input signal
   // stop when the input is finished
 if(i < input_size)
   insig[count]  = input[i];
   else insig[count] = 0.f;

   // overlap-add output starts only
   // after the first convolution operation
  if(i >= impulse_size)
      output[i-impulse_size] = outsig[count] +
       (count < overlap_size ? overlap[count] : 0.f);
}

// de-allocate memory
delete[] overlap;
delete[] outsig;
delete[] insig;
delete[] outspec;
delete[] inspec;
delete[] impspec;
    
}

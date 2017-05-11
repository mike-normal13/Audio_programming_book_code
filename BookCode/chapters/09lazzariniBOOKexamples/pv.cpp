/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////

//////////////////////////////////////////////
// phase vocoder analysis/synthesis
//////////////////////////////////////////////

#include "spec.h"
#include "fourier.h"
#include <string.h>
#include <stdio.h>

int pva(float *input, float *window, float *output, 
        int input_size, int fftsize, int hopsize, float sr){

int posin, posout, i, k, mod;
float *sigframe, *specframe, *lastph;
float fac, scal, phi, mag, delta, pi = (float)twopi/2;

sigframe = new float[fftsize];
specframe = new float[fftsize];
lastph = new float[fftsize/2];
memset(lastph, 0, sizeof(float)*fftsize/2);

fac = (float) (sr/(hopsize*twopi));
scal = (float) (twopi*hopsize/fftsize);

for(posin=posout=0; posin < input_size; posin+=hopsize){
      mod = posin%fftsize;
	// window & rotate a signal frame
      for(i=0; i < fftsize; i++) 
          if(posin+i < input_size)
            sigframe[(i+mod)%fftsize]
                     = input[posin+i]*window[i];
           else sigframe[(i+mod)%fftsize] = 0;

      // transform it
      fft(sigframe, specframe, fftsize);

      // convert to PV output
      for(i=2,k=1; i < fftsize; i+=2, k++){

      // rectangular to polar
      mag = (float) sqrt(specframe[i]*specframe[i] + 
                        specframe[i+1]*specframe[i+1]);  
      phi = (float) atan2(specframe[i+1], specframe[i]);
      // phase diffs
      delta = phi - lastph[k];
      lastph[k] = phi;
         
      // unwrap the difference, so it lies between -pi and pi
      while(delta > pi) delta -= (float) twopi;
      while(delta < -pi) delta += (float) twopi;

      // construct the amplitude-frequency pairs
      specframe[i] = mag;
	  specframe[i+1] = (delta + k*scal)*fac;

      }
      // output it
      for(i=0; i < fftsize; i++, posout++)
			  output[posout] = specframe[i];
		  
}
delete[] sigframe;
delete[] specframe;
delete[] lastph;

return posout;
}

int pvs(float* input, float* window, float* output,
          int input_size, int fftsize, int hopsize, float sr){

int posin, posout, k, i, output_size, mod;
float *sigframe, *specframe, *lastph;
float fac, scal, phi, mag, delta;

sigframe = new float[fftsize];
specframe = new float[fftsize];
lastph = new float[fftsize/2];
memset(lastph, 0, sizeof(float)*fftsize/2);

output_size = input_size*hopsize/fftsize;

fac = (float) (hopsize*twopi/sr);
scal = sr/fftsize;

for(posout=posin=0; posout < output_size; posout+=hopsize){ 

   // load in a spectral frame from input 
   for(i=0; i < fftsize; i++, posin++)
        specframe[i] = input[posin];
	
 // convert from PV input to DFT coordinates
 for(i=2,k=1; i < fftsize; i+=2, k++){
   delta = (specframe[i+1] - k*scal)*fac;
   phi = lastph[k]+delta;
   lastph[k] = phi;
   mag = specframe[i];
  
  specframe[i] = (float) (mag*cos(phi));
  specframe[i+1] = (float) (mag*sin(phi)); 
  
}
   // inverse-transform it
   ifft(specframe, sigframe, fftsize);

   // unrotate and window it and overlap-add it
   mod = posout%fftsize;
   for(i=0; i < fftsize; i++)
       if(posout+i < output_size)
          output[posout+i] += sigframe[(i+mod)%fftsize]*window[i];
}
delete[] sigframe;
delete[] specframe;
delete[] lastph;

return output_size;
}


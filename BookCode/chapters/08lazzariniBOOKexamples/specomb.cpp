/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////

//////////////////////////////////////////////
// spectral comb filter function
//////////////////////////////////////////////
#include "spec.h"

void specomb(float *input, float *output, float scale, float delay, 
			   double radius, int fftsize, float sr) {
int i, k;
double sinw, cosw, w, radsq, rad2;
float re, im, div;

radsq = radius*radius;
rad2 = 2*radius;
delay *= sr;

// 0 Hz and Nyquist taken care of
output[0] = (float)(input[0]*(1.-radius)/(1. - rad2 + radsq))*scale;
output[1] = (float)(input[1]*-(1+radius)/(1. + rad2 + radsq))*scale;

for(i=2, k=1; i <fftsize; i+=2, k++){

    w = (delay*twopi*k)/fftsize;
    sinw = sin(w);
    cosw = cos(w);

   // real and imag parts of filter spectrum   
   div = (float) (1. - rad2*cosw + radsq);
   re = (float) (cosw - radius)/div;
   im = (float) (sinw - rad2*cosw*sinw)/div;


   // complex division
   output[i] = (input[i]*re - input[i+1]*im)*scale;
   output[i+1]= (input[i+1]*re + input[i]*im)*scale;
   

} 


}

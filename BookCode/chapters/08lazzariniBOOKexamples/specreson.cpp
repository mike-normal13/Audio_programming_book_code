/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////

//////////////////////////////////////////////
// spectral resonator program
//////////////////////////////////////////////
#include "spec.h"

void specreson(float *input, float *output, float scale, 
  double angle, double radius, int fftsize) {
int i, k;
double sinw, cosw, cos2w, w, costheta, radsq, rad2;
float re, im, div, rout, imout;

costheta = cos(angle);
radsq = radius*radius;
rad2 = 2*radius;

// 0 Hz and Nyquist taken care of
output[0] = (float)((scale*input[0])/(1. - rad2*costheta + radsq));
output[1] = (float)((scale*input[1])/(1. + rad2*costheta + radsq));

for(i=2, k=1; i <fftsize; i+=2, k++){

    w = (twopi*k)/fftsize;
    sinw = sin(w);
    cosw = cos(w);
    cos2w = cos(2*w);

   // real and imag parts of filter spectrum
   re = (float) (1. - rad2*costheta*cosw + radsq*cos2w);
   im = (float) (sinw*(rad2*costheta - 2*radsq*cosw));
   div = re*re + im*im;

   // complex division
   rout = (input[i]*re + input[i+1]*im)/div;
   imout = (input[i+1]*re - input[i]*im)/div;
   
   output[i] = scale*rout;
   output[i+1] = scale*imout;
} 


}

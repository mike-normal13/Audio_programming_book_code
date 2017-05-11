/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////

//////////////////////////////////////////////
// stft & istft functions
//////////////////////////////////////////////

#include "spec.h"
#include "fourier.h"

int stft(float *input, float *window, float *output, 
        int input_size, int fftsize, int hopsize){

int posin, posout, i, output_size;
float *sigframe, *specframe;
sigframe = new float[fftsize];
specframe = new float[fftsize];
output_size = input_size*fftsize/hopsize;

for(posin=posout=0; posin < input_size; posin+=hopsize){
	// window a signal frame
  for(i=0; i < fftsize; i++) 
   if(posin+i < input_size)
     sigframe[i] = input[posin+i]*window[i];
// if we run out of input samples
   else sigframe[i] = 0;
// transform it
fft(sigframe, specframe, fftsize);
// output it
for(i=0; i < fftsize; i++, posout++) 
         output[posout] = specframe[i];

}
delete[] sigframe;
delete[] specframe;

return output_size;
}

int istft(float* input, float* window, float* output,
          int input_size, int fftsize, int hopsize){

int posin, posout, i, output_size;
float *sigframe, *specframe;
sigframe = new float[fftsize];
specframe = new float[fftsize];
output_size = input_size*hopsize/fftsize;

for(posout=posin=0; posout < output_size; posout+=hopsize){ 
	// load in a spectral frame from input 
for(i=0; i < fftsize; i++, posin++)
        specframe[i] = input[posin];
	// inverse-transform it
ifft(specframe, sigframe, fftsize);
// window it and overlap-add it
for(i=0; i < fftsize; i++)
        if(posout+i < output_size)
          output[posout+i] += sigframe[i]*window[i];
}
delete[] sigframe;
delete[] specframe;
return output_size;
}




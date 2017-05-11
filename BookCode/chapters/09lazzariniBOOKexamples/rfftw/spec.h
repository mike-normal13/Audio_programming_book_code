#ifndef SPEC_H
#define SPEC_H

#include <math.h>

/** straight DFT (non-optmised)

    in: input array of N samples (time-domain) \n
	out: externally-allocated array of N*2 points, which will \n
	receive the DFT output ( N complex pairs) \n
	N: transform size \n
*/
void DFT(float *in, float *out, int N);

/** DFT-based convolution 

    impulse: impulse signal array \n
	input: input signal array \n
	output: output signal array \n
	impulse_size: size (in samples) of impulse\n
    input_size: size (in samples) of input\n
*/
void convol(float* impulse, float* input, float* output, 
       int impulse_size, int input_size);

/** STFT

	input: input signal array\n
	window: window array\n
	output: output signal array\n 
    input_size: size (in samples) of input\n
	fftsize: DFT transform size\n
	hopsize: number of samples between transforms\n
\n
    returns output array size\n
*/
int stft(float *input, float *window, float *output, 
        int input_size, int fftsize, int hopsize);

/** ISTFT

	input: input signal array\n
	window: window array\n
	output: output signal array \n
    input_size: size (in samples) of input\n
	fftsize: IDFT transform size\n
	hopsize: number of samples between transforms\n
    returns output array size
*/
int istft(float* input, float* window, float* output,
          int input_size, int fftsize, int hopsize);

/** cross-synthesis 

	maginput: magnitude signal input [re, im]\n
	phasinput: phase signal input [re, im]\n
	output: output signal array [re, im]\n
	fftsize: DFT transform size\n
*/
void crosspec(float* maginput, float *phasinput, float *output, 
  int fftsize);

/** simple quarter-band lowpass filter

	input: signal input [re, im]\n
	output: output signal array [re, im]\n
	fftsize: DFT transform size\n
*/
void simplp(float *input, float *output, int fftsize);

/** simple quarter-band highpass filter

	input: signal input [re, im]\n
	output: output signal array [re, im]\n
	fftsize: DFT transform size\n
*/
void simphp(float *input, float *output, int fftsize);

/** spectral resonator

	input: signal input [re, im]\n
	output: output signal array [re, im]\n
	scale: output magnitude scaling\n
	angle: filter pole angle\n
	radius: filter pole radius\n
	fftsize: DFT transform size\n
*/
void specreson(float *input, float *output, float scale, 
  double angle, double radius, int N);

/** spectral comb filter

	input: signal input [re, im]\n
	output: output signal array [re, im]\n
	scale: output magnitude scaling\n
	delay: filter delay (secs)\n
	radius: filter pole radius\n
	fftsize: DFT transform size\n
*/
void specomb(float *input, float *output, float scale, float delay, 
			   double radius, int fftsize, float sr);

/** phase differentiation

	spec: spectral signal, on input [re, im], on output [mag, phase diffs]\n
	lastphs: externally-allocated array of N/2 points used to hold previous phases\n
	fftsize: DFT transform size\n
*/
void deltaphi(float *spec, float *lastphs, int fftsize);

/** phase integration

	spec: spectral signal, on input [mag, phase diffs], on output [re, im]\n
	lastphs: externally-allocated array of N/2 points used to hold previous phases\n
	fftsize: DFT transform size\n
*/
void sigmaphi(float *spec, float *lastphs, int fftsize);

/** Phase Vocoder analysis

	input: input signal array\n
	window: window array\n
	output: output signal array \n
    input_size: size (in samples) of input\n
	fftsize: DFT transform size\n
	hopsize: number of samples between transforms\n
	float sr: sampling rate (Hz)\n

    returns output array size
*/
int pva(float *input, float *window, float *output, 
        int input_size, int fftsize, int hopsize, float sr);

/** Phase Vocoder analysis

	input: input signal array\n
	window: window array\n
	output: output signal array \n
    input_size: size (in samples) of input\n
	fftsize: DFT transform size\n
	hopsize: number of samples between transforms\n
	float sr: sampling rate (Hz)\n

    returns output array size
*/
int pvs(float *input, float *window, float *output, 
        int input_size, int fftsize, int hopsize, float sr);

/** Phase Vocoder morphing

	input1: signal input1 [amp, freq]\n
	input2: signal input2 [amp, freq]\n
	output: output signal array [amp, freq]\n
	morpha: amp morphing factor (0-1)\n
	morphfr: freq morphing factor (0-1)\n
	fftsize: DFT transform size\n
	float sr: sampling rate (Hz)\n
*/
void pvmorph(float* input1, float *input2, float *output, 
  float morpha, float morphfr, int fftsize, float sr);


/** IFD analysis

	input: input signal array\n
	window: window array\n
	output: output signal array \n
    input_size: size (in samples) of input\n
	fftsize: DFT transform size\n
	hopsize: number of samples between transforms\n
	float sr: sampling rate (Hz)\n
\n
    returns output array size
*/
int ifd(float *input, float *window, float *output, 
        int input_size, int fftsize, int hopsize, float sr);

/** Additive synthesis 

	input: input signal array\n
	output: output signal array \n
        input_size: size (in samples) of input\n
	thresh: amp threshold for synthesis\n
	pitch: pitch ratio\n
	scale: amp scaling\n
	fftsize: DFT transform size\n
	hopsize: number of samples between transforms\n
	float sr: sampling rate (Hz)\n
\n
    returns output array size
*/
int
addsyn(float* input, float* output, int inputsize, 
	    float thresh, float pitch, float scale, 
		int fftsize,int hopsize, float sr);

/** usage message
*/
void usage();
const double twopi = 8.*atan(1.);



#endif

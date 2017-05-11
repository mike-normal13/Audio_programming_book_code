/* Copyright (c) 2009 Richard Dobson

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

/* wave.h: defintion of oscillator types and associated waveform generation functions  */
#ifndef __WAVE_H_INCLUDED__
#define	__WAVE_H_INCLUDED__

#ifdef __cplusplus
extern "C" {	
#endif


#ifndef M_PI
#define M_PI  (3.1415926535897932)
#endif

#ifndef TWOPI
#define TWOPI (2.0 * M_PI)
#endif

/* definition of the oscillator type */
typedef struct  t_oscil 
{
	double srate;
	double twopiovrsr;
	double curfreq;
	double curphase;
	double incr;
} OSCIL;

/* returns pointer to an OSCIL object created on the heap. Release the memory by passing the pointer to free().*/
OSCIL*  new_oscil(double srate);
/* waveform tick functions, called per sample at the stated srate */
double sinetick  (OSCIL* p_osc, double freq);
double sqtick    (OSCIL* p_osc, double freq);
double sawdtick  (OSCIL* p_osc, double freq);
double sawutick  (OSCIL* p_osc, double freq);
double trunctick (OSCIL* p_osc, double freq);
double tritick   (OSCIL* p_osc, double freq);
/* function pointer for tick funcs */
typedef double (*tickfunc)(OSCIL* osc,double);

#ifdef __cplusplus
}	
#endif


#endif

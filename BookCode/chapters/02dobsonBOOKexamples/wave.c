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
/* wave.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/timeb.h>
#include <wave.h>

/* local typing-saving macro for tick functions */
#define OSC_WRAPPHASE  if(p_osc->curphase > TWOPI) p_osc->curphase -= TWOPI;	\
						if(p_osc->curphase < 0.0) p_osc->curphase += TWOPI

OSCIL*   new_oscil(double srate)
{
	OSCIL* p_osc;

	p_osc = (OSCIL*) malloc(sizeof(OSCIL));
	if(p_osc==NULL)
		return NULL;
	p_osc->twopiovrsr = TWOPI / srate;
	p_osc->curfreq = 0.0;
	p_osc->curphase = 0.0;
	p_osc->incr = 0.0;
	return p_osc;
}

/*********** SINE **************/

double sinetick(OSCIL* p_osc, double freq)
{
	double val;

	val = sin(p_osc->curphase);
	if(p_osc->curfreq != freq){
		p_osc->curfreq = freq;
		p_osc->incr = p_osc->twopiovrsr * freq;		
	}	
	p_osc->curphase += p_osc->incr;
	OSC_WRAPPHASE;
	return val;
}

/******** SQUARE ************/
double sqtick(OSCIL* p_osc, double freq)
{
	double val;

	if(p_osc->curfreq != freq){
		p_osc->curfreq = freq;
		p_osc->incr = p_osc->twopiovrsr * freq;		
	}

	if(p_osc->curphase <= M_PI)
		val = 1.0;
	else
		val = -1;
	p_osc->curphase += p_osc->incr;
	OSC_WRAPPHASE;
	return val;
}
/******* SAWTOOTH DOWNWARDS **********/
double sawdtick(OSCIL* p_osc, double freq)
{
	double val;

	if(p_osc->curfreq != freq){
		p_osc->curfreq = freq;
		p_osc->incr = p_osc->twopiovrsr * freq;		
	}
	val =  1.0 - 2.0 * (p_osc->curphase * (1.0 / TWOPI) );
	p_osc->curphase += p_osc->incr;
	OSC_WRAPPHASE;
	return val;
}

/********** SAWTOOTH UPWARDS **********/

double sawutick(OSCIL* p_osc, double freq)
{
	double val;

	if(p_osc->curfreq != freq){
		p_osc->curfreq = freq;
		p_osc->incr = p_osc->twopiovrsr * freq;		
	}
	val =  (2.0 * (p_osc->curphase * (1.0 / TWOPI) )) - 1.0;
	p_osc->curphase += p_osc->incr;
	OSC_WRAPPHASE;
	return val;
}

/************ TRIANGLE **************/

double tritick(OSCIL* p_osc, double freq)
{
	double val;

	if(p_osc->curfreq != freq){
		p_osc->curfreq = freq;
		p_osc->incr = p_osc->twopiovrsr * freq;		
	}

	if(p_osc->curphase <= M_PI)	{
		val =   (4.0 * (p_osc->curphase * (1.0 / TWOPI) )) - 1.0;
	}
	else {
		val =  3.0 - 4.0 * (p_osc->curphase * (1.0 / TWOPI) );
	}
	p_osc->curphase += p_osc->incr;
	OSC_WRAPPHASE;
	return val;
}


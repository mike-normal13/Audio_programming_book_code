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
#ifndef __GTABLE_H_INCLUDED__
#define	__GTABLE_H_INCLUDED__

#include "wave.h"

enum { SAW_DOWN,SAW_UP };

typedef struct t_gtable 
{
	double* table;
	unsigned long length;
} GTABLE;



typedef struct t_tab_oscil
{
	OSCIL osc;
	const GTABLE* gtable;
	double dtablen;
	double sizeovrsr;
} OSCILT;


/* strictly cyclic osc: guardpoint always copy of first */
typedef struct t_cs_oscil
{
	unsigned long flen;
	unsigned long lenmask;
	unsigned long lobits;
	unsigned long lomask;
	unsigned long maxlen;
	long	lphs;
	double	lodiv;
	double	cvtbas;
	double	cpscvt;
	double	sicvt;
	double	oneovrsr;		/* need this for cs_tick adsyn-style */
	const GTABLE *gtable;
} CS_OSCIL;


GTABLE* new_sine(unsigned long len);
GTABLE* new_gtable(unsigned long length);
GTABLE* new_pulse(unsigned long length,unsigned long nharms);
GTABLE* new_square(unsigned long length,unsigned long nharms);
GTABLE* new_triangle(unsigned long length,unsigned long nharms);
GTABLE* new_saw(unsigned long length,unsigned long nharms,int up);
GTABLE* new_instr(unsigned long length,double amps[], unsigned long nharms);
void	gtable_free(GTABLE** gtable);

OSCILT* new_oscilt(unsigned long srate,const GTABLE* table,double phase);
double tabtick(OSCILT* p_osc, double freq);
double tabitick(OSCILT* p_osc, double freq);
typedef double (*oscilt_tickfunc)(OSCILT *p_osc,double freq);


void sinefbuf(OSCIL* p_osc, float* buffer,unsigned long nframes,unsigned long nchans,const double* amp,const double* freq);
void tabinbuf(OSCILT* p_oscs[], unsigned long noscs,float* buffer,unsigned long nframes,unsigned long nchans,const double* amp,const double* freq);
CS_OSCIL* new_csoscil(double srate,const GTABLE* gtable);
double cs_itick(CS_OSCIL* p_osc, double freq);
double cs_tick(CS_OSCIL* p_osc, double freq);
void cs_tabinbuf(CS_OSCIL* p_oscs[],unsigned long noscs, float* buffer,unsigned long nframes,unsigned long nchans,
                const double* amp,const double* freq);

#endif

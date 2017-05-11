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
/* gtable.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gtable.h"

static void norm_gtable(GTABLE* gtable);

GTABLE* new_gtable(unsigned long length)
{
	unsigned long i;	
	GTABLE* gtable =  NULL;

	if(length == 0)
		return NULL;
	gtable = (GTABLE* ) malloc(sizeof(GTABLE));
	if(gtable == NULL)
		return NULL;
	gtable->table = (double*) malloc((length+1) * sizeof(double) );
	if(gtable->table== NULL){
		free(gtable);
		return NULL;
	}
	gtable->length = length;	
	for(i=0; i <= length; i++) 
		gtable->table[i] = 0.0;			
	
	return gtable;
}

void gtable_free(GTABLE** gtable)
{
	if(gtable && *gtable && (*gtable)->table){
		free((*gtable)->table);
		free(*gtable);
		*gtable = NULL;
	}
}

void norm_gtable(GTABLE* gtable)
{
	unsigned long i;
	double val, maxamp = 0.0;
	double* table = gtable->table;
	
	for(i=0;i < gtable->length;i++){
		val = fabs(table[i]);
		if(maxamp < val)
			maxamp = val;
	}
	maxamp = 1.0 / maxamp;
	for(i=0;i < gtable->length;i++)
		table[i] *= maxamp;
	table[i] = table[0];	
}


GTABLE* new_sine(unsigned long length)
{
	unsigned long i;
	double step;
	GTABLE* gtable =  NULL;
	if(length == 0)
		return NULL;
	gtable =  new_gtable(length);
	if(gtable == NULL)
		return NULL;
	/* generate sinewave */
	step  = TWOPI / length ;
	for(i=0;i < length;i++) {
		gtable->table[i] = sin(step * i);		
	}	
	gtable->table[i] = gtable->table[0];	   /* guard point*/	
	gtable->length = length;
	return gtable;
}

GTABLE* new_triangle(unsigned long length,unsigned long nharms)
{
	unsigned long i,j;
	double step,amp;
	GTABLE* gtable;
	int harmonic = 1;

	if(length == 0 || nharms == 0 || nharms >= length/2)
		return NULL;
	gtable =  new_gtable(length);
	if(gtable == NULL)
		return NULL;
	step  = TWOPI / length ;
	for(i=0;i < nharms; i++){
		amp = 1.0 / (harmonic*harmonic);
		for(j=0;j < length;j++)
			gtable->table[j] += amp * cos(step*harmonic * j);
		harmonic += 2;
	}
	norm_gtable(gtable);	
	return gtable;
}

GTABLE* new_square(unsigned long length,unsigned long nharms)
{
	unsigned long i,j;
	double step,amp;
	GTABLE* gtable;
	int harmonic = 1;

	if(length == 0 || nharms == 0 || nharms >= length/2)
		return NULL;
	gtable =  new_gtable(length);
	if(gtable == NULL)
		return NULL;
	step  = TWOPI / length ;
	for(i=0; i < nharms; i++){
		amp = 1.0 / harmonic;
		for(j=0;j < gtable->length;j++)
			gtable->table[j] += amp * sin(step*harmonic * j);
		harmonic+=2;
	}
	norm_gtable(gtable);	
	return gtable;
}

GTABLE* new_saw(unsigned long length,unsigned long nharms, int up)
{
	unsigned long i,j;
	double step,val,amp = 1.0;
	GTABLE* gtable;
	int harmonic = 1;

	if(length == 0 || nharms == 0 || nharms >= length/2)
		return NULL;
	gtable =  new_gtable(length);
	if(gtable == NULL)
		return NULL;
	step  = TWOPI / length ;
	if(up)
		amp = -1;
	for(i=0; i < nharms; i++){
		val = amp / harmonic;
		for(j=0;j < gtable->length;j++)
			gtable->table[j] += val * sin(step*harmonic * j);
		harmonic++;
	}
	norm_gtable(gtable);	
	return gtable;
}

GTABLE* new_pulse(unsigned long length,unsigned long nharms)
{
	unsigned long i,j;
	double step;
	GTABLE* gtable;
	int harmonic = 1;

	if(length == 0 || nharms == 0 || nharms >= length/2)
		return NULL;
	gtable =  new_gtable(length);
	if(gtable == NULL)
		return NULL;
	step  = TWOPI / length ;
	for(i=0;i < nharms; i++){
		for(j=0;j < gtable->length;j++)
			gtable->table[j] += cos(step*harmonic * j);
		harmonic++;
	}
	norm_gtable(gtable);	
	return gtable;
}

GTABLE* new_instr(unsigned long length,double amps[], unsigned long nharms)
{
	unsigned long i,j;
	double step;
	GTABLE* gtable;
	int harmonic = 1;

	if(length == 0 || nharms == 0 || nharms >= length/2)
		return NULL;
	gtable =  new_gtable(length);
	if(gtable == NULL)
		return NULL;
	step  = TWOPI / length ;
	for(i=0;i < nharms; i++){
		for(j=0;j < gtable->length;j++)
			gtable->table[j] += amps[i] * sin(step*harmonic * j);
		harmonic++;
	}
	norm_gtable(gtable);	
	return gtable;
}

void gen_pulse(GTABLE* gtable, unsigned long nharms)
{
	unsigned long i,j,harmonic = 1;
	double step;
	
	step = TWOPI /  gtable->length;
	for(i=0;i < nharms; i++){
		for(j=0;j < gtable->length;j++)
			gtable->table[j] += cos(step*harmonic * j);
		harmonic++;
	}
}


OSCILT*  new_oscilt(unsigned long srate,const GTABLE* gtable,double phase)
{
	OSCILT* p_osc;
	/* do we have a good GTABLE?*/
	if(gtable == NULL || gtable->table==NULL || gtable->length == 0)
		return NULL;
	p_osc = (OSCILT *) malloc(sizeof(OSCILT));
	if(p_osc==NULL)
		return NULL;
	/* init the osc..*/
	p_osc->osc.curfreq = 0.0;
	p_osc->osc.curphase = gtable->length * phase;
	p_osc->osc.incr = 0.0;
	/* then the table-specific things */
	p_osc->gtable = gtable;
	p_osc->dtablen = (double) gtable->length;		
	p_osc->sizeovrsr =  p_osc->dtablen  / (double) srate;
	return p_osc;
}

double tabtick(OSCILT* p_osc, double freq)
{	
	int index = (int) (p_osc->osc.curphase);
	double val;
	double dtablen =  p_osc->dtablen, curphase = p_osc->osc.curphase;
	double* table = p_osc->gtable->table; 
	if(p_osc->osc.curfreq != freq){
		p_osc->osc.curfreq = freq;
		p_osc->osc.incr = p_osc->sizeovrsr * p_osc->osc.curfreq;
	}
	curphase += p_osc->osc.incr;
	while(curphase >= dtablen)
		curphase -= dtablen;
	//allow negative frequencies!
	while(curphase < 0.0)
		curphase += dtablen;
	p_osc->osc.curphase = curphase;
	val = table[index];
	return val ;
}

double tabitick(OSCILT* p_osc, double freq)
{
	int base_index = (int) p_osc->osc.curphase;
	unsigned long next_index = base_index + 1;
	double frac,slope,val;
	double dtablen =  p_osc->dtablen, curphase = p_osc->osc.curphase;
	double* table = p_osc->gtable->table;

	if(p_osc->osc.curfreq != freq){
		p_osc->osc.curfreq = freq;
		p_osc->osc.incr = p_osc->sizeovrsr * p_osc->osc.curfreq;		
	}
	frac = curphase - base_index;
	val = table[base_index];
	slope = table[next_index] - val;	
	val += (frac * slope);
	curphase += p_osc->osc.incr;
        
     //curphase = fmod(curphase,dtablen);
        /* the while loop permits ferocious aliasing! */
	while(curphase >= dtablen)
            curphase -= dtablen;
	while(curphase < 0.0)
            curphase += dtablen;
	p_osc->osc.curphase = curphase;

	return val; 
}

/*********** SINE **************/

double sinetick(OSCIL* p_osc, double freq)
{
	double val;

	if(p_osc->curfreq != freq){
		p_osc->curfreq = freq;
		p_osc->incr = p_osc->twopiovrsr * freq;		
	}
	val = sin(p_osc->curphase);		  
	p_osc->curphase += p_osc->incr;
	while(p_osc->curphase > TWOPI)
		p_osc->curphase -= TWOPI;
	while(p_osc->curphase < 0.0)
		p_osc->curphase += TWOPI;
	return val;
}



/* Csound style  vector processing - expects amp and freq trajectories for this block in amp, freq buffers */

void sinefbuf(OSCIL* p_osc, float* buffer,unsigned long nframes,unsigned long nchans,const double* amp,const double* freq)
{
	float val;
	unsigned long i,j;
	
	for(i=0;i < nframes;i++){
		val = (float)(*amp++ * sin(p_osc->curphase));
		if(p_osc->curfreq != *freq){
			p_osc->curfreq = *freq;
			p_osc->incr = p_osc->twopiovrsr * *freq;
			freq++;
		}
		for(j=0;j < nchans;j++)
			*buffer++ = val; 		  
		p_osc->curphase += p_osc->incr;
		while(p_osc->curphase > TWOPI)
			p_osc->curphase -= TWOPI;
		while(p_osc->curphase < 0.0)
			p_osc->curphase += TWOPI;
	}
}

void tabinbuf(OSCILT* p_oscs[], unsigned long noscs, float* buffer,unsigned long nframes,
              unsigned long nchans,const double* amp,const double* freq)
{
    double val,ampfac;
    unsigned long i,j,k;
    ampfac = 1.0 / noscs;
    for(i=0;i < nframes;amp++,freq++,i++){
        val = 0.0;
        for(k=0;k < noscs;k++)
            val += *amp * tabitick(p_oscs[k],*freq);
        val *= ampfac;
        for(j=0;j < nchans;j++)
            *buffer++ = (float) val; 		  
    }
}

/* Csound-style integer lookup */

#define MAXLEN     0x1000000L
//= 16777216 = 65536 * 256
//#define FMAXLEN    ((MYFLT)(MAXLEN))
//#define PHMASK     0x0FFFFFFL
//#define PFRAC(x)   ((MYFLT)((x) & ftp->lomask) * ftp->lodiv)
//#define MAXPOS     0x7FFFFFFFL
/* strictly cyclic osc: guardpoint must always be copy of first, set by caller */
CS_OSCIL* new_csoscil(double srate,const GTABLE* gtable)
{
	unsigned long len,flen,flenp1,ltest,lobits,lomod,lenmask;
	CS_OSCIL* p_osc;

	if(gtable == NULL || gtable->table == NULL || gtable->length == 0)
		return NULL;
	len = gtable->length;
	flen = len;
    flen &= -2;                           /*   flen now w/o guardpt   */
    flenp1 = flen + 1;                    /*   & flenp1 with guardpt  */
      
    for (ltest=flen,lobits=0; (ltest & MAXLEN) == 0; lobits++,ltest<<=1)
		;
    if (ltest != MAXLEN)                  /*   flen must be power-of-2 */
        return NULL;

	p_osc = (CS_OSCIL*) malloc(sizeof(CS_OSCIL));
	if(p_osc==NULL)
		return NULL;
	p_osc->maxlen = MAXLEN;
	p_osc->gtable = gtable;	
    lenmask = flen-1;
    p_osc->flen = flen;
    p_osc->lenmask = lenmask;               /*   init hdr w powof2 data */
    p_osc->lobits = lobits;
    lomod = MAXLEN / flen;
    p_osc->lomask = lomod - 1;
    p_osc->lodiv = 1.0/(double)lomod;       /*    & other useful vals    */
 	p_osc->sicvt = (double)MAXLEN / srate;	
	p_osc->lphs = 0L;
	p_osc->oneovrsr = 1.0 / srate;
	return p_osc;
	
    
}

#define PHMASK     0x0FFFFFFL

double cs_itick(CS_OSCIL* p_osc, double freq)
{
	const double* table = p_osc->gtable->table;
	double frac,lodiv;
	const double *ftab;
	double val_1;
	double outval;
	//double tmp;
	unsigned long phs,inc,lomask,lobits;
	
	phs = p_osc->lphs;
	inc = (unsigned long)(freq * p_osc->sicvt);
	lomask = p_osc->lomask;
	lodiv = p_osc->lodiv;
	lobits = p_osc->lobits;
	/* tmp = fmod(phs * lodiv,1.0);	  */	/* much more expensive! */
	frac = (phs & lomask) * lodiv;
	ftab = table + (phs >> lobits);
	val_1 =	*ftab++;
	outval = val_1 + (*ftab - val_1) * frac;
	phs += inc;
	p_osc->lphs = phs & PHMASK ;

	return outval;
}

/* power-of-two trunc lookup osc used by adsyn */
double cs_tick(CS_OSCIL* p_osc, double freq)
{
	const double* table = p_osc->gtable->table;
	unsigned long phs,inc,lomask,lobits;
	double outval,frqscale;

	frqscale = freq * p_osc->oneovrsr;
	inc = (unsigned long) (freq * p_osc->gtable->length);
	
	outval = *(table + p_osc->lphs);
	p_osc->lphs += inc;
	p_osc->lphs &= p_osc->lenmask;
}
/* TODO: buffer version of above */


void cs_tabinbuf(CS_OSCIL* p_oscs[], unsigned long noscs,float* buffer,unsigned long nframes,unsigned long nchans,
                const double* amp,const double* freq)
{
    double val;
    double ampfac = 1.0 / noscs;
    unsigned long i,j,k;

    for(i=0;i < nframes;i++,amp++,freq++){
        val = 0.0;
        for(k=0;k < noscs;k++)
            val += *amp * cs_itick(p_oscs[k],*freq);
        val *= ampfac;
        for(j=0;j < nchans;j++)
            *buffer++ = (float) val; 		  
    }
}







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

#ifndef __BREAKPOINTS_H_INCLUDED
#define __BREAKPOINTS_H_INCLUDED

#include <stdio.h>

typedef struct breakpoint {
		double time;
		double value;
} BREAKPOINT;

BREAKPOINT	maxpoint(const BREAKPOINT* points,unsigned long npoints);
BREAKPOINT	minpoint(const BREAKPOINT* points,unsigned long npoints);
int			inrange(const BREAKPOINT* points,double minval,double maxval,unsigned long npoints);
double		val_at_brktime(const BREAKPOINT* points, unsigned long npoints, double time);
BREAKPOINT* get_breakpoints(FILE* fp, unsigned long* psize);

/* breakpoint stream handling */

typedef struct breakpoint_stream {
	BREAKPOINT*		points;
	BREAKPOINT		leftpoint,rightpoint;
	unsigned long	npoints;
	double			curpos;
	double			incr;
	double			width;
	double			height;
	unsigned long   ileft,iright;
	int				more_points;
} BRKSTREAM;

/* srate cannot be 0; size pointer is optional - can be NULL */
BRKSTREAM*	bps_newstream(FILE* fp,unsigned long srate,unsigned long* size);
void		bps_freepoints(BRKSTREAM* stream);
double		bps_tick(BRKSTREAM* stream);		 /* NB: no error-checking, caller must ensure stream is valid */
void		bps_rewind(BRKSTREAM* stream); 
int			bps_inrange(BRKSTREAM* stream,double minval,double maxval);
int			bps_getminmax(BRKSTREAM* stream,double *outmin,double *outmax);

/* entirely arbitrary...*/
#define NPOINTS (64)
#define LINELENGTH (80)


#endif

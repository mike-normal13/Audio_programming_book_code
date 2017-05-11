
#ifndef _SND_DEFS
#define _SND_DEFS

#include <math.h>
#include <memory.h>
#include <sndfile.h>

/** default signal vectorsize.
*/
const int   def_vsize = 64;
/** default sampling rate
*/
const float def_sr = 44100.f;
/** default control rate
*/
const float def_cr = def_sr/def_vsize;
/** default audio channels.
*/
const int   def_chans = 1;
/** default function table length
*/
const int   def_len = 1024;

/** butterworth modes 
*/
enum { BP,BR,LP,HP };

/** the pi definition. 
*/ 
const double pi = 4*atan(1.); 

/** open RIFF-Wave format output soundfile.  

   name:    soundfile name (*.wav) \n
   chans:   number of output channels\n
   sr:      sampling rate \n
   \n
   returns: soundfile handle or 0 if not successful
*/
SNDFILE *soundout_open(char* name, int chans=def_chans, float sr=def_sr);

/** close output soundfile.
*/
void soundout_close(SNDFILE *psf_out);

/** open input soundfile (any format supported by libsndfile)

   name:    soundfile name  \n
   chans:   an external variable which will hold the number of input channels\n
   sr:      an external variable which will hold the sampling rate \n
   \n
   returns: soundfile handle or 0 if not successful
*/
SNDFILE *soundin_open(char* name, int *chans, float *sr);

/** close output soundfile.
*/
void soundin_close(SNDFILE *psf_out);

/** soundfile output. 
 
   psf_out: soundfile handle \n
   buffer:  signal buffer, interleaved if multichannel \n
   vecsize: signal vector size in frames \n
\n
   returns: number of frames output to file
*/
int soundout(SNDFILE *psf_out, float *buffer, int vecsize=def_vsize);


/** soundfile input. 
 
   psf_out: soundfile handle \n
   buffer:  signal buffer, interleaved if multichannel \n
   vecsize: signal vector size in frames \n
\n
   returns: number of frames output to file
*/
int soundin(SNDFILE *psf_in, float *buffer, int vecsize=def_vsize);

/** get the number of frames in file. 
 
   psf_out: soundfile handle \n
 
\n
   returns: number of frames in file
*/
long dataframes(SNDFILE *psf);

/** linear segments table
    
	brkpts: number of break-points (counting from initial position) \n
    pts: breakpoints in [table position, value] format (at least brkpts pairs) \n  
    length: table size \n

    returns: function table as a float array\n
*/
float*  line_table(int brkpts, float* pts, int length=def_len);

/** exponential segments table
    
	brkpts: number of break-points (counting from initial position) \n
    pts: breakpoints in [table position, value] format (at least brkpts pairs) \n  
    length: table size \n

    returns: function table as a float array\n
*/
float*  exp_table(int brkpts, float* pts, int length=def_len);

/**  sinusoidal wavetable generation.

    length: wavetable size \n
	phase:  phase offset (relative to cosine phase) \n
\n
    returns: wavetable as a float array
*/
float* sinus_table(int length=def_len, float phase=0.f);

/**  fourier-series wavetable generation.

    harms:  number of harmonics \n
	amps: harmonic relative amplitudes (default: 1)\n
    length: wavetable size \n
	phase:  phase offset (relative to cosine phase)  \n
\n
    returns: wavetable as a float array

*/
float*  fourier_table(int harms, float *amps=0, 
				   int length=def_len, float phase=0.f);

/**  sawtooth wavetable generation.
 
    harms:  number of harmonics\n
    length: wavetable size\n
	\n
    returns: wavetable as a float array

*/

float* saw_table(int harms, int length=def_len);

/**  square wavetable generation.
 
    harms:  number of harmonics\n
    length: wavetable size\n
	\n
    returns: wavetable as a float array

*/
float* sqr_table(int harms, int length=def_len);

/**  triangle wavetable geneation.
 
    harms:  number of harmonics\n
    length: wavetable size\n
	\n
    returns: wavetable as a float array

*/
float* triang_table(int harms, int length=def_len);

/**  vector (ftable) normalisation. 
 
	table:  input wavetable\n
    harms:  number of harmonics\n
    length: wavetable size\n

*/
void normalise_table(float* table, int length=def_len, float scale=1.f);

/** truncating table-lookup oscillator.
   
   output:  output signal vector \n
   amp:     amplitude \n
   freq:    frequency \n
   table:   function table \n
   index:   lookup index \n
   length:  function table length \n
   vecsize: signal vector size\n
   sr:      sampling rate\n
\n
   returns: 1

*/  
float osc(float *output, float amp, float freq, 
		float *table, float *index, 
		int length=def_len, int vecsize=def_vsize, 
		float sr=def_sr);

/** interpolating table-lookup oscillator.
   
   output:  output signal vector\n
   amp:     amplitude\n
   freq:    frequency\n
   table:   function table\n
   index:   lookup index\n
   length:  function table length\n
   phase:   phase offset\n
   vecsize: signal vector size\n
   sr:      sampling rate\n
\n
   returns: the first output sample

*/ 
float osci(float *output, float amp, float freq, 
		float *table, float *index, float phase=0.f,
		int length=def_len, int vecsize=def_vsize, 
		float sr=def_sr);

/** cubic interpolation table-lookup oscillator.
   
   output:  output signal vector\n
   amp:     amplitude\n
   freq:    frequency\n
   table:   function table\n
   index:   lookup index\n
   length:  function table length\n
   phase:   phase offset\n
   vecsize: signal vector size\n
   sr:      sampling rate\n
\n
   returns: the first output sample


*/  
float oscc(float *output, float amp, float freq, 
		float *table, float *index, float phase=0.f,
		int length=def_len, int vecsize=def_vsize, 
		float sr=def_sr);

/**  adsr envelope.
   
   maxamp: maximum amplitude\n
   dur: total duration (s)\n
   at:  attack time (s)\n
   dt:  decay time (s)\n
   sus: sustain amplitude\n
   rt:  release time (s)\n
   cnt: time index\n
   cr:  control rate\n
\n
   returns: output control sample
*/
float adsr(float maxamp, float dur, float at, float dt, 
			float sus, float rt, int* cnt, float cr=def_cr);

/** line segment generator
    
     pos1: initial value \n
     dur: duration in secs
	 pos2: final value \n
     cnt: time index \n
	 cr: control rate \n
    \n
    returns: output control sample
*/
float line(float pos1, float dur, float pos2, int *cnt, float cr=def_cr);

/** exponential segment generator
    
     pos1: initial value (non-zero)\n
     dur: duration in secs
	 pos2: final value (non-zero) \n
     cnt: time index \n
	 cr: control rate \n
    \n
    returns: output control sample
*/
float expon(float pos1, float dur, float pos2, int *cnt, float cr=def_cr);

/** general-purpose curve segment generator
    
     pos1: initial value (non-zero)\n
     dur: duration in secs
	 pos2: final value (non-zero) \n
	 alpha: curve coefficient \n
     cnt: time index \n
	 cr: control rate \n
    \n
    returns: output control sample
*/
float interp(float pos1, float dur, float pos2, double alpha,
            int *cnt, float cr=def_cr);

/** 1st order lowpass filter
    
	 sig: signal buffer \n
	 freq: cutoff frequency \n
	 del: delay memory (1 sample) \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample
*/
float lowpass(float* sig, float freq, float *del, int vecsize=def_vsize, 
			  float sr=def_sr);

/** 1st order highpass filter
    
	 sig: signal buffer \n
	 freq: cutoff frequency \n
	 del: delay memory (1 sample) \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample
*/
float highpass(float* sig, float freq, float *del, int vecsize=def_vsize, 
			   float sr=def_sr);

/** 2nd order all-pole filter (resonating bp)
    
	 sig: signal buffer \n
	 freq: cutoff frequency \n
	 bw: bandwidth \n
	 del: delay memory (2 samples) \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample
*/
float resonator(float* sig, float freq, float bw, float *del, 
				int vecsize=def_vsize, float sr=def_sr);

/** 2nd order band-pass filter (resonating)
    
	 sig: signal buffer \n
	 freq: cutoff frequency \n
	 bw: bandwidth \n
	 del: delay memory (2 samples) \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample
*/
float bandpass(float* sig, float freq, float bw, float *del, 
			   int vecsize=def_vsize,float sr=def_sr);

/** 2nd order butterworth filter
    
	 sig: signal buffer \n
	 freq: cutoff frequency \n
	 bw: bandwidth \n
	 del: delay memory (2 samples) \n
	 mode: either BP, BR, LP or HP
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample
*/
float butterworth(float* sig, float freq, float bw, float *del, 
				  int mode=BP, int vecsize=def_vsize, float sr=def_sr);

/** signal balancing, envelope follower
   
	 sig: signal buffer  \n
	 comp: comparator signal \n
	 del: delay memory (2 items) \n
     freq: internal lp filter memory \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample
*/
float  balance(float *sig, float *cmp, float *del, float freq=10.f, 
			   int vecsize=def_vsize, float sr=def_sr);

/** fixed delay line

	 sig: signal buffer \n
	 dtime: delay time (s) \n
	 delay: delay memory (enough samples for delay length) \n
     pnt: external memory for circular buffer pointer
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/
float delay(float *sig, float dtime,
			float *delay, int *p, int vecsize=def_vsize, float sr=def_sr);

/** comb filter

	 sig: signal buffer \n
	 dtime: delay time (s) \n
	 gain: feedback gain  \n
	 delay: delay memory (enough samples for delay length) \n
     pnt: external memory for circular buffer pointer  \n
	 vecsize: vector size \n 
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/
float comb(float *sig, float dtime, float gain,
			float *delay, int *p, int vecsize=def_vsize, float sr=def_sr);

/** allpass filter

	 sig: signal buffer
	 dtime: delay time (s)
	 gain: feedback/feedforward gain (allpass coefficient)
	 delay: delay memory (enough samples for delay length)
     pnt: external memory for circular buffer pointer
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/
float allpass(float *sig, float dtime, float gain,
			float *delay, int *p, int vecsize=def_vsize, float sr=def_sr);


/** variable delay line

	 sig: signal buffer  \n
	 vdtime: variable delay time (s) \n
	 maxdel: max delay time (s) \n
	 delay: delay memory (enough samples for maxdelay length) \n
     pnt: external memory for circular buffer pointer \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/
float vdelay(float *sig, float vdtime, float maxdel,
			float *delay, int *p,int vecsize=def_vsize, float sr=def_sr);

/** variable delay line with feedback

	 sig: signal buffer \n
	 vdtime: variable delay time (s) \n
	 fdb: feedback gain \n
	 maxdel: max delay time (s) \n
	 delay: delay memory (enough samples for maxdelay length) \n
     pnt: external memory for circular buffer pointer \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/
float flanger(float *sig, float vdtime, float fdb, float maxdel,
			float *delay, int *p, int vecsize=def_vsize, float sr=def_sr);


/**  convolution

	 sig: signal buffer \n
	 imp: impulse response buffer \n
         del: delay memory (enough samples for impulse length) \n
	 length: size of impulse response \n
         pnt: external memory for circular buffer pointer \n
	 vecsize: vector size \n
	 sr: sampling rate \n
	 \n
	 returns: first vector sample

*/
float fir(float *sig, float *imp, float *del, 
	  int length, int *p, int vecsize=def_vsize, float sr=def_sr);

#endif  // ifndef _SND_DEFS

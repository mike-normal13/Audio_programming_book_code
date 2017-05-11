#include "waveform.h"

#include "portsf.h"

double waveform_ival(WAVEFORM, double);
int output_sample(OUTPUT, double);

void copywaveform(WAVEFORM w, SPEED s, OUTPUT out, double T)
{
    double index = 0.0;
    int total_samps = (int)(T*sr);
    int size = waveform_size(w);
    int i;
    for (i=0; i<total_samps; i++) {
      double value = waveform_ival(w, index);
      index += size*s/sr;
      if (index >= size) index -= size;
      output_sample(out, value);
    }
}

int waveform_size(WAVEFORM w)
{
    return w->size;
}

double waveform_val(WAVEFORM w, int which)
{
    return w->samples[which];
}

double waveform_ival(WAVEFORM w, double which)
{
  int x0 = (int)which;
  int x1 = (int)(which+1.0);
  double f0, f1;
  if (x1>=waveform_size(w)) x1 = 0;
  f0 = waveform_val(w,x0);
  f1 = waveform_val(w,x1);
  return f0 + (f1 - f0)*(which-x0);
}

OUTPUT initialise_sound(char *name, int srate)
{
    PSF_PROPS inprops;

    inprops.srate = (long)srate;
    inprops.chans = 1L;	
    inprops.samptype = PSF_SAMP_16;
    inprops.format = PSF_STDWAVE;
    inprops.chformat = MC_MONO;
    return psf_sndCreate(name, &inprops, 0, 0, PSF_CREATE_RDWR);
}

int output_sample(OUTPUT out, double sample)
{
    short buffer[1];
    buffer[0] = (short)(sample*32767.0);
    if (psf_sndWriteShortFrames(out, buffer, 1) != 1L)
      return 0;
    return 1;
}

int close_sound(OUTPUT out)
{
    psf_sndClose(out);
    return 1;
}

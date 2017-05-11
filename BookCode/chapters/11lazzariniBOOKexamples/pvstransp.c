#include <csdl.h>
#include <pstream.h>

typedef struct _pvstransp {
    OPDS    h;
    PVSDAT  *fout;
    PVSDAT  *fin;
    MYFLT   *kscal;
    MYFLT   *gain;
    unsigned long lastframe;
} pvstransp;

static int pvstransp_init(CSOUND *csound, pvstransp *p)
{
    long    N = p->fin->N;

    if (p->fout->frame.auxp == NULL ||
        p->fout->frame.size < sizeof(float) * (N + 2))        
        csound->AuxAlloc(csound, 
              (N + 2) * sizeof(float), &p->fout->frame);
    p->fout->N = N;
    p->fout->overlap = p->fin->overlap;
    p->fout->winsize = p->fin->winsize;
    p->fout->wintype = p->fin->wintype;
    p->fout->format = p->fin->format;
    p->fout->framecount = 1;
    p->lastframe = 0;

    return OK;
}
static int pvstransp_process(CSOUND *csound, pvstransp *p)
{
    long    i, bin, j, N = p->fout->N;
    float   max = 0.0f;
    MYFLT   pscal = (MYFLT) fabs(*p->kscal);
    float   g = (float) *p->gain;
    float   *fin = (float *) p->fin->frame.auxp;
    float   *fout = (float *) p->fout->frame.auxp;

    if (fout == NULL)
      return csound->PerfError(csound,     
               Str("pvscale: not initialised"));

    if (p->lastframe < p->fin->framecount) {
      /* do not change the Ohz and Nyquist bins */
      fout[0] = fin[0]; 
      fout[N] = fin[N];
      /* zero the output amplitudes */
      for (i = 2; i < N; i += 2) {        
        fout[i] = 0.f;
        fout[i+1] = -1.0f;
      }

      for (i=2,bin=1; i < N; bin++, i += 2) {
        /* find the target new bin */
        j = (int) (bin * pscal)*2;
        /* scale frequency and move bins */
        if (j < N && j > 0) {
          fout[j] = fin[i];
          fout[j+1] = (float) (fin[i+1] * pscal);
        }
      }
      for (i = 2; i < N; i += 2) {
        if (fout[i+1] != -1.0f)
          fout[i] *= g;  /* scale amplitude */
      }
      p->fout->framecount = p->lastframe = p->fin->framecount;
    }
    return OK;
}

static OENTRY localops[] = {
{"pvstransp", sizeof(pvstransp), 3, "f", "fkk",    
(SUBR)pvstransp_init, (SUBR)pvstransp_process}
};

LINKAGE

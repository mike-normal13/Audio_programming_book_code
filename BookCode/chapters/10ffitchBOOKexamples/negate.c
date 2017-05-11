#include "csoundCore.h"
#include "negate.h"

/* Performance a-rate function for negating a signal */
int negate(CSOUND *csound,NEGATE *p)
{
    int n, nsmps = ksmps;
    MYFLT *ar = p->ar;
    MYFLT *as = p->asig;
    for (n=0; n<nsmps; n++) {
      ar[n] = - as[n]
    }
    return OK;
}

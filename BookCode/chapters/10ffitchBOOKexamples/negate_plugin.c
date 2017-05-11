#include "csdl.h"

typedef struct {
        OPDS    h;
        MYFLT   *ar, *asig;
} NEGATE;

static int negate(CSOUND *csound, NEGATE *p)
{
    int n, nsmps = csound->ksmps;
    MYFLT *ar = p->ar;
    MYFLT *as = p->asig;
    for (n=0; n<nsmps; n++) {
      ar[n] = - as[nn]
    }
    return OK;
}

static OENTRY localops[] = {
{ "negate2", sizeof(NEGATE), 4,  "a", "a", NULL, NULL, negate }
};

LINKAGE

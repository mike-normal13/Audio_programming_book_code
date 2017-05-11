#include <stdio.h>

double A = 1.3, B = 0.21;

void henon(double *x, double *y)
{
    double new_x;
    double new_y;
    double xx = *x;
    double yy = *y;

    new_x = 1.0 + yy  - A * (xx*xx);
    new_y = B * xx;
    *x = new_x;
    *y = new_y;
}


int instr_1(double tt, double dur, double pitch)
{
/*  i1:  p6=amp,p7=vibrat,p8=glisdeltime (default < 1),
         p9=frqdrop */
    double amp, vibrat, glis, frqdrop;
    amp= 500.0;
    vibrat= 0.001;
    glis = 10.0;
    frqdrop = 0.99;
    printf("i1 %f %f 0 %f %f %.3f %f %.2f\n",
            tt, dur, pitch, amp, vibrat, glis, frqdrop);
}

int instr_2(double tt, double dur, double pitch)
{
/*  i2:  p6=amp,p7=rvbsnd,p8=lfofrq,p9=num of harmonics,
         p10=sweeprate */
    double amp, rvbsnd, lfofrq, numh, sweeprate;
    amp= 2000;
    rvbsnd = 0.5;
    lfofrq =  11;
    numh = 10;
    sweeprate =0.5;
    printf("i2 %f %f 0 %f %f %f %f %f %f\n",
           tt, dur, pitch, amp, rvbsnd, lfofrq, 
           numh, sweeprate);
}

int instr_3(double tt, double dur, double pitch)
{
/*  i3:  p6=amp,p7=rvbsnd,p8=rndfrq */
    double amp, rvbsnd, rndfrq;
    amp= 700.0;
    rvbsnd = 0.8;
    rndfrq = 57.0;
    printf("i3 %f %f 0 %f %f %f %f\n",
           tt, 10*dur, pitch, amp, rvbsnd, rndfrq);
}

int instr_4(double tt, double dur, double pitch)
{
  /*  i4:  p6=amp,p7=fltrswp:strtval,p8=fltrswp:endval,
           p9=bdwth,p10=rvbsnd */
    double amp, strtval, endval, bdwth, rvbsnd;
    amp     = 3000.0;
    strtval = 7000;
    endval  =  40;
    rvbsnd  = 0.5;
    printf("i4 %f %f 0 %f %f %f %f %f %f\n",
           tt, dur, pitch, amp, strtval, endval, 
           bdwth, rvbsnd);
}

int instr_5(double tt, double dur, double pitch)
{
/*  i5: p6=amp,p7=rvbatn,p8=pan:1.0,p9=carfrq,p10=modfrq,
        p11=modndx,p12=rndfrq */
    double amp, rvbatn, pan, carfrq, modfrq, modndx, rndfrq;
    amp = 
    rvbatn = -0.3;
    pan = 
    carfrq = 5;
    modfrq = 7;
    modndx = 17;
    rndfrq = 25;
    printf("i5 %f %f 0 %f %f %f %f %f %f %f %f\n",
           tt, dur, pitch, amp, rvbatn, pan, carfrq,
           modfrq, modndx, rndfrq);
}

int instr_6(double tt, double dur, double pitch)
{
/*  i6:  p5=swpfrq:strt,p6=swpfrq:end,p7=bndwth,p8=rvbsnd,
         p9=amp */
    double strt, end, bndwth, rvbsnd, amp2;
    strt = 3000;
    end = 17;
    bndwth = 10;
    rvbsnd = 0.6;
    amp2 = 1.6;
    printf("i6 %f %f 17 %f %f %f %f %f\n",
           tt, dur, strt, end, bndwth, rvbsnd, amp2);
}

int instr_7(double tt, double dur, double pitch)
{
/*  i7: p4=amp,p5=frq,p6=strtphse,p7=endphse,p8=ctrlamp(.1-1),
        p9=ctrlfnc, p10=mainfnc, p11=revb  */
    double amp, frq, trtphse, endphse, ctrlamp, ctrlfnc, 
           mainfnc, revb;
    amp = 9000.0;
    trtphse = 0.2;
    endphse = 0.7;
    ctrlamp = 0.5;
    ctrlfnc = 2;
    mainfnc = 3;                /* Should choose 2, 3 or 14 */
    revb = 0.12;
    printf("i7 %f %f %f %f %f %f %f %f %f %f\n", 
           tt, dur, amp, pitch, trtphse, endphse, ctrlamp,
           ctrlfnc, mainfnc, revb);
}

int instr_8(double tt, double dur, double pitch)
{
/*  p4=amp,p5=swpstrt,p6=swpend,p7=bndwt,p8=rnd1:cps,
    p9=rnd2:cps,p10=rvbsnd */
    double amp, swpstrt, swpend, bndwt, cps1, cps2, rvbsnd;
    amp = 1.95;
    swpstrt = 2000;
    swpend = 5000;
    bndwt = 300;
    cps1 = 8;
    cps2 = 5.6;
    rvbsnd = 0.4;
    printf("i8 %f %f %f %f %f %f %f %f %f\n", 
           tt, dur, amp,  swpstrt, swpend, bndwt,
           cps1, cps2, rvbsnd);
}

int instr_9(double tt, double dur, double pitch)
{
/*  i9:  p4=delsnd,p5=frq,p6=amp,p7=rvbsnd,p8=rndamp,
         p9=rndfrq */
    double delsnd, amp, rvbsnd, rndamp, rndfrq;
    delsnd = 0.4;
    amp = 1200;
    rvbsnd = 0.2;
    rndamp = 25;
    rndfrq = 100;
    printf("i9 %f %f %f %f %f %f %f %f\n",
           tt, dur, delsnd, pitch, amp, rvbsnd,
           rndamp, rndfrq);
}

int instr_10(double tt, double dur, double pitch)
{
/*  i10: p4=0,p5=frq,p6=amp,p7=rvbsnd,p8=rndamp,p9=rndfrq */
    double amp, rvbsnd, rndamp, rndfrq; 
    amp = 2000;
    rvbsnd = 0.3;
    rndamp = 6.9;
    rndfrq = 160;
    printf("i10 %f %f  0 %f %f %f %f %f\n",
           tt, dur, pitch, amp, rvbsnd, rndamp, rndfrq);
}

int instr_11(double tt, double dur, double pitch)
{
/*  i11: p4=delsnd,p5=frq,p6=amp,p7=rvbsnd */
    double delsnd, amp, vbsnd;
    delsnd = 0.3;
    amp = 2330;
    vbsnd = 0.2;
    printf("i11 %f %f %f %f %f %f\n",
           tt, dur, delsnd, pitch, amp, vbsnd);
}

int instr_12(double tt, double dur, double pitch)
{
/*  i12: p6=amp,p7=swpstrt,p8=swppeak,p9=bndwth,p10=rvbsnd */
    double amp, swpstrt, swppeak, bndwth, rvbsnd;
    amp =    2000.0;
    swpstrt = 1500;
    swppeak = 6000;
    bndwth = 4.0;
    rvbsnd = 0.2;
    printf("i12 %f %f 0 %f %f %f %f %f %f\n",
           tt, dur, pitch, amp, swpstrt, swppeak, bndwth,
           rvbsnd);
}

int instr_13(double tt, double dur, double pitch)
{
/*  i13: p6=amp,p7=vibrat,p8=dropfrq */
    double amp, vibrat, dropfrq;
    amp =    2000;
    vibrat = 40;
    dropfrq = 7;
    printf("i13 %f %f 0 %f %f %f %f\n",
           tt, dur, pitch, amp, vibrat, dropfrq);
}


int main(int argc, char **argv)
{
    int iterations = 1000;
    double x = 1.0, y = 1.0;    /* Initial values */
    double tt = 0.0, pitch;
    int i;

    printf("; amp envelopes\n");
    printf("f1 0 8192 10 1\n");
    printf("f2 0 8192 10 10 8 0 6 0 4 0 1\n");
    printf("f3 0 8192 10 10 0 5 5 0 4 3 0 1\n");
    printf("f4 0 2048 10 10 0 9 0 0 8 0 7 0 4 0 2 0 1\n");
    printf("f5 0 2048 10 5 3 2 1 0\n");
    printf("f6 0 2048 10 8 10 7 4 3 1\n");
    printf("f7 0 2048 10 7 9 11 4 2 0 1 1\n");
    printf("f8 0 2048 10 0 0 0 0 7 0 0 0 0 2 0 0 0 1 1\n");
    printf("f9 0 2048 10 10 9 8 7 6 5 4 3 2 1\n");
    printf("f10 0 2048 10 10 0 9 0 8 0 7 0 6 0 5\n");
    printf("f11 0 2048 10 10 10 9 0 0 0 3 2 0 0 1\n");
    printf("f12 0 2048 10 10 0 0 0 5 0 0 0 0 0 3\n");
    printf("f13 0 2048 10 10 0 0 0 0 3 1\n");
    printf("f14 0 8192 9 1 3 0 3 1 0 9 .333 180\n");
    printf("f15 0 8192 9 1 1 90 \n");
    printf("f16 0 2048 9 1 3 0 3 1 0 6 1 0\n");
    printf("f17 0 9  5 .1 8 1\n");
    printf("f18 0 17 5 .1 10 1 6 .4\n");
    printf("f19 0 16 2 1 7 10 7 6 5 4 2 1 1 1 1 1 1 1 1\n");
    printf("f20 0 16 -2 0 30 40 45 50 40 30 20 10 5 4 3 2 "
           "1 0 0 0\n");
    printf("f21 0 16 -2 0 20 15 10 9 8 7 6 5 4 3 2 1 0 0\n");
    printf("f22 0 9 -2 .001 .004 .007 .003 .002 .005 .009"
           " .006\n");

    for (i=0; i<iterations; i++) {
      int instr;
      henon(&x, &y);
      instr = (int)(1.0+(x+0.860850)*6.279514);
      pitch = (y+0.180779)*1000.0 + 150;
      switch (instr) {
      case 1:
        instr_1(tt, 1.0, pitch); tt += 1.0; break;
      case 2:
        instr_2(tt, 1.0, pitch); tt += 1.0; break;
      case 3:
        instr_3(tt, 1.0, pitch); tt += 1.0; break;
      case 4:
        instr_4(tt, 1.0, pitch); tt += 1.0; break;
      case 5:
        instr_5(tt, 1.0, pitch); tt += 1.0; break;
      case 6:
        instr_6(tt, 1.0, pitch); tt += 1.0; break;
      case 7:
        instr_7(tt, 1.0, pitch); tt += 1.0; break;
      case 8:
        instr_8(tt, 1.0, pitch); tt += 1.0; break;
      case 9:
        instr_9(tt, 1.0, pitch); tt += 1.0; break; 
     case 10:
        instr_10(tt, 1.0, pitch); tt += 1.0; break;
      case 11:
        instr_11(tt, 1.0, pitch); tt += 1.0; break;
      case 12:
        instr_12(tt, 1.0, pitch); tt += 1.0; break;
      case 13:
        instr_13(tt, 1.0, pitch); tt += 1.0; break;
      }
    }
    printf("\ne\n");
}

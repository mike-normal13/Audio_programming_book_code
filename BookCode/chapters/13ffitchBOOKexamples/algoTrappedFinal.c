#include <stdio.h>
#include <math.h>
#include <time.h>

#define POINTS   (800)
#define PI       (3.14159265358979323846)
#define TWO_PI   (PI+PI)

double tt = 0.0;
int pitch;			/* Last main pitch */
int stats[128];
int istats[15];
int choise[170];

#define MAX_AMP (7000)
int squence[] = { 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 4, 2, 2, 2, 4, 3, 1, 1,
                  1, 1, 5, 5, 6, 9, 9, 9, 9, 9, 9, 3, 6, 7, 7, 7, 3, 7, 8,
                  8, 8, 7, 7, 9, 9, 9, 9, 9, 9, 9, 9, 2, 2, 2, 2, 2, 2, 2,
                  4, 4, 9, 9, 9, 9, 11, 13, 9, 9, 2, 11, 2, 8, 11, 9, 9, 9,
                  9, 11, 11, 13, 13, 9, 9, 9, 2, 8, 2, 11, 8, 2, 2, 11, 11,
                  11, 11, 4, 4, 4, 5, 5, 5, 5, 4, 4, 4, 4, 5, 5, 4, 4, 4, 4,
                  12, 12, 12, 4, 4, 4, 12, 10, 10, 12, 10, 4, 4, 4, 4, 4, 4,
                  4, 4, 12, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
                  10, 10, 8, 8, 8, 8, 8, 8, 8, 4, 10, 10, 10, 10, 2, 2, 4,
                  4, 10, 10, 10, 10, 10, 10, 10} ; 
int last_amp = MAX_AMP;

void instr_0(double tt, double dur, double pitch, double xx, double yy)
{
/*  i9:  p4=delsnd,p5=frq,p6=amp,p7=rvbsnd,p8=rndamp,p9=rndfrq         */
    double delsnd, amp, rvbsnd, rndamp, rndfrq;
    delsnd = 0.4;
    pitch = 440.0*pow(2.0,((int)pitch-40.0)/12.0); /* Should be a table */
    amp = 2599;
    rvbsnd = 0.3;
    rndamp = 4.1;
    rndfrq = 440;
    istats[9]++;
    printf("i9 %f %f %f %f %.1f %.1f %.1f %.1f\n",
           0.0, 50.0*dur, delsnd, pitch, amp, rvbsnd, rndamp, rndfrq);
}

void instr_1(double tt, double dur, double pitch, double xx, double yy)
{
/*  i1:  p6=amp,p7=vibrat,p8=glisdeltime (default < 1),p9=frqdrop      */
    double amp, vibrat, glis, frqdrop;
    istats[1]++;
    amp= fabs(xx*1000);
    vibrat= 0.001;
    glis = fabs(0.9*yy)/6.0;
    frqdrop = 0.99;
    pitch = 440.0*pow(2.0,((int)pitch-40.0)/12.0); /* Should be a table */
    printf("i1 %f %f 0 %f %f %.3f %f %.2f\n",
           tt, 3.0*dur, pitch, amp, vibrat, glis, frqdrop);
}

void instr_2(double tt, double dur, double pitch, double xx, double yy)
{
/*  i2:  p6=amp,p7=rvbsnd,p8=lfofrq,p9=num of harmonics,p10=sweeprate   */
    double amp, rvbsnd, lfofrq, numh, sweeprate;
    istats[2]++;
    amp= fabs(xx*1800);
    rvbsnd = 0.5;
    lfofrq =  20*fabs(yy);       /* Was 20* but that gives noise */
    while (lfofrq>50) lfofrq -= 15;
    numh = 10;
    sweeprate =0.5;
    dur = 9.0*dur;
    while (dur>20.0) dur -= 10.0;
    pitch = 440.0*pow(2.0,((int)pitch-34.0)/12.0); /* Should be a table */
    printf("i2 %f %f 0 %f %f %f %f %f %f\n",
           tt, 9.0*dur, pitch, amp, rvbsnd, lfofrq, numh, sweeprate);
}

void instr_3(double tt, double dur, double pitch, double xx, double yy)
{
/*  i3:  p6=amp,p7=rvbsnd,p8=rndfrq                                     */
    double amp, rvbsnd, rndfrq;
    istats[3]++;
    amp= 0.7*fabs(xx*900);
    rvbsnd = 0.8;
    rndfrq = 25*fabs(yy);
    pitch = 440.0*pow(2.0,((int)pitch-40.0)/12.0); /* Should be a table */
    printf("i3 %f %f 0 %f %f %f %f\n",
           tt, 10*dur, pitch, amp, rvbsnd, rndfrq);
}

void instr_4(double tt, double dur, double pitch, double xx, double yy)
{
  /*  i4:  p6=amp,p7=fltrswp:strtval,p8=fltrswp:endval,p9=bdwth,p10=rvbsnd */
    double amp, strtval, endval, bdwth, rvbsnd;
    istats[4]++;
    amp= fabs(xx*1000);
    strtval = 7000;             /* These need variability ************ */
    endval =  40;
    bdwth = 15*yy/3.0;
    rvbsnd = 0.5;
    pitch = 440.0*pow(2.0,((int)pitch-40.0)/12.0); /* Should be a table */
    printf("i4 %f %f 0 %f %f %f %f %f %f\n",
           tt, dur, pitch, amp, strtval, endval, bdwth, rvbsnd);
}

void instr_5(double tt, double dur, double pitch, double xx, double yy)
{
/*  i5: p6=amp,p7=rvbatn,p8=pan:1.0,p9=carfrq,p10=modfrq,p11=modndx, 
        p12=rndfrq */
    double amp, rvbatn, pan, carfrq, modfrq, modndx, rndfrq;
    istats[5]++;
    amp = fabs(xx*400);
    rvbatn = -.3;
    pan = (yy>3.0 ? 1.0 : 0.1);
    carfrq = 5;
    modfrq = 7;
    modndx = 17;
    rndfrq = 25;
    pitch = 220.0*pow(2.0,((int)pitch-40.0)/12.0); /* Should be a table */
    printf("i5 %f %f 0 %f %f %f %f %f %f %f %f\n",
           tt, 8.0*dur, pitch, amp, rvbatn, pan, carfrq, modfrq,
           modndx, rndfrq);
}

void instr_6(double tt, double dur, double pitch, double xx, double yy)
{
/*  i6:  p5=swpfrq:strt,p6=swpfrq:end,p7=bndwth,p8=rvbsnd,p9=amp          */
    double amp, strt, end, bndwth, rvbsnd, amp2;
    istats[6]++;
    amp = 0.9*fabs(xx*4000);
    if (yy>3) {
      strt = 3000;
      end = 17;
      bndwth = 10;
      rvbsnd = 0.6;
      amp2 = 1.6;
    }
    else {
      strt = 6000;
      end = 9000;
      bndwth = 100;
      rvbsnd = 0.4;
      amp2 = 0.9;
    }
/*     bndwth = 50; */
/*     rvbsnd = 0.5; */
/*     amp2 = 1.0; */
    printf("i6 %f %f 17 %f %f %f %f %f\n",
           tt, 7.0*dur, strt, end, bndwth, rvbsnd, amp2);
}

void instr_7(double tt, double dur, double pitch, double xx, double yy)
{
/*  i7: p4=amp,p5=frq,p6=strtphse,p7=endphse,p8=ctrlamp(.1-1),p9=ctrlfnc,
        p10=mainfnc, p11=revb  */
    double amp, frq, trtphse, endphse, ctrlamp, ctrlfnc, mainfnc, revb;
    istats[7]++;
    amp =fabs(xx*4000);
    pitch = 440.0*pow(2.0,((int)pitch-40.0)/12.0); /* Should be a table */
    trtphse = 0.2;
    endphse = 0.7;
    ctrlamp = yy/7.0;
    ctrlfnc = 2;
    mainfnc = 3;                /* Should choose 2, 3 or 14 */
    revb = 0.12;
    printf("i7 %f %f %f %f %f %f %f %f %f %f\n", 
           tt, 7.00*dur, amp, pitch, trtphse, endphse, ctrlamp,
           ctrlfnc, mainfnc, revb);
}

void instr_8(double tt, double dur, double pitch, double xx, double yy)
{
/*  p4=amp,p5=swpstrt,p6=swpend,p7=bndwt,p8=rnd1:cps,p9=rnd2:cps,
    p10=rvbsnd */
    double amp, swpstrt, swpend, bndwt, cps1, cps2, rvbsnd;
    amp = 1.95;
    swpstrt = fabs(xx*2000);
    swpend = 5000;
    bndwt = 300;
    cps1 = 8;
    cps2 = yy;
    rvbsnd = 0.4;
    istats[8]++;
    printf("i8 %f %f %f %f %f %f %f %f %f\n", 
           tt, 10.0*dur, amp,  swpstrt, swpend, bndwt, cps1, cps2, rvbsnd);
}

void instr_9(double tt, double dur, double pitch, double xx, double yy)
{
/*  i9:  p4=delsnd,p5=frq,p6=amp,p7=rvbsnd,p8=rndamp,p9=rndfrq         */
    double delsnd, amp, rvbsnd, rndamp, rndfrq;
    delsnd = 0.4;
    pitch = 440.0*pow(2.0,((int)pitch-40.0)/12.0); /* Should be a table */
    amp = fabs(xx*10);
    rvbsnd = 0.2;
    rndamp = 4*yy;
    rndfrq = 100;
    istats[9]++;
    printf("i9 %f %f %f %f %f %f %f %f\n",
           tt, 10*dur, delsnd, pitch, amp, rvbsnd, rndamp, rndfrq);
}

void instr_10(double tt, double dur, double pitch, double xx, double yy)
{
/*  i10: p4=0,p5=frq,p6=amp,p7=rvbsnd,p8=rndamp,p9=rndfrq         */
    double amp, rvbsnd, rndamp, rndfrq; 
    pitch = 440.0*pow(2.0,((int)pitch-40.0)/12.0); /* Should be a table */
    amp = fabs(xx*1100);
    rvbsnd = 0.3;
    rndamp = 6.9;
    rndfrq = 100*fabs(yy);
    istats[10]++;
    printf("i10 %f %f  0 %f %f %f %f %f\n",
           tt, 5*dur, pitch, amp, rvbsnd, rndamp, rndfrq);
}

void instr_11(double tt, double dur, double pitch, double xx, double yy)
{
/*  i11: p4=delsnd,p5=frq,p6=amp,p7=rvbsnd                           */
    double delsnd, amp, vbsnd;
    istats[11]++;
    delsnd = 0.3;
    pitch = 440.0*pow(2.0,((int)pitch-40.0)/12.0); /* Should be a table */
    amp = fabs(xx*700);
    vbsnd = ((int)fabs(yy))%5;
    printf("i11 %f %f %f %f %f %f\n",
           tt, 10*dur, delsnd, pitch, amp, vbsnd/10.0);
}

void instr_12(double tt, double dur, double pitch, double xx, double yy)
{
/*  i12: p6=amp,p7=swpstrt,p8=swppeak,p9=bndwth,p10=rvbsnd              */
    double amp, swpstrt, swppeak, bndwth, rvbsnd;
    istats[12]++;
    pitch = 440.0*pow(2.0,((int)pitch-40.0)/12.0); /* Should be a table */
    amp =    fabs(xx*1000);
    swpstrt = 1500 /* 60 */;
    swppeak = 6000;
    bndwth = 30.0*yy/6.3;
    rvbsnd = 0.2;
    printf("i12 %f %f 0 %f %f %f %f %f %f\n",
           tt, 10*dur, pitch, amp, swpstrt, swppeak, bndwth, rvbsnd);
}

void instr_13(double tt, double dur, double pitch, double xx, double yy)
{
/*  i13: p6=amp,p7=vibrat,p8=dropfrq                             */
    double amp, vibrat, dropfrq;
    istats[13]++;
    pitch = 440.0*pow(2.0,((int)pitch-40.0)/12.0); /* Should be a table */
    amp =    fabs(xx*500);
    vibrat = 40;
    dropfrq = (yy<3 ? 5 : 7);
    printf("i13 %f %f 0 %f %f %f %f\n",
           tt, 10*dur, pitch, amp, vibrat, dropfrq);
}

static double max = -1000000.0;
static double min = 1000000.0;
int instr(double tt)
{
    int which;
    double ch = 168.0*tt/323;
/*     which = (int)(tt+0.5); */
    which = (int)(ch+0.5);
    choise[which]++;
    if (which>167) which = 1;
    if (which == 0) return 0;
    if (tt>max) max = tt;
    if (tt<min) min = tt;
    return squence[which];
}

void middle(void);

void initial(void)
{
    time_t t0 = time(0);
    int i;
    for (i=1; i<128; i++) stats[i] = 0;
    for (i=1; i<14; i++) istats[i] = 0;
    for (i=0; i<168; i++) choise[i] = 0;
    tt = 0.0f;
    printf(";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
    printf(";;jpff\n");
    printf(";****++++\n");
    printf(";**** Last modified: %s", ctime(&t0));
    printf(";****----\n");
    printf(";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
    printf("\n\n");
    printf(";; Score for Drums and Different canons #4\n");
    printf(";; do not edit -- generated by C program DD4.c\n");
    printf("\n\n; amp envelopes\n");
    printf("f1  0 8192 10  1\n");
    printf("f2  0 8192 10  10 8  0  6  0  4  0  1\n");
    printf("f3  0 8192 10  10 0  5  5  0  4  3  0  1\n");
    printf("f4  0 2048 10  10 0  9  0  0  8  0  7  0 4 0 2 0 1\n");
    printf("f5  0 2048 10  5  3  2  1  0\n");
    printf("f6  0 2048 10  8  10 7  4  3  1\n");
    printf("f7  0 2048 10  7  9  11 4  2  0  1  1\n");
    printf("f8  0 2048 10  0  0  0  0  7  0  0  0  0 2 0 0 0 1 1\n");
    printf("f9  0 2048 10  10 9  8  7  6  5  4  3  2 1\n");
    printf("f10 0 2048 10  10 0  9  0  8  0  7  0  6 0 5\n");
    printf("f11 0 2048 10  10 10 9  0  0  0  3  2  0 0 1\n");
    printf("f12 0 2048 10  10 0  0  0  5  0  0  0  0 0 3\n");
    printf("f13 0 2048 10  10 0  0  0  0  3  1\n");
    printf("f14 0 8192 9  1  3  0  3  1  0  9 .333  180\n");
    printf("f15 0 8192 9  1  1  90 \n");
    printf("f16 0 2048 9  1  3  0  3  1  0  6  1  0\n");
    printf("f17 0 9   5  .1  8  1\n");
    printf("f18 0 17  5  .1  10 1  6 .4\n");
    printf("f19 0 16  2  1  7  10 7  6  5  4  2  1  1 1 1 1 1 1 1\n");
    printf("f20 0 16  -2  0  30 40 45 50 40 30 20 10 5 4 3 2 1 0 0 0\n");
    printf("f21 0 16  -2  0  20 15 10 9  8  7  6  5  4 3 2 1 0 0\n");
    printf("f22 0 9  -2  .001 .004 .007 .003 .002 .005 .009 .006\n");
    printf(";; Quotation from \"Trapped in Convert\" by Richard Boulanger\n\n");
    printf("i198 2.9  15.1\n");
    printf("i199 2.9  15.1  3\n");
    printf("i109 2.9  2.9   0.4  31.407  600  0.2  6.2  320\n");
    printf("i109 2.93 2.85  0.43  31.770  640  0.23  6.1  300\n");
    printf("i109 7.1  7.9   0.2  33.467  700  0.4  4.5  289\n");
    printf("i109 7.14 7.78  0.17  34.050  700  0.43  4.4  280\n");
    printf("s\n");
    middle();
    printf("\n;; End of Quotation\n\n");
}

void middle(void)
{
    printf(";; Second quotation from Trapped\n");
    printf("b 196\n");
    printf("i109 0   5   0.4  1108.668 1200  0.2  28   39\n");
    printf("i109 0   5   0.3  1040.410 1200  0.4  29   37\n");
    printf("i103 0.5  6.5  0   6163.728 600  0.6  47\n");
    printf("i199 4.5  2   7\n");
    printf("i106 4.5  4.3  17   6000   9000  100  0.4  0.9\n");
    printf("i199 6.5  9.9  0.7\n");
    printf("i107 6.5  3.2  9999  37.349  0.2  0.7  0.6  2  3 0.12\n");
    printf("i107 8   1.9  9800  40.965  0.01  0.9  1   3  2 0.23\n");
    printf("i107 9   3.8  9900  1052.541 0.99  0.1  0.3  3  2 0.12\n");
    printf("i103 9.1  5.5  0   1154.427 900  0.5  67\n");
    printf("i107 9.2  2.5  9890  44.158  0.1  1.0  0.7  2  3 0.23\n");
    printf("i108 11  4.6  4   20    8000  590  2   9.9 0.6\n");
    printf("i108 15.5 3.1  3   50    4000  129  8   2.6 0.3\n");
    printf("i108 16.2 4.3  2   10000  9000  200  12   17.9 1\n");
    printf("i199 16.4 10.3  0.3\n");
    printf("i107 16.4 3.5  8000  36.496  0.2  0.7  0.5  2  3 0.1\n");
    printf("i107 19.3 6.4  8000  41.439  0.01  0.9  1   3  2 0.1\n");
    printf("i109 20.2 4   0.4  590.702 1000  0.2  4   100\n");
    printf("i109 21  4.2  0.4  926.926 1100  0.4  4   167\n");
    printf("i109 21.1 8   0.4  670.752 1000  0.6  4   210\n");
    printf("i109 21.25 5   0.4  748.553 1200  0.9  5   200\n");
    printf("i199 26.7 5.3  11\n");
    printf("i109 26.7 1.9  0.5  31.588  3100  0.1  5.9  300\n");
    printf("i109 29.1 2.1  0.1  35.252  2900  0.2  4.2  390\n");
    printf("i199 32  6.2  0.28\n");
    printf("i109 32  9.1  0.34  43.904  2300  0.5  3.8  420\n");
    printf("i109 32.1 9.0  0.4  44.083  2500  0.4  4   430\n");
    printf("b 0\n");
}

void tail(int end)
{
    if (end)  printf("e\n");
    fprintf(stderr, "Max/Min of tt = %f/%f\n", max, min);
}

int max_pitch = -1;
int min_pitch = 2000000;

double y_max = -1.0;
double y_min = 1.0;
static int early =1;

void output(double x0, double y, double hx, double hy)
{
    double dur;
    int ins;
    double x;
/*     printf(";;;**** T: %f %f H: %f %f\n", x0, y, hx, hy); */
    x = fabs(x0);
    pitch = 1+(int)(x*400.0/TWO_PI);
    if (pitch>max_pitch) max_pitch = pitch;
    if (pitch<min_pitch) min_pitch = pitch;
    tt += fabs(3.0*y);
    dur = fabs(y) *60.0/(float)pitch; /* Make duration dependent on pitch */
    if (dur>1) dur = 3.0f*(float)sqrt((double)dur);
    ins = instr(tt);
    if (early && tt>194.0) {
      printf("b 43\n");
      early = 0;
    }
    if (hy>y_max) y_max = hy;
    if (hy<y_min) y_min = hy;
    switch (ins) {
    case 0:
      instr_0(tt, dur, pitch, hx, hy);
      break;
    case 1:
      instr_1(tt, dur, pitch, hx, hy);
      break;
    case 2:
      instr_2(tt, dur, pitch, hx, hy);
      break;
    case 3:
      instr_3(tt, dur, pitch, hx, hy);
      break;
    case 4:
      instr_4(tt, dur, pitch, hx, hy);
      break;
    case 5:
      instr_5(tt, dur, pitch, hx, hy);
      break;
    case 6:
      instr_6(tt, dur, pitch, hx, hy);
      break;
    case 7:
      instr_7(tt, dur, pitch, hx, hy);
      break;
    case 8:
      instr_8(tt, dur, pitch, hx, hy);
      break;
    case 9:
      instr_9(tt, dur, pitch, hx, hy);
      break;
    case 10:
      instr_10(tt, dur, pitch, hx, hy);
      break;
    case 11:
      instr_11(tt, dur, pitch, hx, hy);
      break;
    case 12:
      instr_12(tt, dur, pitch, hx, hy);
      break;
    case 13:
      instr_13(tt, dur, pitch, hx, hy);
      break;
    default:
      fprintf(stderr,"Intr %d not done\n", ins);
    }
    stats[pitch]++; 
}

double A = 1.3, B = 0.21;

void henon(double *x, double *y)
{
    double new_x;
    double new_y;
    double xx = *x;

    new_x = *y + 1.0 - (A * (xx*xx));
    new_y = B*xx;
    *x = new_x;
    *y = new_y;
}

double K = 1.2;

void torus(double *Iota, double *Theta)
{
    double new_iota;
    double new_theta;
    double theta = *Theta;

    new_iota = *Iota + K*sin(theta);
    new_theta = theta + new_iota;
				/* Normalise */
    while (new_iota < 0.0) new_iota += TWO_PI;
    while (new_iota >= TWO_PI) new_iota -= TWO_PI;
    while (new_theta < 0.0) new_theta += TWO_PI;
    while (new_theta >= TWO_PI) new_theta -= TWO_PI;
    *Iota = new_iota;
    *Theta = new_theta;
/*  fprintf(stderr,"Iota = %f, Theta = %f\n", new_iota, new_theta); */
}

#ifdef LORENZ
double S = 10.0, R = 28.0, BL = 8.0/3.0;

void lorenz(double *xx, double *yy, double *zz)
{
#define	delta_t		(0.1)
    double x = *xx;
    double y = *yy;
    double z = *zz;
    double xdot = S * (y - z);
    double ydot = (R * x) - y - (x * z);
    double zdot = (x * y) - (B * z);

    *xx = x + (xdot * delta_t);
    *yy = y + (ydot * delta_t);
    *zz = z + (zdot * delta_t);
}
#endif

int main(int argc, char *argv)
{
    double x = 0.1,
           y = 0.1;
    double hx = 1,
           hy = 1;
    int i;

    initial();
    for (i=0; i<POINTS || last_amp!=MAX_AMP; i++) { /* Some iterations */
      torus(&x, &y);
      henon(&hx, &hy);
      output(hx, hy, x, y);     /* Or the other way? */
    }

    printf("i98\t-43\t%.2f\n", tt+65);  /* Delay */
    printf("i99\t-43\t%.2f 5\n", tt+65);  /* Reverb */
    tail(1);
    fprintf(stderr, "Max,Min pitch = %d, %d\n", max_pitch, min_pitch);
    fprintf(stderr, "Max,Min y     = %f, %f\n", y_max, y_min);
    fprintf(stderr, "Duration: %.2f(%.2f)\n", tt+15, (tt+15)*60/365);
    fprintf(stderr, "Note frequency\n\n");
    for (i=1; i<128; i++) fprintf(stderr, "%3d: %3d%c", i, stats[i],
                           ((i%5)==0 ? '\n' : '\t'));
    fprintf(stderr, "\n\nInstrument Stats\n");
    for (i=1; i<14; i++) fprintf(stderr, "%2d: %3d%c", i, istats[i],
                           ((i%5)==0 ? '\n' : '\t'));
    fprintf(stderr, "\n\nChoice stats\n");
    for (i=0; i<168; i++) fprintf(stderr, "%2d: %3d%c", i, choise[i],
                           (((1+i)%5)==0 ? '\n' : '\t'));
    fprintf(stderr, "\n\n");
    return 0;
}
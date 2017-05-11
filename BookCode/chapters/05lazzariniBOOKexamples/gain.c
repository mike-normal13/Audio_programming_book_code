/* gain.c
   simple program demonstrating scaling
   from "Introduction to Digital Signals"

   (c) V Lazzarini, 2007
*/
#include <stdio.h>
#include <sndfile.h>
#include <stdlib.h>
#define N 1024

void usage_and_exit();
int formats_equal(SF_INFO a, SF_INFO b);

int main(int argc, char** argv) {

  SNDFILE *fp1,*fpo;                                /* soundfile handle */
  SF_INFO   sfinfo1;                           /* soundfile format data */
  int cnt;                                        /* sample block count */  
  int n;                                                /* sample index */
  short sig1[N],scaled[N];	       /* 16-bit (2-byte) signal blocks */
  float gain;                                           /* scaling gain */

  if(argc != 4) usage_and_exit();

  fp1 = sf_open(argv[1], SFM_READ, &sfinfo1);        /* open soundfile 1 */
  if(fp1 == NULL) usage_and_exit();

  gain = atof(argv[3]);
  fpo = sf_open(argv[2], SFM_WRITE, &sfinfo1);      /* open the soundfile */
  do {                      /* file-writing loop */
    cnt = sf_read_short(fp1, sig1, N);
    for(n=0; n < cnt; n++)                   /* the signal processing loop */
                scaled[n]= sig1[n]*gain;
    sf_write_short(fpo, scaled, cnt);   /* write the cnt-sample data block */
  }
  while(cnt);
  sf_close(fpo);                                        /* close the files */
  sf_close(fp1);
}

void usage_and_exit(){
  printf("usage: gain input1 output scale\n"
         "where input1 is the input soundfile\n"
         "scale is the gain factor and output is the\n"
         "output filename \n"
	 );
  exit(1);
}

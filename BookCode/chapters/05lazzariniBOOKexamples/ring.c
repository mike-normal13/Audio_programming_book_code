/* ring.c
   simple program demonstrating ring modulation
   from "Introduction to Digital Signals"

   (c) V Lazzarini, 2007
*/

#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#define N 1024

void usage_and_exit();
int formats_equal(SF_INFO a, SF_INFO b);

int main(int argc, char** argv) {

  SNDFILE *fp1,*fp2,*fpo;                           /* soundfile handle */
  SF_INFO   sfinfo1,sfinfo2;                   /* soundfile format data */
  int cnt;                                        /* sample block count */  
  int n;                                                /* sample index */
  short sig1[N],sig2[N],ring[N];       /* 16-bit (2-byte) signal blocks */
  if(argc != 4) usage_and_exit();

  fp1 = sf_open(argv[1], SFM_READ, &sfinfo1);        /* open soundfile 1 */
  fp2 = sf_open(argv[2], SFM_READ, &sfinfo2);        /* open soundfile 2 */

  if(fp1 == NULL || fp2 == NULL) usage_and_exit();

  if(formats_equal(sfinfo1,sfinfo2)){                   /* if same format */
  fpo = sf_open(argv[3], SFM_WRITE, &sfinfo1);      /* open the soundfile */
  do {                      /* file-writing loop */
    cnt = sf_read_short(fp1, sig1, N);
    sf_read_short(fp2, sig2, N); 
    for(n=0; n < cnt; n++)                   /* the signal processing loop */
                ring[n]= sig1[n]*sig2[n];
    sf_write_short(fpo, ring, cnt);       /* write the cnt-sample data block */
  }
  while(cnt);
  sf_close(fpo);
  }                                        /* close the files */
  sf_close(fp1);
  sf_close(fp2);
}

int formats_equal(SF_INFO a, SF_INFO b){
  if(a.format == b.format && 
     a.samplerate == b.samplerate &&
     a.channels == b.channels)
    return 1;
  else return 0;
}

void usage_and_exit(){
  printf("usage: ring input1 input2 output\n"
         "where input1 & input2  are soundfiles\n"
         "with the same format and output is the\n"
          "ring-modulated filename \n"
	 );
  exit(1);
}

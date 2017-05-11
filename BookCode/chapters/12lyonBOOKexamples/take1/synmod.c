#include <stdio.h>
#include <stdlib.h>
#include "synmod.h"

int main(int argc, char **argv)
{

  OSCMOD *oscs;
  int osc_count = 0;
  char modname[64];

  oscs = (OSCMOD *) malloc(MAXMODS * sizeof(OSCMOD));

  while(scanf("%s", modname) != EOF){
    if(! strcmp(modname, "OSC")){
      /* READ IN THE DATA */
      scanf("%s", oscs[osc_count].sig_out);
      scanf("%s", oscs[osc_count].frequency);
      scanf("%s", oscs[osc_count].waveform);
      scanf("%s", oscs[osc_count].sig_am);
      scanf("%s", oscs[osc_count].sig_fm);
      scanf("%s", oscs[osc_count].omin);
      scanf("%s", oscs[osc_count].omax);

      /* PRINT IT TO MAKE SURE IT'S OK */

      printf("%s %s %s %s %s %s %s\n",
             oscs[osc_count].sig_out,oscs[osc_count].frequency,
             oscs[osc_count].waveform, oscs[osc_count].sig_am,
             oscs[osc_count].sig_fm,oscs[osc_count].omin,
             oscs[osc_count].omax);
      ++osc_count;
    }
  }
	return 0;
}

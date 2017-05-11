#define SYNMOD_CHARS (128)
#define MAXMODS (256)

typedef struct {
  char frequency[SYNMOD_CHARS];
  char sig_out[SYNMOD_CHARS];
  char sig_am[SYNMOD_CHARS];
  char sig_fm[SYNMOD_CHARS];
  char waveform[SYNMOD_CHARS];
  char omin[SYNMOD_CHARS];
  char omax[SYNMOD_CHARS];
} OSCMOD;

void read_osc(OSCMOD *oscs, int count);
void print_osc(OSCMOD osc);

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

typedef struct {
  char outvar[SYNMOD_CHARS];
  char amplitude[SYNMOD_CHARS];
} MIXOUT;

typedef struct {
  char speed[SYNMOD_CHARS];
  char sig_out[SYNMOD_CHARS];
  char seed[SYNMOD_CHARS];
  char omin[SYNMOD_CHARS];
  char omax[SYNMOD_CHARS];
} NOISEMOD;

void read_osc(OSCMOD *oscs, int count);
void print_osc(OSCMOD osc);
void read_mix(MIXOUT *mix, int count);
void print_mix(MIXOUT mix);
void print_header(void);
void print_score(float duration);
void initialize_globals(OSCMOD *oscs, int osc_count, NOISEMOD *noises, int noise_count);
void read_noise(NOISEMOD *unit, int count);
void print_noise(NOISEMOD noise);



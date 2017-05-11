static char     RIFF_ID[4] = {'R','I','F','F'};
static char     WAVE_ID[4] = {'W','A','V','E'};
static char     FMT_ID[4]  = {'f','m','t',' '};
static char     DATA_ID[4] = {'d','a','t','a'};


typedef struct riff_wave_header {
  int	magic;	      /* 'RIFF' */
  int	len0;			/*  file size - 8 (in bytes) */
  int	magic1;		/* 'WAVE' */
  int	magic2;		/*  'fmt '  */
  int	len;			/* chunk length (16 bytes)      */
  short	format;	      /* 1 is PCM, rest not important */
  short	nchns;		/* channels */
  int	rate;			/* sampling frequency */
  int	aver;			/* nBlockAlign * rate */
  short	nBlockAlign;	/* nchns * size / 8  */
  short	bits;			/* size of each sample (8,16,32) */
  int	magic3;		/* 'data'                        */
  int	datasize;		/* sound data size in bytes      */
} wavehead;

void 
 update_header(wavehead* header, int sr, int channels, 
	       int precision, int databytes);



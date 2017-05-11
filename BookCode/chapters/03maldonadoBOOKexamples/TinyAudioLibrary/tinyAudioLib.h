#ifndef TYNYALIB_H 
#define TYNYALIB_H (1)

#ifdef __cplusplus
extern "C" {
#endif

#define NO_PORT (-1)
#define CHOOSE_PORT (-2)
#define DEFAULT_PORT (-3)

/* ======== AUDIO ================== */

/* sets an audio OUT or IN device (optional).
   If not set, the default audio i/o ports are used.
    port = -1 --> No out port (for programs that use only audio input)
    port = -2 --> Choose port from a console list of available ports
    port <= -3 --> set the default port ID
*/
void setOutPort(int port); 
void setInPort(int port); 

/*  hint for setting the latency 
    in seconds. Not too much precise 
    (optional) 
*/
void setInputLatency(double seconds); 
void setOutputLatency(double seconds); 

/* sets the sample rate of the ports. 
   Must be compatible with your card capabilities. 
   Optional, default = 44100
*/
void setSampleRate(double sr);

/* initialize AUDIO operations (optional),
   if not explicitly called from the user,
   it is called at the first output or input 
   operation.
 */
void tinyInit(); 

/* cleanup AUDIO operations (optional),
   if not called explicitly from the user,
   it is automatically called at the end of program. 
   Warning, if the program is killed abnormally, 
   such cleanup could not be done.
 */
void tinyExit(); 

/* inputs or outputs a stereo frame (i.e. a couple of
   sample, one for the left channel and one for the rigth one.
*/
void inSampleStereo( float *sampleLeft, float *sampleRight);
void outSampleStereo(float sampleLeft, float sampleRight);


/* inputs or outputs a mono sample */
float inSampleMono(void);
void outSampleMono(float sample);


/* inputs or outputs a block of mono samles */
void inBlockMono(float *samples, long numsamples);
void outBlockMono(float *samples, long numsamples);

/* inputs or outputs two blocks of samples in stereo.
   The first block for the left channel, 
   the other for the rigth channel.
   The argments are the pointers to the blocks and the
   number of samples of the two blocks (the number of
   samples is the same for both the blocks).
*/
void inBlockStereo(float *samplesLeft, float *samplesRight, long numframes);
void outBlockStereo(float *samplesLeft, float *samplesRight, long numframes);

/* outputs a block of interleaved stereo frames */
void outBlockInterleaved(float *samples, long numframes);

#ifdef __cplusplus
};
#endif

#endif /* TYNYALIB_H */

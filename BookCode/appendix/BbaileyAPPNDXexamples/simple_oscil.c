// 
// simple_oscil
// author: jonathan bailey, (c) 2009 
// see License.txt for license info
// a simple monophonic synthesizer, based on a single oscillator and 
// using portaudio, for use in The Audio Programming Book, MIT Press 
// 

// included libraries
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"
#include "simple_waveforms.h"

// constants for generation of period waveforms and portaudio
#define TABLE_LENGTH        8192
#define SAMPLE_RATE         44100.0

#define MIN_FREQ 0.0
#define MAX_FREQ 22050.0

#define MIN_DUR .01
#define MAX_DUR 1000.0

#define MIN_TABLE 1
#define MAX_TABLE 4

// define a data structure that will be used to pass user data to portaudio
typedef struct
{
  float table_inc;   // table lookup increment
  float table_index; // table lookup current index (ie, phase)
  int   table_num;
  float **tables;
  float duration;
} simpleOscil;


//// portaudio callback function ////

// this function is defined in the portaudio.h header file and implemented here
static int simpleOscilCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
  // cast last userdata to our table increment pointer
  simpleOscil *data = (simpleOscil*)userData;

  // create a local pointer to the output buffer
  float *out = outputBuffer;

  // loop through the buffersize and write out values from our table
  for(int i=0; i<framesPerBuffer; i++) {
    out[i] = data->tables[data->table_num][(int)data->table_index];
    data->table_index += data->table_inc;
    if(data->table_index > TABLE_LENGTH) data->table_index -= TABLE_LENGTH;
  }
  return 0;
}


// function prototypes
void usage();
void create_waveshapes    (simpleOscil *data);
void extract_data         (simpleOscil *data, char **argv);
int  generate             (simpleOscil *data);



// usage function
void usage() {
  printf("usage: simple_oscil <waveform> <frequency> <duration>\n");
  printf("(waveforms)\t1 - sine\n");
  printf("\t\t2 - sawtooth\n");
  printf("\t\t3 - triangle\n");
  printf("\t\t4 - square\n");
}

// create waveshapes using the functions defined in the simple_waveshapes library
void create_waveshapes(simpleOscil *data) {
  // allocate space for the pointers to each array
  // data->tables => float **;
  data->tables = (float**)malloc(sizeof(float*)*MAX_TABLE);

  // allocate space for the arrays themselves
  for(int i = 0; i < MAX_TABLE; i++) {
    data->tables[i] = (float*)malloc(sizeof(float)*TABLE_LENGTH);
  }

  // now generate the waveforms within the data structure//
  create_wave_sine(data->tables[0], TABLE_LENGTH);
  create_wave_saw(data->tables[1], TABLE_LENGTH);
  create_wave_triangle(data->tables[2], TABLE_LENGTH);
  create_wave_square(data->tables[3], TABLE_LENGTH);
}

// this function simply initializes portaudio, creates a stream and registers our 
// callback function, and starts processing. after a delay corresponding to our 
// duration,  playback is stopped, the stream is closed and portaudio is terminated. 

// a good portion of this function was simply copied and tweaked from the portaudio
// test/example code, written by Phil Burk and Ross Bencina. 
int generate(simpleOscil *data)
{
  // set up the data structure //  

  // since we are starting, the current index = 0;
  data->table_index = 0.0;

  // create the portaudio stream and start processing //
  PaStream *stream; PaError err;
    
  printf("simple_oscil: starting audio.\n");

  // Initialize library before making any other calls
  err = Pa_Initialize();
  if( err != paNoError ) goto error;
    
  // Open an audio I/O stream
  err = Pa_OpenDefaultStream( &stream,
			      0,          // no input channels
			      1,          // mono output
			      paFloat32,  // 32 bit floating point output
			      SAMPLE_RATE,
			      256,        // frames per buffer
			      simpleOscilCallback,
			      data);
  if( err != paNoError ) goto error;

  err = Pa_StartStream( stream );
  if( err != paNoError ) goto error;

  // Sleep for several seconds - trick to get audio to play for x seconds
  printf("simple_oscil: sleeping for %d seconds.\n", data->duration);
  Pa_Sleep(data->duration*1000);

  err = Pa_StopStream( stream );
  if( err != paNoError ) goto error;
  err = Pa_CloseStream( stream );
  if( err != paNoError ) goto error;
  Pa_Terminate();
  printf("simple_oscil: all done.\n");
  return err;

 error:
  Pa_Terminate();
  fprintf( stderr, "An error occured while using the portaudio stream\n" );
  fprintf( stderr, "Error number: %d\n", err );
  fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
  return err;

}

// simple user interaction function that queries input from the user until they provide an acceptable value, 
// defined by the ranges set above. 

void extract_data(simpleOscil *data, char **argv) {
  int itemp = atoi(argv[1]);
  if(itemp < MIN_TABLE || itemp > MAX_TABLE) {
    printf("Error: enter a valid wave shape (1=sin, 2=sawtooth, 3=triangle)\n");
    exit(1);
  } else {
    data->table_num = itemp-1;
  }

  float ftemp = atof(argv[2]);
  if(ftemp < MIN_FREQ || ftemp > MAX_FREQ) {
    printf("Error: enter a frequency between %.1f and %.1fHz\n", MIN_FREQ, MAX_FREQ);
    exit(2);
  } else {
    // sample inc equation for table lookup oscillator:
    // inc = freq * tablesize/sample rate
    data->table_inc = ftemp * TABLE_LENGTH/SAMPLE_RATE;
  }
  
  ftemp = atof(argv[3]);
  if(ftemp < MIN_DUR || ftemp > MAX_DUR) {
    printf("Error: enter a duration between %.1f and %.1f sec\n", MIN_DUR, MAX_DUR);
    exit(3);
  } else {
    data->duration = ftemp;
  }
}


int main(int argc, char *argv[])
{
  simpleOscil data; // data structure that will contain the defined waveshapes and be passed to portaudio
  
  if(argc < 4) { 
    usage();
    return 4;
  }
  
  // pull out user data for frequency, duration and waveshape
  extract_data(&data, argv);

  // construct the waveform tables
  // pass the address of strcture as an argument so we can modify all aspects of it from another function
  create_waveshapes(&data);

  // generate audio
  generate(&data);
  return 0;
}


/* metronome.c */

/* generates sequence of pulses at given bpm */

/* usage: metronome bpm nbeats */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#ifndef M_PI
#define M_PI (3.141592654)
#endif

int MIN_BPM = 32.0;
int MAX_BPM =  220.0; 
double SAMP_RATE =  1000.0;
double FREQ = 440.0; 
double ALPHA = .008;
enum {ARG_NAME, ARG_BPM, ARG_NBEATS, ARG_NARGS};

int main(int argc, char** argv)
{
  double dur; /* duration of a beat */  
  double T;
  int bpm, nbeats, nsamps;
  double a, k, x, samp;
  double twopi = 2.0 * M_PI;
  double angleincr;

  

  if(argc != ARG_NARGS) {
   fprintf(stderr, "usage: metronome bpm nbeats\n");
   return 1;
  }

  bpm = atoi(argv[ARG_BPM]);
  nbeats = atoi(argv[ARG_NBEATS]);

  
  if(bpm < MIN_BPM || 
     bpm > MAX_BPM) {
    fprintf(stderr, "BPM value must be betweed %d and %d\n",
        MIN_BPM, MAX_BPM);
    return 1;
  }

  if(nbeats < 0) {
    fprintf(stderr, "nbeats must be positive number.\n");
    return 1;
  }
  dur = 60.0 / bpm; 
  T = (-dur) / log(ALPHA); /* neg sign needed sin for alpha < 1, ln(alpha) is neg */
  nsamps = (int)(dur * SAMP_RATE);
  angleincr = twopi*FREQ/SAMP_RATE;
  k = dur/nsamps;
  a = exp(-k/T);
  for (int i=0; i<nbeats; i++) {
    x = 1.0;
    for (int j=0; j<nsamps; j++) {
      samp = sin(angleincr*j);
      x *= a;
      samp *= x;
      fprintf(stdout, "%.8lf\n",samp);
    }
  }
  fprintf(stderr,"done\n");
  return 0;
}

/* we need the duration of the beat to be less than 60 /  MAX_BPM
 * eg if MAX_BPM = 180, then dur dur < 1/3 seconds.  
 *
 * we need the beat to decay quickly enough so that it is barely 
 * audible before the next beat starts.
 * 
 * set the lower threshold leval alpha, then
 *
 *
 *  
 * then we solve for dur:
 * exp(-dur/T) < alpha => T < ln(alpha) / dur
 */


/* tfork.c virtual tuning fork combining sinetext.c and expdecay.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI (3.141592654)
#endif

enum {ARG_NAME,ARG_OUTFILE,ARG_DUR,ARG_HZ,
  ARG_SR,ARG_SLOPE,ARG_NARGS};

int main(int argc, char** argv)
{
  int i,sr,nsamps;
  double samp,dur,freq,srate,k,a,x,slope;
  double twopi = 2.0 * M_PI;
  double angleincr;
  
  double maxsamp = 0.0;
  FILE* fp = NULL;

  if(argc != ARG_NARGS){
    printf("Usage: tfork outfile.txt dur freq srate slope\n");
      return 1;
  }

  fp = fopen(argv[ARG_OUTFILE],"w");
  if(fp==NULL){
    fprintf(stderr, "Error creating output file %s\n",
        argv[ARG_OUTFILE]);
    return 1;
  }

  dur = atof(argv[ARG_DUR]);
  freq = atof(argv[ARG_HZ]);
  srate = atof(argv[ARG_SR]);
  slope = atof(argv[ARG_SLOPE]);
  nsamps = (int)(dur * srate);
  /* 2pi / (period * srate) = 2pi / (# samples per waveform) */
  angleincr = twopi * freq / srate; 
  k = dur/nsamps; /* time increment between samples */
  a = exp(-k/slope);
  /* the higher the slope, the slower the decay (for positive slope). */
  x = 1.0;

  for(i=0; i<nsamps; i++) {
    samp = sin(angleincr*i); 
  /* 0 <= samp < angleincr*nsamps = 2pi * nsamps / (period * srate)
   * = 2pi * (dur*srate) / (period * srate) = 2pi * dur / period
   * = 2pi * # of wave forms
   */
   /* apply exp decay */
    x *= a;
    samp *= x;
    fprintf(fp,"%.8lf\n",samp);
  }
  fclose(fp);
  printf("done\n");
  return 0;
}



  


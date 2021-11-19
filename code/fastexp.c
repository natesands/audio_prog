/* Fast Exponential Envelope Generator */
/* A numerical way to compute an exponential
 * envelope that is less costly than using exp()
 /* fastexp.c from musicdsp.org */

/* NOT SURE THIS DOES WHAT IT IS SUPPOSED TO DO */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
const float sampleRate = 44100;
float coeff;
float currentLevel;
void init(float levelBegin, float levelEnd, float releaseTime);
void calculateEnvelope(int samplePoints,void (*pfun)(int n,float val));
void printVals(int n,float val);

void init(float levelBegin, float levelEnd, float releaseTime) {
  currentLevel = levelBegin;
  coeff = 1.0f + (log(levelEnd) - log(levelBegin)) /
    (releaseTime * sampleRate);
}

inline void calculateEnvelope(int samplePoints,void (*pfun)(int n,float val)) {
  for (int i=0; i<samplePoints; i++) {
    currentLevel *= coeff;
    pfun(i,currentLevel);
  }
}

inline void printVals(int n, float val) {
  fprintf(stdout, "%d\t%.8lf\n", n, val);
}

int main(int argc, char** argv)
{
  void (*pfun) = printVals;
  float levelBegin, levelEnd, releaseTime;
  int npoints;
  if (argc != 5) {
    fprintf(stderr, "Usage: fastexp npoints levelBegin levelEnd releaseTime\n");
    return 1;
  }
  npoints = atoi(argv[1]);
  levelBegin = atof(argv[2]);
  levelEnd = atof(argv[3]);
  releaseTime = atof(argv[4]);
  
  init(levelBegin, levelEnd, releaseTime);
  calculateEnvelope(npoints, pfun);

  return 0;
}






/* peakbrk.c */

/* generates breakpoints for a two-segment attack-decay
 * shape, where the peak-value and the segment durations 
 * are user-controllable.
 */

/* usage: peakbrk dur1 dur2 peak npoints */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char** argv) 
{
  double dur1, dur2, peak,t, dt, m1, m2, current_level;
  int npoints;

  if (argc != 5) {
    fprintf(stderr,"Error.  Usage: peakbrk dur1 dur2 peak npoints\n");
    return 1;
  } 

  dur1 = atof(argv[1]);
  dur2 = atof(argv[2]);
  peak = atof(argv[3]);
  npoints = atoi(argv[4]);

  dt = (dur1 + dur2) / npoints;
  m1 = peak / dur1;
  m2 = (-peak) / dur2;

  t = 0.0;
  current_level = 0.0;

  /* print first segment values */
  while (t < dur1) {
    printf("%.4lf\t%.8lf\n",t,current_level);
    t += dt;
    current_level += dt*m1;
  }
 // assert: current_level >= peak
  
  /* print peak point */
  printf("%.4lf\t%.8lf\n",dur1,peak);

  /* make adjustments for new segment */
  if (t == dur1) t += dt;
  current_level = peak + (t - dur1) * m2;

  /* print second segment values */
  while (t <= dur1 + dur2) {
    printf("%.4lf\t%.8lf\n",t,current_level);
    t += dt;
    current_level += dt*m2;
  }

  /* print final value */
  printf("%.4lf\t%.8lf\n",t,current_level);
  return 0;
}

/* REMEMBER:  DO NOT LOOP OVER A FLOAT INDEX!! */


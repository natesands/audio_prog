/* sinetext2.c */

/* Note:
 * The period T of a wave with frequency freq is (1/freq). 
 * Let srate = sampling rate (datapoints / second) 
 * Let nsamps = # of samples made in a complete cycle.
 * Then:
 *  T*srate = (1/freq) * srate = nsamps
 *  freq =  srate / nsamps
 *  srate = freq / nsamps 
 *  nsamps = freq / srate
 *  Each sample corresponds to a change in radian measure 
 *  angleincr = 2*pi / nsamps = 2*pi*(nsamps/freq)
 *  
 *  usage:
 *  sintext2 nsamps freq srate
 *  
 *  prints two columns of data, corresponding to the value samp
 *  of a given sample point, and samp^2 (for channel 1 and channel 2)
 */

 /*  e.g.
      sintext 100 30 3000
    1 +--------------------------------------------------------------------+
      |      +    *##     ##**    +      +     +      ##     ##     +      |
      |         **#         ##**                    ##out" usin# 1 ******* |
      |       **##           ## *                  ##"out" using## ####### |
      |      ** #              # **               #              #         |
  0.5 |-+   * ##                # **             #                ##     +-|
      |   **  #                  #  *           #                  #       |
      |  *  ##                    #  *         #                    ##     |
      | ** ##                      ## *      ##                      ##    |
      |* ##                         ###**  ###                         ##  |
    0 |##                             ######                             ##|
      |                                  **                               *|
      |                                    *                            ** |
      |                                     *                           *  |
      |                                      *                        **   |
 -0.5 |-+                                     **                     *   +-|
      |                                        **                  **      |
      |                                          *                **       |
      |                                           ***           **         |
      |      +      +      +      +      +     +    ***      +**    +      |
   -1 +--------------------------------------------------------------------+
      0      10     20     30     40     50    60     70     80     90    100

  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI (3.141592654)
#endif

/* define our program argument list */

enum {ARG_NAME,ARG_NSAMPS,ARG_FREQ,ARG_SR,ARG_NARGS};

int main(int argc, char** argv)
{
  int i,nsamps;
  double samp,freq,srate;
  double twopi=2*M_PI;
  double angleincr;
  if(argc!=ARG_NARGS){
    fprintf(stderr,"Usage: sinetext2 nsamps freq srate\n");
    return 1;
    }
  nsamps = atoi(argv[ARG_NSAMPS]);
  freq = atof(argv[ARG_FREQ]);
  srate = atof(argv[ARG_SR]);
  angleincr = twopi*freq/srate;

  for (i=0; i<nsamps; i++){
    samp = sin(angleincr*i);
    /* create two columns of data for channels 1 and 2 */
    // fprintf(stdout,"%.8lf\t%.8lf\n",samp,samp*samp);
    fprintf(stdout, "%.8lf\n", samp);
  }

  fprintf(stderr,"done.\n");
  return 0;
}


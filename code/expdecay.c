/* expdecay.c */

/* calculates values of 
 * x = N_0*e^(-kt)
 * over a specified time interval 
 * and a specified number of steps:
 *
 *  expdecay [time interval] [num points] [k value] [N_0 value] [tau] (optional
 *  txt file for output)
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  double dur, k, N0, tau, t,ratio,dt, val;
  FILE* fp = NULL;
  int points = 0;
  
    if (argc < 6) {
      printf("usage: expdecay dur points k N0 tau (file_name)\n");
      return 1;
    }
  dur = atof(argv[1]);
  points = atoi(argv[2]);
  k = atof(argv[3]);
  N0 = atof(argv[4]);
  tau = atof(argv[5]);
  dt = dur / points; 
  ratio = exp(-k*dt/tau); 
  val = N0;
  t = 0;
  
  if (argc == 7) {
    fp = fopen(argv[6], "w");
    if (fp == NULL) {
      printf("Unable to open file %s\n", argv[6]);
      return 1;
    }
    for (int i=0; i<points; i++) {
      fprintf(fp, "%lf\t%lf\n", t, val);
      t += dt;
      val *= ratio; 
    }
    fclose(fp);
    return 0;
  }
  else
    for (int i=0; i<points; i++) {
      printf("%lf\t%lf\n", t, val);
      t += dt;
      val *= ratio; 
    }
}



   
  
      
      
  

  

    

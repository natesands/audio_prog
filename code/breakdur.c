#include <stdio.h>
#include <stdlib.h>
typedef struct {
  double time;
  double value;
} BREAKPOINT;

BREAKPOINT maxpoint(const BREAKPOINT* points, int npoints);
BREAKPOINT* get_breakpoints(FILE* fp, int* psize);
void scale_values(BREAKPOINT* bp, int npoints, double sc); 
void shift_values(BREAKPOINT* bp, int npoints, double shift);
double scale_duration(BREAKPOINT* bp, int npoints, double sc); 
BREAKPOINT* insert_breakpoint(BREAKPOINT* bp, int *npoints, double t, double val); 
int breakpoints_to_file(BREAKPOINT* bp, char* file_name, int npoints);
void print_breakpoints(BREAKPOINT* bp, int npoints);

void shift_values(BREAKPOINT* bp, int npoints, double shift)
  //applies shift to all breakpoint values
{
  for (int i=0;i<npoints;i++)
   bp[i].value += shift;
}

double scale_duration(BREAKPOINT* bp, int npoints, double sc) 
  // scales all time values; returns new duration of envelope
{
  for (int i=0; i<npoints; i++)
    bp[i].time *= sc;
  
  return bp[npoints-1].time;
}

BREAKPOINT* insert_breakpoint(BREAKPOINT* bp, int *npoints, double t, double val)
// inserts breakpoint (t,val).  if point is already present at t, then
// replace old val with new.
// pre:  bp points to a memory block with at least two breakpoints
{
  // confirm that (t,val) is legal
  if (t < 0.0) {
    printf("error: time must be positive value\n");
    return bp;
  } 

  // check for value at time t; if it exists, replace w/ new value
  for (int i=0; i < *npoints; i++)
    if (bp[i].time == t) {
     bp[i].value=val;
     return bp;
    }

  //assert: t is a new breakpoint; allocate extra memory location
  BREAKPOINT* tmp;
  
  tmp = (BREAKPOINT*) realloc(bp,sizeof(BREAKPOINT)*(*npoints+1));
  if (tmp==NULL) {
    printf("unable to add point (%lf,%lf)", t,val);
    return bp; // assuming that the block pointed to by bp has not been
               // freed?  
  }
  
  int i = *npoints - 1;
  while (t < tmp[i].time && i >= 0)
    i--;
  
  // tmp[i].time < t
  // shift values and add (t,val) to i+1
  for (int j=*npoints-1; j>i; j--)
    tmp[j+1] = tmp[j];
  tmp[i+1].time=t;
  tmp[i+1].value=val;
  // account for extra point
  *npoints += 1;
  return tmp; 
   
}
  
void scale_values(BREAKPOINT* bp, int npoints, double sc)
{
  for(int i=0; i<npoints;i++)
    bp[i].value *= sc;
}


int breakpoints_to_file(BREAKPOINT* bp, char* file_name, int npoints) 
{
  FILE* fp;
  int err;
  fp = fopen(file_name,"w");
  if (fp==NULL)
    return -1;
  for (int i=0; i<npoints; i++) {
    err = fprintf(fp,"%lf\t%lf\n",
        bp[i].time, bp[i].value);
    if (err<0)
      printf("error writing line %d\n", i);
  }    
  if(fp) 
    fclose(fp);
  return 0;
}

void print_breakpoints(BREAKPOINT* bp, int npoints) {
  for (int i = 0; i < npoints; i++)
    printf("%lf\t%lf\n", bp[i].time, bp[i].value);
}

BREAKPOINT maxpoint(const BREAKPOINT* points, int npoints)
/* input: array of breakpoints, with size npoints
 * output: copy of BREAKPOINT containing the largest value;
 */
{
  int i;
  BREAKPOINT point;

  point.time=points[0].time;
  point.value=points[0].value;

  for(i=0; i<npoints; i++) {
    if(point.value < points[i].value) {
      point.value = points[i].value;
      point.time = points[i].time;
    }
  }
  return point;
}

BREAKPOINT* get_breakpoints(FILE* fp, int* psize)
/* returns pointer to array of breakpoints, set to NULL if 
 * there was an error.  int pointed to by psize will hold size
 * of array
 */
{
#define NPOINTS 64
  int got;
  int npoints=0, size=NPOINTS; // initial number of breakpoints allotted
  double lasttime = 0.0;
  BREAKPOINT* points = NULL;
  char line[80];  //C-string to hold line

  if(fp==NULL)  // error on file read
    return NULL;
  points = (BREAKPOINT*) malloc(sizeof(BREAKPOINT)*size); 
  if(points == NULL)  // memory allocation error
    return NULL;

  while(fgets(line,80,fp)) 
  // reads line in file up to (80-1) chars 
  { 
    got = sscanf(line, "%lf%lf",       // values are read into struct
        &points[npoints].time, &points[npoints].value);
    if(got<0)
      continue; // empty line; restart loop
    if(got==0) {
      printf("Line %d has non-numeric data\n",npoints+1);
      break;
    }
    if(got==1) {
      printf("Incomplete breakpoint found at point %d\n",
          npoints+1);
      break;
    }
    if(points[npoints].time < lasttime) {
      printf("data error at point %d: time not increasing\n",
          npoints+1);
      break;
    }
    lasttime = points[npoints].time;
    if(++npoints == size) {
      BREAKPOINT* tmp;
      size += NPOINTS;
      tmp=(BREAKPOINT*)realloc(points,sizeof(BREAKPOINT)*size);
      if(tmp == NULL) { // unable to allocate memory
        npoints=0;
        free(points);
        points = NULL;
        break;
      }
      points = tmp;
    }
  }
  if(npoints) // npoints is non-zero
    *psize=npoints;
  return points;
}

int main(int argc, char* argv[])
{
  int size, err;
  double dur;
  BREAKPOINT point, *points;
  FILE* fp;

  printf("breakdur: find duration of breakpoint file\n");
  if(argc < 2){
    printf("usage: breakdur infile.text \n");
    return 0;
  }
  fp=fopen(argv[1],"r");
  if(fp == NULL) {
    printf("open failed.");
    return 0;
  }
  size =0;
  points = get_breakpoints(fp,&size);
  if(points==NULL){
    printf("No breakpoints read.\n");
    fclose(fp);
    return 1;
  }
  if(size<2) {
    printf("Error: at least two breakpoints required\n");
    free(points);
    fclose(fp);
    return 1;
  }
  /* we require breakpoints to start from 0 */
  if(points[0].time != 0.0) {
    printf("Error in breakpoint data: first time must be 0.0\n");
    free(points);
    fclose(fp);
    return 1;
  }
  printf("read %d breakpoints\n",size);
  dur = points[size-1].time;
  printf("duration: %f seconds\n", dur);
  point = maxpoint(points,size);
  printf("maximum value: %f at %f secs\n", point.value,point.time);
  printf("************************\n");
  print_breakpoints(points, size);
  printf("************************\n");
  scale_values(points,size,2.0); 
  print_breakpoints(points, size);
  printf("************************\n");
  points = insert_breakpoint(points, &size, 1.0, .6);
  print_breakpoints(points,size);
  printf("************************\n");
  scale_duration(points,size,2.0); 
  print_breakpoints(points, size);
  err = breakpoints_to_file(points,"output_breakpoints.brk",size);
  if (err < 0) 
    printf("Error writing file\n");
  free(points);
  fclose(fp);
  return 0;
}



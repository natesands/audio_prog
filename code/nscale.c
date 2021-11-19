/* nscale.c.  Display equally tempered (ET) frequencies for an 
 * N-note octave, from a given MIDI note.  Syntax:
 *  nscale notes midinote
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[])
{
  int notes, midinote;
  double frequency, ratio;
  double c0, c5;
  double interval[24];

  if (argc != 3) {
    printf("usage: nscale notes midinote\n");
    return 1;
  }
  notes = atoi(argv[1]);
  /* atoi has syntax: int atoi(const char *str)
   * parameter must be a c-string (i.e. a char* )
   */
  if(notes > 24) {
    printf("Error: max value for notes in 24\n");
    return 1;
  }
  if (notes < 1) {
    printf("Error: notes must be positive.\n");
    return 1;
  }

  midinote = atoi(argv[2]);
  if (midinote < 0 || midinote > 127) {
    printf("Error: cannot MIDI notes must be in range 0 to 127\n");
    return 1;
  }
  /* calc standard ET semitone ratio */
  ratio = pow(2.0, 1.0/12.0);
  /* find Middle C, three semitones about low A = 220 */
  c5 = 220.0 * pow(ratio,3);
  /* MIDI note 0 is C, 5 octaves below Middle C */
  c0 = c5 * pow(.5,5);
  frequency = c0 * pow(ratio, midinote);

  /* calc ratio from notes, and fill the freqency array */
  ratio = pow(2.0,1.0/notes);
  for (int i=0; i < notes; i++) {
    interval[i]=frequency;
    frequency *= ratio;
  }
  /* read array and write to screen */
  for (int i=0;i<notes;i++){
    printf("%d: %f\n", i,interval[i]);
  }
  return 0;

}


/* take one-second extract from a two-channel soundfile
 * sr = 44100 and split into two separate files.
 *
 * There are two samples per frame, each a 16-bit (short) integer.
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
  FILE *fpin, *fpleft, *fpright;
  short *audioblock, *left, *right;

  int frames, samples, end, i, j;
  int sr = 44100;
  int blockframes = 100, blocksamples;
  int channels = 2;
  blocksamples = blockframes*channels;
  end = sr;

  fpin = fopen("soundfile", "rb");
  fpleft = fopen("left_channel", "wb");
  fpright = fopen("right_channel", "wb");

  audioblock = (short *) malloc(sizeof(short)*blocksamples);
  left = (short *) malloc(sizeof(short)*blockframes);
  right = (short *) malloc(sizeof(short)*blockframes);

  for(i=0; i < end && !feof(fpin); i+=blockframes) {
    samples = fread(audioblock, sizeof(short), blocksamples);
    frames = samples / 2;

    for (j=0; j < frames; j++) {
      left[j] = audioblock[j*2];
      right[j] = audioblock[j*2+1];
    }

    fwrite(left,sizeof(short), frames, fpleft);
    fwrite(right,sizeof(short), frames, fpright);
  }
  
  free(audioblock);
  free(left);
  free(right);
  fclose(fpleft);
  fclose(fpright);
  fclose(fpin);

  return 0;
}



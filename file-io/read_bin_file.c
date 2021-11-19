/* open and read a binary file containing PCM 
 * audio samples, stored as 16-bit (short) 
 * integers.
 *
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
  FILE *fpin;
  short *audioblock;
  int samples = 0;

  fpin = fopen("soundfile", "rb");
  audioblock = (short *) malloc(sizeof(short)*256);

  while(!feof(fpin))
    samples += fread(audioblock, sizeof(short), 256, fpin);

  printf("%d samples read from file", samples);

  free(audioblock);
  fclose(fpin);

  return 0
}


  /*
   * fread() and fwrite() are used to read/write any type of
   * data, using any kind of representation.
   *
   * size_t fread(void *buffer, size_t size, size_t num, FILE *fp);
   *
   * size_t fwrite(void *buffer, size_t size, size_t num, FILE *fp);
   *
   * fread() reads num items from file fp, each item size bytes long,
   * into a buffer pointed to by buffer.  It returns the number actually
   * read.
   *
   * fwrite() writes num items to file fp each size bytes long from
   * the buffer pointed to by buffer.  returns number of items
   * written.
   *
   * can be used in conjunction with int feof(FILE *fp) and 
   * int ferror(FILE *fp).
   */



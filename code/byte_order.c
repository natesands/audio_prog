#include <stdio.h>



int byte_order()
{
  int one=1;
  char* endptr = (char *) &one;
  return (*endptr);
}

const char* endianness[2] = {"big_endian","little_endian"};

int main()
{
  printf("%s",endianness[byte_order()]);
  return 0;
}


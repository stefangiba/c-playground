#include <stdio.h>

int main(int argc, char *argv[]) {
  char c;          // 1 byte, 8 bits
  short s;         // 2 bytes, 16 bits
  int i;           // 4 bytes, 32 bits
  long long int l; // 8 bytes, 64 bits
  unsigned int ui = -4;
  float f = 3.14;
  double d = 3.14159;

  unsigned int x = 128;
  int sx = (int)x;
  printf("%d\n", sx);

  unsigned int bigx = 0xfffffefe;
  short kindabig = (short)bigx;
  printf("%d\n", bigx);
  printf("%d\n", kindabig);

  short short2 = -1;
  int wasashort = (int)short2;
  printf("%d\n", wasashort);

  float f2 = 3.14;
  int wasafloat = (int)f2;
  printf("%d\n", wasafloat);

  short *mydata = (short *)argv[1];
  printf("%d\n", argc);
  printf("%d\n", *mydata);

  return 0;
}
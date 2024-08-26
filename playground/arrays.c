#include <stdio.h>

#define MAX_IDS 32

int main() {
  int ids[MAX_IDS] = {0, 1, 2};

  printf("%d\n", ids[2]);

  ids[3] = 0x41;

  printf("%d\n", ids[3]);

  // Strings in C are null terminated, meaning that they need to end in a null
  // byte
  char mystr[] = {'h', 'e', 'l', 'l', 'o', '\0'};

  // when a String is put between double quotes, the compiler knows to null
  // terminate this by default
  char *myotherstr = "hello";
  char myotherotherstr[] = "hello";

  printf("%s\n", mystr);
  printf("%s\n", myotherstr);
  printf("%s\n", myotherotherstr);

  // copy source into dest
  // strcpy(dest, src);

  // copies up to n bytes from source to dest
  // strncpy(dest, src, n);

  return 0;
}
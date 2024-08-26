#include <stdio.h>

// return_type function_name(list of arguments) {
//
//}

int add(int a, int b) { return a + b; }

int main(int argc, char *argv[]) {
  int other_int = add(1, 2);
  printf("%d\n", other_int);

  return 0;
}

#include <stdio.h>

#define MAX_IDS 32

int main() {
  printf("What temperature is it?\n");

  int temp;
  scanf("%d", &temp);

  if (temp >= 70) {
    printf("It's hot!\n");
  } else if (temp >= 30 && temp < 70) {
    printf("It's mild!\n");
  } else {
    printf("It's cold!\n");
  }

  int ids[MAX_IDS] = {0};

  for (int i = 0; i < MAX_IDS; i++) {
    ids[i] = i * i;
    printf("%d -> %d\n", i, ids[i]);
  }

  int i = 0;
  while (i < MAX_IDS) {
    ids[i] = i * i;
    printf("%d -> %d\n", i, ids[i]);
    i++;
  }

  i = 0;
  do {
    ids[i] = i * i;
    printf("%d -> %d\n", i, ids[i]);
    i++;
  } while (i < MAX_IDS);

  return 0;
}

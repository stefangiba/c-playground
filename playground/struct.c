#include <stdbool.h>
#include <stdio.h>

#define MAX_EMPLOYEES 1000


struct __attribute__((__packed__)) employee_t {
  int id;
  char firstname[64];
  char lastname[64];
  float income;
  bool ismanager;
};

// a union only allocates enough memory for the biggest element
// the union will be "int x" wide
union myunion_u {
  int x;
  char c;
  short s;
};

int main() {
  struct employee_t employees[MAX_EMPLOYEES];

  for (int i = 0; i < MAX_EMPLOYEES; i++) {
    employees[i].income = 0;
    employees[i].ismanager = false;
  }

  printf("%f\n", employees[10].income);

  union myunion_u u;
  u.x = 0x41424344;

  printf("%hx %hhx\n", u.s, u.c);

  printf("Size of employee: %d\n", sizeof(struct employee_t));

  return 0;
}
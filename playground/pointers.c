#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_EMPLOYEES 1000

struct employee_t {
  int id;
  int income;
  bool staff;
};

int initialize_employee(struct employee_t *e) {
  // STATIC MEMORY ALLOCATIION

  // A static variable is a variable that exists in a static lifetime,
  // but does not have global scope.By creating a static variable,
  // we are able to create variables that out live the lifetime of a
  // function call and track data over multiple calls to the same function.\
  //
  // This variable is exists in a static lifetime, but does not
  // have global scope. it will outlive the lifetime of a function call
  // and track data over multiple calls to the same function
  //
  // It will be stored into the data section of the running program
  static int numEmployees = 0;

  e->id = numEmployees;
  e->income = 0;
  e->staff = false;

  numEmployees++;

  return e->id;
}

// a pointer is just a memory address;
int main() {
  int x = 3;
  // create an integer pointer p_x and
  // set its value to the address of x
  int *p_x = &x;

  // dereference p_x => value
  printf("%d\n", *p_x);
  printf("%p\n", p_x);

  // setting the default value for Ralph
  struct employee_t Ralph;
  initialize_employee(&Ralph);

  printf("%d\n", Ralph.id);

  // DYNAMIC MEMORY ALLOCATION

  // this value came from the database header
  int n = 4;
  struct employee_t *employees = malloc(sizeof(struct employee_t) * n);

  if (employees == NULL) {
    printf("The allocator failed\n");
    return -1;
  }

  initialize_employee(&employees[0]);
  printf("%d\n", employees[0].id);

  for (int i = 0; i < n; i++) {
    int id = initialize_employee(&employees[i]);
    printf("New employee, ID is %d\n", id);
  }

  free(employees);
  employees = NULL;

  return 0;
}
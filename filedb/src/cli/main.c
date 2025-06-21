#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "lib/file.h"
#include "lib/parse.h"

void print_usage(char *argv[]) {
  printf("Usage:  %s -n -f <database file>", argv[0]);
  printf("\t -f <file_path> - (required) create new database file");
  printf("\t -n  - create new database file");
  printf(
      "\t -a <name,address,number_of_hours> - add an employee to the database");
  printf("\t -l  - list all employees in the database");
  printf(
      "\t -r <name>  - remove all employees with a specific name from the "
      "database");
  printf(
      "\t -u <name,number_of_hours> - update the number of hours employees "
      "with a specific name");
}

int main(int argc, char *argv[]) {
  char *file_path = NULL, *add_string = NULL, *remove_string = NULL,
       *update_string = NULL;
  bool new_file = false, list = false;
  int c;

  while ((c = getopt(argc, argv, "nf:a:lr:u:")) != -1) {
    switch (c) {
      case 'n':
        new_file = true;
        break;
      case 'f':
        file_path = optarg;
        break;
      case 'a':
        add_string = optarg;
        break;
      case 'l':
        list = true;
        break;
      case 'r':
        remove_string = optarg;
        break;
      case 'u':
        update_string = optarg;
        break;
      case '?':
        printf("Unknown option -%c\n", c);
        break;
      default:
        print_usage(argv);
        return -1;
    }
  }

  if (file_path == NULL) {
    printf("Filepath is a required argument\n");
    print_usage(argv);
    return 0;
  }

  int db_file_descriptor;
  struct DbHeader *db_header = NULL;
  if (new_file) {
    db_file_descriptor = create_db_file(file_path);

    if (db_file_descriptor == STATUS_ERROR) {
      printf("Unable to create database file!\n");
      return -1;
    }

    if (create_db_header(db_file_descriptor, &db_header) != STATUS_SUCCESS) {
      printf("Failed to create database header!\n");
      close(db_file_descriptor);
      return -1;
    }
  } else {
    db_file_descriptor = open_db_file(file_path);

    if (db_file_descriptor == STATUS_ERROR) {
      printf("Unable to open database file!\n");
      return -1;
    }

    if (validate_db_header(db_file_descriptor, &db_header) != STATUS_SUCCESS) {
      printf("Invalid database header!\n");
      close(db_file_descriptor);

      return -1;
    }
  }

  struct Employee *employees = NULL;
  if (read_employees(db_file_descriptor, db_header, &employees) !=
      STATUS_SUCCESS) {
    printf("Failed to read employees!\n");
    close(db_file_descriptor);

    return -1;
  }

  if (add_string != NULL) {
    if (add_employee(db_header, &employees, add_string) != STATUS_SUCCESS) {
      printf("Failed to add employee!\n");
      free(employees);

      return -1;
    }
  }

  if (update_string) {
    if (update_employees(db_header, employees, update_string) !=
        STATUS_SUCCESS) {
      printf("Failed to update employees!\n");
      free(employees);
      close(db_file_descriptor);

      return -1;
    }
  }

  if (remove_string != NULL) {
    if (remove_employees(db_header, &employees, remove_string) !=
        STATUS_SUCCESS) {
      printf("Failed to remove employee(s) with name: %s!\n", remove_string);
      free(employees);
      close(db_file_descriptor);

      return -1;
    }
  }

  if (list) {
    list_employees(db_header, employees);
  }

  if (output_file(db_file_descriptor, db_header, employees) != STATUS_SUCCESS) {
    printf("Failed to write to file!\n");
    free(employees);
    close(db_file_descriptor);

    return -1;
  }

  return 0;
}

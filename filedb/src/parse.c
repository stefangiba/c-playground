#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "common.h"

int create_db_header(int fd, struct DbHeader **header_out) {
  struct DbHeader *header = calloc(1, sizeof(struct DbHeader));

  if (header == NULL) {
    printf("Malloc failed to create database header!\n");
    return STATUS_ERROR;
  }

  header->version = 0x1;
  header->count = 0;
  header->magic = HEADER_MAGIC;
  header->file_size = sizeof(struct DbHeader);

  *header_out = header;

  return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct DbHeader **header_out) {
  if (fd < 0) {
    printf("Invalid file descriptor!\n");
    return STATUS_ERROR;
  }

  struct DbHeader *header = calloc(1, sizeof(struct DbHeader));

  if (header == NULL) {
    printf("Malloc failed to create database header!\n");
    return STATUS_ERROR;
  }

  if (read(fd, header, sizeof(struct DbHeader)) != sizeof(struct DbHeader)) {
    perror("read");
    free(header);
    return STATUS_ERROR;
  }

  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->magic = ntohl(header->magic);
  header->file_size = ntohl(header->file_size);

  if (header->magic != HEADER_MAGIC) {
    printf("Improper header magic!\n");
    free(header);
    return STATUS_ERROR;
  }

  if (header->version != 1) {
    printf("Invalid database version!\n");
    free(header);
    return STATUS_ERROR;
  }

  struct stat db_stat = {0};
  fstat(fd, &db_stat);

  if (header->file_size != db_stat.st_size) {
    printf("Corrupted database!\n");
    free(header);
    return STATUS_ERROR;
  }

  *header_out = header;

  return STATUS_SUCCESS;
}

int read_employees(int fd, struct DbHeader *db_header,
                   struct Employee **employees_out) {
  if (fd < 0) {
    printf("Invalid file descriptor!\n");
    return STATUS_ERROR;
  }

  int count = db_header->count;
  struct Employee *employees = calloc(count, sizeof(struct Employee));

  if (employees == NULL) {
    printf("Malloc failed to create employee array!\n");
    return STATUS_ERROR;
  }

  read(fd, employees, count * sizeof(struct Employee));

  for (int i = 0; i < count; i++) {
    employees[i].hours = ntohl(employees[i].hours);
  }

  *employees_out = employees;

  return STATUS_SUCCESS;
}

int add_employee(struct DbHeader *db_header, struct Employee **employees,
                 char *add_string) {
  db_header->count++;
  struct Employee *temp =
      realloc(*employees, (db_header->count) * sizeof(struct Employee));

  if (temp == NULL) {
    printf("Failed to allocate memory for new employee!\n");
    return STATUS_ERROR;
  }

  *employees = temp;

  char *name = strtok(add_string, ",");
  char *addr = strtok(NULL, ",");
  char *hours_str = strtok(NULL, ",");

  struct Employee *employees_arr = *employees;

  strncpy(employees_arr[db_header->count - 1].name, name,
          sizeof(employees_arr[db_header->count - 1].name));

  strncpy(employees_arr[db_header->count - 1].address, addr,
          sizeof(employees_arr[db_header->count - 1].address));

  employees_arr[db_header->count - 1].hours = atoi(hours_str);

  return STATUS_SUCCESS;
}

int update_employees(struct DbHeader *db_header, struct Employee *employees,
                     char *add_string) {
  char *name = strtok(add_string, ",");
  char *hours_str = strtok(NULL, ",");

  if (name == NULL || hours_str == NULL) {
    printf("Invalid update string!\n");
    return STATUS_ERROR;
  }

  for (int i = 0; i < db_header->count; i++) {
    if (strcmp(employees[i].name, name) == 0) {
      employees[i].hours = atoi(hours_str);
    }
  }

  return STATUS_SUCCESS;
}

int remove_employees(struct DbHeader *db_header, struct Employee **employees,
                     char *remove_string) {
  int num_employees_to_remove = 0;
  for (int i = 0; i < db_header->count; i++) {
    if (strcmp((*employees)[i].name, remove_string) == 0) {
      num_employees_to_remove++;
      continue;
    }

    (*employees)[i - num_employees_to_remove] = (*employees)[i];
  }

  db_header->count -= num_employees_to_remove;
  struct Employee *temp =
      realloc(*employees, db_header->count * sizeof(struct Employee));

  if (temp == NULL) {
    printf("Realloc failed!\n");
    return STATUS_ERROR;
  }

  *employees = temp;

  return STATUS_SUCCESS;
}

void list_employees(struct DbHeader *db_header, struct Employee *employees) {
  for (int i = 0; i < db_header->count; i++) {
    printf("Employee %d\n", i);
    printf("\tName %s\n", employees[i].name);
    printf("\tAddress %s\n", employees[i].address);
    printf("\tHours %d\n", employees[i].hours);
  }
}

int output_file(int fd, struct DbHeader *db_header,
                struct Employee *employees) {
  if (fd < 0) {
    printf("Invalid file descriptor!\n");
    return STATUS_ERROR;
  }

  int real_count = db_header->count;

  db_header->magic = htonl(db_header->magic);
  db_header->version = htons(db_header->version);
  db_header->file_size =
      htonl(sizeof(struct DbHeader) + sizeof(struct Employee) * real_count);
  db_header->count = htons(db_header->count);

  if (lseek(fd, 0, SEEK_SET) == -1) {
    perror("lseek");
    return STATUS_ERROR;
  }

  if (ftruncate(fd, 0) == -1) {
    perror("ftruncate");
    return STATUS_ERROR;
  }

  if (write(fd, db_header, sizeof(struct DbHeader)) == -1) {
    perror("write");
    return STATUS_ERROR;
  }

  for (int i = 0; i < real_count; i++) {
    employees[i].hours = htonl(employees[i].hours);
  }

  if (write(fd, employees, sizeof(struct Employee) * real_count) == -1) {
    perror("write");
    return STATUS_ERROR;
  }

  return STATUS_SUCCESS;
}

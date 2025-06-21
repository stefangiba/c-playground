#ifndef PARSE_H
#define PARSE_H

#define HEADER_MAGIC 0x4c4c4144

struct DbHeader {
  unsigned int magic;
  unsigned short version;
  unsigned short count;
  unsigned int file_size;
};

struct Employee {
  char name[256];
  char address[256];
  unsigned int hours;
};

int create_db_header(int fd, struct DbHeader **header_out);
int validate_db_header(int fd, struct DbHeader **header_out);
int read_employees(int fd, struct DbHeader *db_header,
                   struct Employee **employees_out);
int output_file(int fd, struct DbHeader *db_header, struct Employee *employees);
int add_employee(struct DbHeader *, struct Employee **employees,
                 char *add_string);
int remove_employees(struct DbHeader *db_header, struct Employee **employees,
                     char *remove_string);
int update_employees(struct DbHeader *db_header, struct Employee *employees,
                     char *update_string);
void list_employees(struct DbHeader *db_header, struct Employee *employees);

#endif

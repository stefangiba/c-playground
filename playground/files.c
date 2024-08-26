#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

struct database_header_t {
  unsigned short version;
  unsigned short employees;
  unsigned int filesize;
};

int main(int argc, char *argv[]) {
  // In Linux, every program has three file descriptors open by default
  // 0 = stdin
  // 1 = stdout
  // 2 = stderr

  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return -1;
  }

  int fd = open(argv[1], O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open");
    return -1;
  }

  struct database_header_t head = {0};
  struct stat db_stat = {0};
  if (read(fd, &head, sizeof(head)) != sizeof(head)) {
    perror("read");
    close(fd);
    return -1;
  }

  printf("DB version: %u\n", head.version);
  printf("DB # of employees: %u\n", head.employees);
  printf("DB file length: %u\n", head.filesize);

  if (fstat(fd, &db_stat) < 0) {
    perror("fstat");
    close(fd);
    return -1;
  }

  printf("DB file length, reported by fstat: %lld\n", db_stat.st_size);

  if (db_stat.st_size != head.filesize) {
    printf("GET OUTTA HERE HACKER!\n");
    close(fd);

    return -2;
  }

  close(fd);

  return 0;
}

// this is an example of writing data to a file using its
// file descriptor
void write_to_file(int file_descriptor) {
  char *mydata = "Hello there, file!\n";
  write(file_descriptor, mydata, strlen(mydata));
}
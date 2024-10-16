#include <commons/constants.h>
#include <getopt.h>
#include <server/sync_multiplexed_server.h>
#include <server/sync_server.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 5555
#define BACKLOG 0

void usage(char *argv[]) {
  fprintf(stderr, "usage: %s [--sync | --multiplexed | --async]\n", argv[0]);
}

int main(int argc, char *argv[]) {
  int sync = 0, multiplexed = 0, async = 0;

  /* options descriptor */
  struct option long_opts[] = {{"sync", no_argument, NULL, 's'},
                               {"multiplexed", no_argument, NULL, 'm'},
                               {"async", no_argument, NULL, 'a'},
                               {NULL, 0, NULL, 0}};

  int ch;
  while ((ch = getopt_long(argc, argv, "asm", long_opts, NULL)) != -1) {
    switch (ch) {
      case 'a':
        async = 1;
        break;
      case 's':
        sync = 1;
        break;
      case 'm':
        multiplexed = 1;
        break;
      default:
        usage(argv);
        return STATUS_ERROR;
    }
  }

  if (async) {
    if (sync + multiplexed > 0) {
      fprintf(stderr, "Cannot use --async with --sync or --multiplexed!\n");
      return STATUS_ERROR;
    }

    // TODO: add async multiplexed server!
    fprintf(stderr, "Async server not implemented yet!\n");
    return STATUS_ERROR;
  }

  if (sync) {
    if (multiplexed) {
      printf("Starting sync multiplexed server!\n");

      return run_sync_multiplexed(PORT, BACKLOG);
    }

    return run_sync(PORT, BACKLOG);
  }

  if (multiplexed) {
    fprintf(stderr,
            "Multiplexed is a feature of the sync server. Use the `--sync` "
            "flag as well!\n");
    return STATUS_ERROR;
  }

  printf("No mode option specified, defaulting to SYNC!\n");
  fprintf(stderr, "An option should have been specified!\n");

  return run_sync(PORT, BACKLOG);
}

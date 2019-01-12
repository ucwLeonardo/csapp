#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#include "cachelab.h"

int main(int argc, char *argv[])
{
  int opt, s = 0, E = 0, b = 0, verbose = 0;
  char *trace_file = NULL;

  while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
    switch (opt) {
      case 'h':
        fprintf(stderr, "Usage: %s [-hv] -s <s> -E <E> -b <b> "
        "-t <tracefile>\n", argv[0]);
        return 0;
      case 'v':
        verbose = 1;
        break;
      case 's':
        s = atoi(optarg);
        break;
      case 'E':
        E = atoi(optarg);
        break;
      case 'b':
        b = atoi(optarg);
        break;
      case 't':
        trace_file = optarg;
        break;
      default: /* '?' */
        fprintf(stderr, "Usage: %s [-hv] -s <s> -E <E> -b <b> "
          "-t <tracefile>\n", argv[0]);
        exit(EXIT_FAILURE);
      }
  }

  printf("s: %d, E: %d, b: %d, verbose: %d, tracefile: %s\n",
          s, E, b, verbose, trace_file);
  printSummary(0, 0, 0);
  return 0;
}
#define _GNU_SOURCE
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "cachelab.h"

#define HEX_SIZE(h) (h >> 2)

const int OPT_CHOP = 3;
const int OPT_OFFSET = 1;

int HIT = 0, MISS = 0, EVICTION = 0;
int s = 0, b = 0, S = 0, E = 0, VERBOSE = 0;
char *TRACE_FILE = NULL;

const char hex_in_binary[16][5] = {"0000", "0001", "0010", "0011", "0100", \
  "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", \
  "1110", "1111"};
const char *digits = "0123456789abcdef";

typedef unsigned int ttime;

typedef struct Line {
  int tag;
  ttime time;
} Line;

Line **CACHE;
ttime *MAX_TIME;  // latest time stamp for each set

void print_max() {
  for (int i = 0; i < S; i++)
    printf("%d ", MAX_TIME[i]);
  printf("\n");
}

void hex2binary(char *hex, char *binary, int hex_len) {
  for (int i = 0; i < hex_len; i++) {
    char *c = strchr(digits, hex[i]);
    if (c)
      strcat(binary, hex_in_binary[c - digits]);
  }
}

bool search_cache(int set, int tag, int *first_empty_line) {
  *first_empty_line = -1;
  int temp = - 1;
  // printf("\nMAX_TIME: ");
  // print_max();
  for (int i = 0; i < E; i++) {
    // printf("searching set: %d, line: %d, tag: %d\n", set, i, tag);
    // printf("cache line tag: %d, time: %u\n", CACHE[set][i].tag, CACHE[set][i].time);
    if (CACHE[set][i].time && CACHE[set][i].tag == tag) {
      CACHE[set][i].time = MAX_TIME[set] + 1;
      MAX_TIME[set] += 1;
      return true;
    }
    else if (temp == -1 && CACHE[set][i].time == 0)
      temp = i;
  }
  *first_empty_line = temp;
  // update time
  if (temp > -1) {
    CACHE[set][temp].time = MAX_TIME[set] + 1;
    CACHE[set][temp].tag = tag;
    MAX_TIME[set] += 1;
  }

  MISS++;
  return false;
}

void parse_line(char *str, int *set, int *tag) {
  // copy line
  int line_len = strlen(str) - OPT_CHOP;
  char line[line_len + 1];
  strcpy(line, &str[OPT_CHOP]);
  int tag_temp = 0, set_temp = 0;
  // discard block offset after ',', now only address
  for (int i = 0; i < line_len; i++) {
    if (line[i] == ',') {
      line[i] = '\0';
      break;
    }
  }
  
  int hex_len = strlen(line);
  int binary_len = hex_len << 2;
  char *binary_line = (char *) calloc(binary_len + 1, sizeof(char));
  hex2binary(line, binary_line, hex_len);

  // if addr_len < b, do nothing, set and tag both 0
  if (binary_len > b) {
    int combined_len = binary_len - b;
    binary_line[combined_len] = '\0'; // discard last b bits (block offset)
    if (binary_len <= s + b)
      set_temp = (int) strtol(binary_line, NULL, 2);
    else {  // both tag and set not 0
      set_temp = (int) strtol(&binary_line[combined_len - s], NULL, 2);
      binary_line[combined_len - s] = '\0';
      tag_temp = (int) strtol(binary_line, NULL, 2); 
    }
  }
  // printf("set: %d, tag: %d\n", set_temp, tag_temp);
  *set = set_temp;
  *tag = tag_temp;
}

void evict(int s, int tag) {
  Line *set = CACHE[s];
  ttime min = (ttime) -1;
  int min_line;
  for (int i = 0; i < E; i++) {
    ttime cur = set[i].time;
    if (cur < min) {
      min = cur;
      min_line = i;
    }
  }
  set[min_line].tag = tag;
  set[min_line].time = MAX_TIME[s] + 1;
  MAX_TIME[s]++;
  EVICTION++;
}

void cache_load(char *line, bool modify) {
  int set, tag;
  parse_line(line, &set, &tag);
  int first_empty_line;
  bool hit = search_cache(set, tag, &first_empty_line);
  if (hit) {
    HIT++;
    if (VERBOSE) {
      if (modify)
        printf("%s hit hit\n", line);
      else
        printf("%s hit\n", line);
    }
  } else {
    if (first_empty_line > -1) {  // has empty line
      if (VERBOSE) {
        if (modify)
          printf("%s miss hit\n", line);
        else
          printf("%s miss\n", line);
      }
    } else {  // no empty line in current set
      evict(set, tag);
      if (VERBOSE) {
        if (modify)
          printf("%s miss eviction hit\n", line);
        else
          printf("%s miss eviction\n", line);
      }
    }
  }
  if (modify)
    HIT++;  // no matter the load phase hit or miss, store phase must be hit
}

void cache_store(char *line) {
  cache_load(line, false);
}

void cache_modify(char *line) {
  cache_load(line, true);
}

void simulate() {
  // create CACHE
  CACHE = (Line **) calloc(S, sizeof(Line *));
  for (int i = 0; i < S; i++) {
    if ((CACHE[i] = (Line *) calloc(E, sizeof(Line *))) == NULL)
      exit(EXIT_FAILURE);
  }
  MAX_TIME = (ttime *) calloc(S, sizeof(ttime *));
  
  // simulate from file line by line
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  if ((fp = fopen(TRACE_FILE, "r")) == NULL) {
    exit(EXIT_FAILURE);
  }
  while((read = getline(&line, &len, fp)) != -1) {
    line[strlen(line) - 1] = '\0';  // chop tailing newline getline stores
    if (len < OPT_CHOP - 1 || line[0] == 'I')
      continue;
    char opt = line[OPT_OFFSET];
    
    switch(opt) {
      case 'L':
        cache_load(line, false);
        break;
      case 'S':
        cache_store(line);
        break;
      case 'M':
        cache_modify(line);
        break;
    }
  }
  fclose(fp);

  printSummary(HIT, MISS, EVICTION);
}

int main(int argc, char *argv[])
{
  int opt;

  while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
    switch (opt) {
      case 'h':
        fprintf(stderr, "Usage: %s [-hv] -s <s> -E <E> -b <b> "
        "-t <tracefile>\n", argv[0]);
        return 0;
      case 'v':
        VERBOSE = 1;
        break;
      case 's':
        s = atoi(optarg);
        S = 2 << s;
        break;
      case 'E':
        E = atoi(optarg);
        break;
      case 'b':
        b = atoi(optarg);
        break;
      case 't':
        TRACE_FILE = optarg;
        break;
      default: /* '?' */
        fprintf(stderr, "Usage: %s [-hv] -s <s> -E <E> -b <b> "
          "-t <tracefile>\n", argv[0]);
        exit(EXIT_FAILURE);
      }
  }
  
  printf("S: %d, E: %d, b: %d, verbose: %d, tracefile: %s\n",
          S, E, b, VERBOSE, TRACE_FILE);

  simulate();
  return 0;
}

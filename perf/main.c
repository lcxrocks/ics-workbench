#include "perf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

#define DECL(fn) void fn();

PROGRAMS(DECL)

static void run(void (*func)(), int rounds);
static uint64_t gettime();
static void (*lookup(const char *fn))();

int main(int argc, char **argv) {
  // TODO: parse arguments: set @func and @rounds
  char funcname[64]= ""; 
  char *fun = funcname;
  int rounds = 1;
  memset(funcname, 0, sizeof(funcname));
  switch (argc)
  {
  case 1: //./perf
    memcpy(funcname, "dummy", sizeof(funcname));
    rounds = 1;
    break;

  case 2: //./perf dummy
    memcpy(funcname, argv[1], sizeof(funcname));
    rounds = 1; 
    break;
  
  case 4: //./perf -r 10000 dummy
    memcpy(funcname, argv[3], sizeof(funcname));
    rounds = atoi(argv[2]);
    break;

  default:
    printf("Invalid arg list.\n");
    return 1;
    break;
  }
  
  void (*func)() = lookup(fun);

  run(func, rounds);
}

static uint64_t gettime() {
  // TODO: implement me!
  clock_t c_time = clock();  //current time
  return c_time;
}

static void (*lookup(const char *fn))() {
  #define PAIR(fn) { #fn, fn },
  struct pair {
    const char *name;
    void *ptr;
  };
  struct pair lut[] = {
    PROGRAMS(PAIR)
  };

  for (int i = 0; i < LENGTH(lut); i++) {
    struct pair *p = &lut[i];
    if (strcmp(fn, p->name) == 0) return p->ptr;
  }
  return print_error;
}

static void run(void (*func)(), int rounds) {
  uint64_t *elapsed = malloc(sizeof(uint64_t) * rounds);
  if (!elapsed) {
    perror("elapsed");
    return;
  }

  for (int round = 0; round < rounds; round++) {
    uint64_t st = gettime();
    func();
    uint64_t ed = gettime();
    elapsed[round] = ed - st;
    double time_second = elapsed[round] / CLOCKS_PER_SEC ; // get time(seconds)
    printf("CPU clock used: %ld \t timeused: %f \n", elapsed[round], time_second);
  }

  // TODO: display runtime statistics
  free(elapsed);
}

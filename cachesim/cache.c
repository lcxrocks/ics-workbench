#include "common.h"
#include <inttypes.h>

uint32_t NR_LINE; 
uint32_t NR_GROUP;
uint32_t tt; //short for total_size_width
uint32_t as; //short for associativity width
uint32_t gp; //short for group number width

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions
uint32_t get_tag(uintptr_t addr){
  return 
}

uint32_t cache_read(uintptr_t addr) {
  //uint32_t tag = (uint32_t )
  return tt;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
}

void init_cache(int total_size_width, int associativity_width) {
  tt = total_size_width;
  as = associativity_width;
  gp = tt - BLOCK_SIZE - as;
  NR_LINE = exp2(tt - BLOCK_SIZE); //cache 行数
  for (int i = 0; i < NR_LINE; i++)
  {
    line[i].dirty = false;
    line[i].valid = false;
  }
}

void display_statistic(void) {
}

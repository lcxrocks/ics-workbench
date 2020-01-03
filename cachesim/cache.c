#include "common.h"
#include <inttypes.h>

#define block_offset 0x3F; //get block offset
uint32_t NR_LINE = 0; 
uint32_t tt = 0; //short for total_size_width
uint32_t as = 0; //short for associativity width
uint32_t gp = 0; //short for group number width
#define NR_GP exp(as); //组内行数

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

uint32_t check_hit(uintptr_t addr, int group_number, uint32_t tag){ //在cache中寻找主存对应的行号，缺失就返回-1
  int gp_start = group_number * NR_GP;
  int gp_end = gp_start + NR_GP -1; 
  for (int i = gp_start; i <= gp_end; i++)
  {
    if (line[i].valid == true && line[i].tag == tag)
    {
      return i; //返回hit后所在的行号
    }
  }
  return -1;
}

uint32_t cache_read(uintptr_t addr) {
  uint64_t tag = addr >> (tt- as); // 获得tag标记段
  uint32_t gp_num_mask = (1 << (tt-as-BLOCK_WIDTH)) - 1; 
  uint32_t gp_number = (addr >> BLOCK_WIDTH) & gp_num_mask; //获得组号
  uint32_t in_block_addr = addr & block_offset; //获得组内地址
  uint32_t data_num = in_block_addr >>2; //因为data是用uint32存的，所以每次取都只取1整个data出来(4 Byte)
  uint32_t line_number = check_hit(addr,gp_number,tag);

  if (line_number >= 0)
    return line[i].data[data_num];
  
  
  return 0;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
}

void init_cache(int total_size_width, int associativity_width) {
  tt = total_size_width;
  as = associativity_width;
  gp = tt - BLOCK_SIZE - as;
  NR_LINE = exp2(tt - BLOCK_WIDTH); //cache 行数
  for (int i = 0; i < NR_LINE; i++)
  {
    line[i].dirty = false;
    line[i].valid = false;
  }
}

void display_statistic(void) {
}

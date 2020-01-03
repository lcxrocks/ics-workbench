#include "common.h"
#include <inttypes.h>

uint32_t choose(uint32_t n);

#define block_offset 0x3F; //get block offset
uint32_t NR_LINE; 
uint32_t tt; //short for total_size_width
uint32_t as; //short for associativity width
uint32_t gp; //short for group number width
#define NR_GP exp2(as) //组内行数

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

void load(uintptr_t addr, uint32_t line_number, uint32_t tag){
  assert(line[line_number].valid ==false);
  uintptr_t block_num = addr >> BLOCK_WIDTH;
  mem_read(block_num, (uint8_t *)(line[line_number].data));
  line[line_number].valid = true;
  line[line_number].dirty = false;
  line[line_number].tag = tag;
}

void unload(uint32_t group_number, uint32_t line_number){
  assert(line[line_number].valid == true);
  line[line_number].valid = false;
  if (line[line_number].dirty)
  {
    uintptr_t block_number = (line[line_number].tag << gp) + group_number;
    mem_write(block_number,(uint8_t *)(line[line_number].data));
    line[line_number].dirty = false;
  }
}

uint32_t check_hit(uint32_t group_number, uint32_t tag){ //在cache中寻找主存对应的行号，缺失就返回-1
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

uint32_t replace(uintptr_t addr, uint32_t group_number, uint32_t tag){
  int gp_start = group_number * NR_GP;
  int gp_end = gp_start + NR_GP -1;
  for (int i = gp_start; i <= gp_end; i++)
  {
    if (line[i].valid == false) load(addr,i, tag);
    return i; //返回新装入的行号
  }
  //开始随机替换
  uint32_t line_replace = gp_start + choose(NR_GP);
  assert(line_replace <= gp_end);
  unload(group_number,line_replace);
  load(addr,line_replace, tag);
  return line_replace;
}

uint32_t cache_read(uintptr_t addr) {
  uint64_t tag = addr >> (tt- as); // 获得tag标记段
  uint32_t gp_num_mask = (1 << (tt-as-BLOCK_WIDTH)) - 1; 
  uint32_t gp_number = (addr >> BLOCK_WIDTH) & gp_num_mask; //获得组号
  uint32_t in_block_addr = addr & block_offset; //获得组内地址
  uint32_t data_num = in_block_addr >>2; //因为data是用uint32存的，所以每次取都只取1整个data出来(4 Byte)
  uint32_t line_number = check_hit(gp_number,tag);

  if (line_number >= 0)
    return line[i].data[data_num];
  
  else //否则就要进行替换
  {
    uint32_t line_miss = replace(addr, gp_number, tag);
    return line[line_miss].data[data_num];
  }
  
  
  return 0;
}

void write2line(uint32_t line_number, int data_num, uint32_t data, uint32_t wmask){
  line[line_number].dirty = true;
	line[line_number].data[data_num] &= (~wmask);
	line[line_number].data[data_num] |= (data & wmask);
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint64_t tag = addr >> (tt- as); // 获得tag标记段
  uint32_t gp_num_mask = (1 << (tt-as-BLOCK_WIDTH)) - 1; 
  uint32_t gp_number = (addr >> BLOCK_WIDTH) & gp_num_mask; //获得组号
  uint32_t in_block_addr = addr & block_offset; //获得组内地址
  uint32_t data_num = in_block_addr >>2; //因为data是用uint32存的，所以每次取都只取1整个data出来(4 Byte)
  uint32_t line_number = check_hit(gp_number,tag);
  if (line_number >= 0)
    write2line(line_number, data_num, data, wmask);
  else{
    uint32_t line_miss = replace(addr, gp_number, tag);
    write2line(line_miss, data_num, data, wmask);
  }
  
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
#include "common.h"
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>
static inline uint32_t choose(uint32_t n) { return rand() % n; }

#define block_offset 0x3F; //get block offset
uint32_t nr_line; 
uint32_t tt; //short for total_size_width
uint32_t as; //short for associativity width
uint32_t gp; //short for group number width
#define NR_GP exp2(as) //组内行数

double write_time, read_time, total_time; //for statistics
double miss_time = 0;
int total_cnt = 0;
int miss_cnt = 0;
int call_to_read = 0;
int call_to_write = 0;
int hit_cnt = 0;
struct timeval cache_st, cache_ed;

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

void load(uintptr_t addr, int line_number, uint32_t tag){
  assert(line[line_number].valid ==false);
  uintptr_t block_num = addr >> BLOCK_WIDTH;
  mem_read(block_num, (uint8_t *)(line[line_number].data));
  line[line_number].valid = true;
  line[line_number].dirty = false;
  line[line_number].tag = tag;
}

void unload(uint32_t group_number, int line_number){
  assert(line[line_number].valid == true);
  line[line_number].valid = false;
  if (line[line_number].dirty)
  {
    uintptr_t block_number = (line[line_number].tag << gp) + group_number;
    mem_write(block_number,(uint8_t *)(line[line_number].data));
    line[line_number].dirty = false;
  }
}

int check_hit(uint32_t group_number, uint32_t tag){ //在cache中寻找主存对应的行号，缺失就返回-1
  int gp_start = group_number * NR_GP;
  int gp_end = gp_start + NR_GP -1; 
  for (int i = gp_start; i <= gp_end; i++)
  {
    if (line[i].valid == true && line[i].tag == tag)
    {
      hit_cnt++;
      return i; //返回hit后所在的行号
    }
  }
  return -1;
}

uint32_t replace(uintptr_t addr, uint32_t group_number, uint32_t tag){
  miss_cnt++;
  gettimeofday(&cache_st, NULL);
  int gp_start = group_number * NR_GP;
  int gp_end = gp_start + NR_GP -1;
  //printf("gp_start: %d, gp_end: %d\n",gp_start,gp_end);
  for (int i = gp_start; i <= gp_end; i++)
  {
    if (line[i].valid == false) 
    {
      load(addr,i, tag);
      return i; //返回新装入的行号
    }
  }
  //开始随机替换
  int32_t line_replace = gp_start + choose(NR_GP);
  assert(line_replace <= gp_end);
  unload(group_number,line_replace);
  load(addr,line_replace, tag);
  gettimeofday(&cache_ed, NULL);
  miss_time += (cache_ed.tv_usec - cache_st.tv_usec);
  return line_replace;
}

uint32_t cache_read(uintptr_t addr) {
  call_to_read++;
  uint32_t tag = addr >> (tt- as); // 获得tag标记段
  uint32_t gp_num_mask = (1 << (tt-as-BLOCK_WIDTH)) - 1; 
  uint32_t gp_number = (addr >> BLOCK_WIDTH) & gp_num_mask; //获得组号
  uint32_t in_block_addr = addr & block_offset; //获得组内地址
  uint32_t data_num = in_block_addr >>2; //因为data是用uint32存的，所以每次取都只取1整个data出来(4 Byte)
  int line_number = check_hit(gp_number,tag);
  // printf("addr\t\ttag\t\tgp_number\tin_block_addr\tdata_num\t\t\n");
  // printf("%8lx\t%8x\t%8x\t%8x\t%8x\t\n",addr,tag, gp_number, in_block_addr, data_num);
  // printf("read line_number: %d\n",line_number);
  if (line_number >= 0)
    return line[line_number].data[data_num];
  
  else //否则就要进行替换
  {
    int line_miss = replace(addr, gp_number, tag);
    return line[line_miss].data[data_num];
  }
}

void write2line(int line_number, uint32_t data_num, uint32_t data, uint32_t wmask){
  line[line_number].dirty = true;
	line[line_number].data[data_num] &= (~wmask);
	line[line_number].data[data_num] |= (data & wmask);
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  call_to_write++;
  uint32_t tag = addr >> (tt- as); // 获得tag标记段
  uint32_t gp_num_mask = (1 << (tt-as-BLOCK_WIDTH)) - 1; 
  uint32_t gp_number = (addr >> BLOCK_WIDTH) & gp_num_mask; //获得组号
  uint32_t in_block_addr = addr & block_offset; //获得组内地址
  uint32_t data_num = in_block_addr >>2; //因为data是用uint32存的，所以每次取都只取1整个data出来(4 Byte)
  int line_number = check_hit(gp_number,tag);
  // printf("addr\t\tdata\t\twmask\t\ttag\t\tgp_number\tin_block_addr\tdata_num\t\t\n");
  // printf("%8lx\t%8x\t%8x\t%8x\t%8x\t%8x\t%8x\t\n",addr, data, wmask, tag, gp_number, in_block_addr, data_num);
  // printf("line number : %d\n",line_number);
  if (line_number >= 0)
    write2line(line_number, data_num, data, wmask);
  else{
    int line_miss = replace(addr, gp_number, tag);
    write2line(line_miss, data_num, data, wmask);
  }
}

void init_cache(int total_size_width, int associativity_width) {
  tt = total_size_width;
  as = associativity_width;
  gp = tt - BLOCK_WIDTH - as;
  nr_line = exp2(tt - BLOCK_WIDTH); //cache 行数
  call_to_read = 0; 
  call_to_write = 0;
  cycle_cnt = 0;
  miss_time = 0;
  total_cnt = 0;
  miss_cnt = 0;
  hit_cnt = 0;

  for (int i = 0; i < nr_line; i++)
  {
    line[i].dirty = false;
    line[i].valid = false;
  }
  printf("finished init_cache, tt: %d, as:%d, gp:%d, nr_line:%d\n",tt,as,gp,nr_line);
}

void display_statistic(void) {
  total_cnt = hit_cnt + miss_cnt;
  total_time = read_time + write_time;
  printf("total call: %d\t call_to_read: %d\t call to write: %d\n", call_to_write+call_to_write,call_to_read,call_to_write);
  printf("hit: %d / %d;\nmiss: %d / %d\n", hit_cnt, total_cnt, miss_cnt, total_cnt);
  printf("miss_time: %lf / %lf\n", miss_time, total_time);
}

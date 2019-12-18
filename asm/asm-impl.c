#include "asm.h"
#include <string.h>
// __asm__(assembler template 
// 	: output operands                  /* optional */
// 	: input operands                   /* optional */
// 	: list of clobbered registers      /* optional */
// 	);

// 返回有符号 64 位整数 a + b 的数值
int64_t asm_add(int64_t a, int64_t b){
  int64_t result = 0;
    asm volatile(
    "leaq (%[a], %[b]), %[t];"
    : [t] "=r"(result)
    :[a] "r"(a),[b] "r"(b)
  );
  return result;
}

// 返回无符号 64 位整数 x 二进制表示中 1 的数量
int asm_popcnt(uint64_t n) {
  unsigned int c = 0;
  for (c = 0; n; c++)
  {
    asm volatile(
      "\t leaq -1(%[n]), %%rdx;"
      "\t andq %%rdx, %[n];"
      : [n]"+r"(n)
      :"[n]"(n)
      :"rdx"
    );
  }
  return c;
}

// C 标准库中的 memcpy，用于复制两段不相交的内存
void *asm_memcpy(void *dest, const void *src, size_t n) {
  uint64_t cnt = 0;
  void *_dest = dest;
  while (cnt<n)
  {
    asm volatile(
      "movq %[dest], %%rdi;"
      "movq %[src], %%rsi;"
      "addq %[cnt], %%rdi;"
      "addq %[cnt], %%rsi;"
      "movb (%%rsi), %%al;"
      "movb %%al, (%%rdi);"
      :
      : [cnt]"r"(cnt),[src]"r"(src), [dest]"r"(dest)
      : "al","rdi","rsi"
    );
    cnt++;
  }
  return _dest;
}

// C 标准库中的 setjmp/longjmp，用于控制流长跳转
// # our buffer looks like:
// #  rbx,rbp,r12,r13,r14,r15,rsp,rip
// int asm_setjmp(asm_jmp_buf env) {
//   asm volatile(
//     "movq %%rbx, (%%rdi);"
//     "movq %%rbp, 0x8(%%rdi);"
//     "movq %%r12, 0x10(%%rdi);"
//     "movq %%r13, 0x18(%%rdi);"
//     "movq %%r14, 0x20(%%rdi);"
//     "movq %%r15, 0x28(%%rdi);"
//     "leaq 8(%%rsp), %%rax;"
//     "movq  %%rax, (%%rdi);"
//     "movq (%%rsp), %%rax;"
//     "movq (%%rax), 0x30(%%rdi);"
//     :"=rdi"(env)
//     :
//     :"rax"
//   );
//   return 0;
// }
int asm_setjmp(asm_jmp_buf env) {
  asm volatile(
    "mov %%rbx, (%%rdi);"
    "mov %%rbp,0x8(%%rdi);"
    "mov %%r12,0x10(%%rdi);"
    "mov %%r13,0x18(%%rdi);"
    "mov %%r14,0x20(%%rdi);"
    "mov %%r15,0x28(%%rdi);"
    "lea 0x8(%%rsp),%%rdx;"
    "mov %%rdx, 0x30(%%rdi);"
    "mov (%%rsp),%%rax;"
    "mov %%rax, 0x38(%%rdi);"
    : "=rdi"(env)
    :
    :"rax","rdx"
  );
  return 0;
}
void asm_longjmp(asm_jmp_buf env, int val) {
  uint64_t _val = (uint64_t) val;
  asm volatile(
    "movq %%rsi, %%rax;" //ret val
    "movq 0x8(%%rdi), %%rsp;"
    "movq 0x30(%%rdi), %%rsp;"
    "pushq 0x38(%%rdi);"
    "movq (%%rdi), %%rbx;"
    "movq 0x10(%%rdi), %%r12;"
    "movq 0x18(%%rdi), %%r13;"
    "movq 0x20(%%rdi), %%r14;"
    "movq 0x28(%%rdi), %%r15;"

    "testq %%rax, %%rax;" //if rax!=0, ret val = val;
    "movq $1, %%rsi;"
    "cmove %%rsi, %%rax;" //if rax=0, ret val = 1;
    :
    :"rdi"(env), "rsi"(_val)  
    );
    return ;
}

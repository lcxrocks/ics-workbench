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
int asm_setjmp(asm_jmp_buf env) {
  asm volatile(
    "movq %%rbx, (%%rdi);"
    "movq %%rbp, 8(%%rdi);"
    "movq %%r12, 16(%%rdi);"
    "movq %%r13, 24(%%rdi);"
    "movq %%r14, 32(%%rdi);"
    "movq %%r15, 40(%%rdi);"
    "leaq 8(%%rsp), %%rax;"
    "movq  %%rax, 48(%%rdi);"
    "movq (%%rsp), %%rax;"
    "movq %%rax, 56(%%rdi);" //ret addr
    :"=rdi"(env)
    :
    :"rax"
  );
  return 0;
}

// void asm_longjmp(asm_jmp_buf env, int val) {
//   uint64_t _val = (uint64_t) val;
//   asm volatile(
//     "movq %%rsi, %%rax;" //ret val
//     "movq 8(%%rdi), %%rsp;"
//     "movq 48(%%rdi), %%rsp;"
//     "pushq 56(%%rdi);"
//     "movq (%%rdi), %%rbx;"
//     "movq 16(%%rdi), %%r12;"
//     "movq 24(%%rdi), %%r13;"
//     "movq 32(%%rdi), %%r14;"
//     "movq 40(%%rdi), %%r15;"

//     "testq %%rax, %%rax;" //if rax!=0, ret val = val;
//     "movq $1, %%rsi;"
//     "cmove %%rsi, %%rax;" //if rax=0, ret val = 1;
//     :
//     :"rdi"(env), "rsi"(_val)  
//     );
//     return ;
// }
void asm_longjmp(asm_jmp_buf env, int val) {
  asm volatile(
    "movq %%rsi, %%rax;" //ret val
    "movq 8(%%rdi), %%rsp;"
    "movq 48(%%rdi), %%rsp;"
    "movq 56(%%rdi), %%rdx;" //ret addr
    "movq (%%rdi), %%rbx;"
    "movq 16(%%rdi), %%r12;"
    "movq 24(%%rdi), %%r13;"
    "movq 32(%%rdi), %%r14;"
    "movq 40(%%rdi), %%r15;"

    "testq %%rax, %%rax;" //if rax!=0, ret val = val;
    "movq $1, %%rsi;"
    "cmove %%rsi, %%rax;" //if rax=0, ret val = 1;
    "jmpq *%%rdx;"
    :
    :"rdi"(env), "rsi"(_val)  
    );
    return ;
}
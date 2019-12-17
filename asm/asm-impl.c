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
int asm_setjmp(asm_jmp_buf env) {

  asm volatile(
    "movq %%rax, (%[env]);"
    "movq %%rbx, 8(%[env]);"
    "movq %%rcx, 16(%[env]);"
    "movq %%rdx, 24(%[env]);"
    "movq %%rsp, 32(%[env]);"
    "movq %%rbp, 40(%[env]);"
    "movq %%rsi, 48(%[env]);"
    "movq %%rdi, 56(%[env]);"
    "movq %%r8, 64(%[env]);"
    "movq %%r9, 72(%[env]);"
    "movq %%r10, 80(%[env]);"
    "movq %%r11, 88(%[env]);"
    "movq %%r12, 96(%[env]);"
    "movq %%r13, 104(%[env]);"
    "movq %%r14, 112(%[env]);"
    "movq %%r15, 120(%[env]);"
    "movq (%%rsp), %%rax;"
    "movq %%rax, 128(%[env]);" //ret addr.
    :
    :[env]"r"(env)
  );
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  uint64_t val = (uint64_t) val;
  asm volatile(
    "movq (%[env]), %%rax;"
    "movq 8(%[env]), %%rbx;"
    "movq 16(%[env]), %%rcx;"
    "movq 24(%[env]), %%rdx;"
    "movq 32(%[env]), %%rsp;"
    "movq 40(%[env]), %%rbp;"
    //no rsi
    //no rdi
    "movq 64(%[env]), %%r8;"
    "movq 72(%[env]), %%r9;"
    "movq 80(%[env]), %%r10;"
    "movq 88(%[env]), %%r11;"
    "movq 96(%[env]), %%r12;"
    "movq 104(%[env]), %%r13;"
    "movq 112(%[env]), %%r14;"
    "movq 120(%[env]), %%r15;"
    "testq %%rax, %%rax;"
    "movq %[val], %%rax;" //if rax!=0, ret val = val;
    "movq $1, %%rsi;"
    "cmove %%rsi, %%rax;" //if rax=0, ret val = 1;

    "movq 128(%[env]), %%rsi;"
    "movq %%rsi, (%%rsp);" //restore ret addr.
    
    "movq 48(%[env]), %%rsi;"
    "movq 56(%[env]), %%rdi;"
    :
    :[env]"r"(env), [val]"r"(val)  
    );
    return ;
}

#include <setjmp.h>
#include <stdio.h>

jmp_buf env;

int f(int n) {
  if (n >= 8) longjmp(env, n); // 某个条件达成时，恢复快照
  printf("Call f(%d)\n", n);
  f(n + 1);
}

int main() {
  int r = setjmp(env); // 快照
  if (r == 0) {
    f(1);
  } else { // longjmp goes here
    printf("Recursion reaches %d\n", r);
  }
}
#include "perf.h"
#include <stdio.h>
#include <stdlib.h>
#include "../multimod/multimod.h"

void dummy() {
}

void print_hello() {
  printf("hello\n");
}

void print_error() {
  printf("invalid subject!\n");
  exit(1);
}

void simple_loop() {
  for (volatile int i = 0; i < 1000000; i++) ;
}

void multimod_p1(int64_t a, int64_t b, int64_t m) {
    uint64_t A[70]={};
    uint64_t B[70]={};
    uint64_t C[140]={};
    int la = 0;
    int lb = 0;
    int lc = 0;
    while (a > 0)
    {
      A[la] = a%10;
      a /=10;
      la++;
    }

    while (b > 0)
    {
      B[lb] = b%10;
      b /=10;
      lb++;
    }

    for (int i = 0; i < la; i++)
      for (int j = 0; j< lb; j++)
        C[i+j+1] += A[i] * B[j];
    
    for (int i = 1; i <= la+lb; i++)
    {
      C[i-1] = C[i] % 10;
      C[i+1] += C[i] / 10;
    }

    for (int i = la+lb-1; i >= 0; i--)
    {
      if(C[i]!=0) {
        lc = i; 
        break;
      }
    }
    
    uint64_t result = 0;
    uint64_t ans2x = 0;
    uint64_t ans4x = 0;
    uint64_t ans8x = 0;
    uint64_t ans10x = 0;

    for(int i=lc;i>=0;i--)  
    {
      ans2x = (uint64_t)(result<<1)%(uint64_t)m;
      ans4x = (uint64_t)(ans2x<<1)%(uint64_t)m;
      ans8x = (uint64_t)(ans4x<<1)%(uint64_t)m;
      ans10x = (uint64_t)(ans8x+ans2x)%(uint64_t)m;
      result = (uint64_t)(ans10x + C[i]) %(uint64_t)m;
      //assert(result < m);
    }
    printf("abm\n");
    //return (int64_t)result;
}

void multimod_p2(int64_t a, int64_t b, int64_t m) {
    uint64_t tmp = 0;
    if(a<b) tmp=a,a=b,b=tmp; //swap(a,b)
    uint32_t A[70]={};
    uint32_t la=0;
    uint64_t result = 0;
    while ((uint64_t)a > 0)
    {
      A[la] = a & 0x1;
      a >>= 1;
      la++;
    }
    b = (uint64_t)b % (uint64_t)m;
    for (int i = la; i >= 0; i--)
    {
      result  = (uint64_t)((result << 1)%m + (uint64_t)(A[i]*b)) %m;
    }
    //return (int64_t) result;
}

void multimod_p3(int64_t a, int64_t b, int64_t m) {
  int64_t t = (a * b - (int64_t)((double)a * b / m) * m) % m; 
  (void) t; //make gcc happy
  //return t < 0 ? t + m : t;
}
#include "multimod.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>

int64_t multimod_p1(int64_t a, int64_t b, int64_t m) {
    uint64_t A[70]={};
    uint64_t B[70]={};
    uint64_t M[70]={};
    uint64_t C[140]={};
    int la = 0;
    int lb = 0;
    int lc = 0;
    int lm = 0;
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
      assert(result < m);
    }
    return (int64_t)result;
}

int main(){
  char tmp[256]={};
  int64_t a=0;
  int64_t b=0;
  int64_t m=0;
  int64_t c=0;
  int64_t result = 0;
  char *delim= " ";
  char *num;
  int cnt= 0;
  FILE *fp = fopen("test.txt","r");
  struct timeval t1, t2;
  double _time;
  if(NULL==fp) printf("Can not find file\n");
  else 
  { printf("testing\n");
    while (fgets(tmp, 256, fp))
    {
      num = strtok(tmp,delim);  sscanf(num, "%ld", &a); 
      num = strtok(NULL,delim); sscanf(num, "%ld", &b);
      num = strtok(NULL,delim); sscanf(num, "%ld", &m); 
      num = strtok(NULL,delim); sscanf(num, "%ld", &c); 
      memset(tmp,0,sizeof(tmp));
      gettimeofday(&t1, NULL);
      result = multimod_p1(a,b,m);
      gettimeofday(&t2,NULL);
      cnt++;
      if(c != result) cnt--;
      _time += t2.tv_sec - t1.tv_sec+(t2.tv_usec - t1.tv_usec)/1000000.0; 
    }
    printf("Success! Time: %lf, correct: %d\n",_time, cnt);
  }
  fclose(fp);
}
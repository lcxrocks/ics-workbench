#include "multimod.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

int64_t multimod_p3(int64_t a, int64_t b, int64_t m) {
  int64_t t = (a * b - (int64_t)((double)a * b / m) * m) % m;
  return t < 0 ? t + m : t;
}
int main(){
  char tmp[256]={};
  int64_t a=0;
  int64_t b=0;
  int64_t m=0;
  int64_t c=0;
  int64_t result = 0;
  int64_t pivot = 0;
  char *delim= " ";
  char *num;
  int cnt= 0;
  int correct =0;
  FILE *fp = fopen("counter_ex.txt","r");
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
      num = strtok(NULL,delim); sscanf(num, "%ld", &pivot);
      memset(tmp,0,sizeof(tmp));
      gettimeofday(&t1, NULL);
      result = multimod_p3(a,b,m);
      gettimeofday(&t2,NULL);
      cnt++; correct++;
      if(c != result){
        correct--;
        printf("wrong at: ( %ld * %ld ) mod %ld = %ld\n", a,b,m,c);
        printf("p3.c ans : %ld\n", result);
        printf("pivot: %ld\n",pivot);
        printf("mypivot: %ld\n", (int64_t)((double)a * b / m));
      }
      else
      {
        printf("pivot: %ld\n",pivot);
        printf("mypivot: %ld\n", (int64_t)((double)a * b / m));
      }
      
      _time += t2.tv_sec - t1.tv_sec+(t2.tv_usec - t1.tv_usec)/1000000.0; 
    }
    printf("Success! Time: %lf, correct: %d / %d\n",_time, correct, cnt);
  }
  fclose(fp);
  // uint64_t a,b,m;
  // while (scanf("%ld %ld %ld",&a,&b,&m))
  // {
  //   printf("result: %ld\n",multimod_p2(a,b,m));
  // }

}
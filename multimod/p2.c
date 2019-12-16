#include "multimod.h"
// #include <stdint.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <sys/time.h>
// #include <unistd.h>

int64_t multimod_p2(int64_t a, int64_t b, int64_t m) {
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
    return (int64_t) result;
}

// int main(){
//   char tmp[256]={};
//   int64_t a=0;
//   int64_t b=0;
//   int64_t m=0;
//   int64_t c=0;
//   int64_t result = 0;
//   char *delim= " ";
//   char *num;
//   int cnt= 0;
//   int correct =0;
//   FILE *fp = fopen("test.txt","r");
//   struct timeval t1, t2;
//   double _time = 0;
//   if(NULL==fp) printf("Can not find file\n");
//   else 
//   { printf("testing\n");
//     while (fgets(tmp, 256, fp))
//     {
//       num = strtok(tmp,delim);  sscanf(num, "%ld", &a); 
//       num = strtok(NULL,delim); sscanf(num, "%ld", &b);
//       num = strtok(NULL,delim); sscanf(num, "%ld", &m); 
//       num = strtok(NULL,delim); sscanf(num, "%ld", &c); 
//       memset(tmp,0,sizeof(tmp));
//       gettimeofday(&t1, NULL);
//       result = multimod_p2(a,b,m);
//       gettimeofday(&t2,NULL);
//       cnt++; correct++;
//       if(c != result) correct--;
//       _time += t2.tv_sec - t1.tv_sec+(t2.tv_usec - t1.tv_usec)/1000000.0; 
//     }
//     printf("Success! Time: %lf, correct: %d / %d\n",_time, correct, cnt);
//   }
//   fclose(fp);
//   // uint64_t a,b,m;
//   // while (scanf("%ld %ld %ld",&a,&b,&m))
//   // {
//   //   printf("result: %ld\n",multimod_p2(a,b,m));
//   // }

// }
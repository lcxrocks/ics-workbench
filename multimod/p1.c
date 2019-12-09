#include "multimod.h"
#include <string.h>
#include <stdlib.h>

int64_t multimod_p1(int64_t a, int64_t b, int64_t m) {
    char c[256]={};
    char stra[128]={};
    char strb[128]={};
    char s[256]={};
    itoa(a,stra,10);
    itoa(b,strb,10);

    int La = strlen(stra);
    int Lb = strlen(strb);

    for(int i=1;i<=La;i++)  
        for(int j=1;j<=Lb;j++)  
        c[i+j-1]+=(int)stra[i]*(int)strb[j];//a的第i位乘以b的第j位为积的第i+j-1位（先不考虑进位）

    for(int i=1;i<=La+Lb;i++)  
        c[i+1]+=c[i]/10,c[i]%=10;//统一处理进位  

    if(c[La+Lb]) s[La+Lb]+=c[La+Lb]+'0';//判断第i+j位上的数字是不是0  
    for(int i=La+Lb-1;i>=1;i--)  
        s[i]+=c[i];//将整形数组转成字符串  
    
    return s;  
 
}

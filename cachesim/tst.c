#include <stdio.h>
int hah = 0;

void p1(int something){
    hah = something;
    printf("hah in p1: %d\n", hah);
}

void p2(){
    printf("hah in p2: %d\n", hah);
}

int main(int argc, char const *argv[])
{
    p2();
    p1(3);
    p2();
    p1(5);
    p2();
    return 0;
}

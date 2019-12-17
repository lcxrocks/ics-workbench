// #include <stdint.h>
// #include <stdio.h>
// #include <string.h>
// void *asm_memcpy(void *dest, const void *src, size_t n) {
//   uint64_t cnt = 0;
//   void *_dest = dest;
//   while (cnt<n)
//   {
//     asm volatile(
//       "movq %[dest], %%rdi;"
//       "movq %[src], %%rsi;"
//       "addq %[cnt], %%rdi;"
//       "addq %[cnt], %%rsi;"
//       "movb (%%rsi), %%al;"
//       "movb %%al, (%%rdi);"
//       :
//       : [cnt]"r"(cnt),[src]"r"(src), [dest]"r"(dest)
//       : "al","rdi","rsi"
//     );
//     cnt++;
//   }
//   return _dest;
// }

// int main(){
//     char tmp[20] = "";
//     char hello[20]= "Hello World!\n";
//     asm_memcpy(tmp,hello,sizeof(hello));
//     printf("tmp:%s\n",tmp);
// }
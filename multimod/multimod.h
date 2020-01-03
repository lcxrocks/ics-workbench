#include <stdint.h>

typedef void (*multimod_t)(int64_t, int64_t, int64_t); //uint64_t changed to void due to lab3
#define LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

void multimod_p1(int64_t a, int64_t b, int64_t m);
void multimod_p2(int64_t a, int64_t b, int64_t m);
void multimod_p3(int64_t a, int64_t b, int64_t m);

extern multimod_t multimod_tab[];

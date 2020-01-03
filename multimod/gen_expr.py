#!/usr/bin/env python3
import random

#MAX = 134217728
MAX = 9223372036854775807
f = open("test.txt","w")
i = 0
for i in range(0,10000000):
    i = i+1
    a = random.randint(0,MAX)
    b = random.randint(0,MAX)
    m = random.randint(1,MAX)
    ans = (a * b) % m
    f.write (str(a)+' '+str(b)+ ' '+str(m)+ ' ' +str(ans) +'\n')
f.close()
#!/usr/bin/env python3
import random

#MAX = 134217728
#MAX = 9223372036854775807
SIZE = 14;
f = open("test.txt","w")
i = 0

for i in range(0,100000):
    la = random.randint(1,SIZE)
    lb = random.randint(1,SIZE)
    lc = random.randint(1,SIZE)
    a = '1'
    b = '1'
    c = '1'
    i = i+1
    for j in range(0,la):    
        a = str(a) + '0'
    for k in range(0,lb):
        b = str(b) + '0'
    for p in range(0,lc):
        c = str(c) + '0'
    # a = random.randint(0,MAX)
    # b = random.randint(0,MAX)
    # m = random.randint(1,MAX)
    f.write (str(a)+' '+str(b)+ ' '+str(c)+'\n')
f.close()
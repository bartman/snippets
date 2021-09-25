#!/usr/bin/python3

import example
import random

vs = example.Vectors()
print(vs)
print()

for i in range(10):
    x = random.randint(1,100);
    y = random.randint(1,100);
    z = random.randint(1,100);
    v = example.Vector(x,y,z)
    print(v)
    vs.append(v)
print()

print(vs)
print(len(vs))
print()

for i in range(len(vs)):
    v = vs[i]
    print(v)

print()



for v in vs:
    print(v)

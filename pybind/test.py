#!/usr/bin/python3
import os
import os.path
import sys
sys.path.append("build")
import my_module

print(dir(my_module))
for n in dir(my_module):
    a = getattr(my_module, n)
    print(' -', n, type(a), str(a))

print(my_module.add)
print(my_module.add(1,2))

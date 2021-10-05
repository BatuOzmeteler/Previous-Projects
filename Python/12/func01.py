#!/usr/bin/env python3
# -*- coding: utf-8 -*-

def addOne(y):
    y += 1
    print(y)
    return y

x = 2
z = addOne(x)
print(x)
print(z)


def manyArgs(*arg): 
     print ("I was called with", len(arg), "arguments:", arg)

manyArgs(1) 
manyArgs("a",True)
manyArgs(1,2,3)

a = ['a1', 'a2', 'a3']
b = ['b1', 'b2']

print ("Zip:")
for x, y in zip(a, b):
    print (x, y)

aa=[(x,y) for x in a for y in b]
print (aa)

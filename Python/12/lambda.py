#!/usr/bin/env python3
# -*- coding: utf-8 -*-

def myfunc(n):
  
    return lambda a : a * n



mydoubler = myfunc(2) #HOW????

print(mydoubler(11)) #What’s written?

mydict = {'A':4,'B':100,'C':0,'D':87}
print (mydict.items())

mmm=max(mydict.items(), key=lambda rr: rr[1])
print (mmm)


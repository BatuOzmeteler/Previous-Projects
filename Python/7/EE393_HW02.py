# -*- coding: utf-8 -*-
"""
Created on Wed Oct 17 10:11:22 2018

Author: Mehmet Batu Özmeteler
Class: EE393.A 
School No: S009924
Department: Electrical & Electronics Engineering 

This program accepts a sequence of comma separated 4 digit binary numbers as its 
input and then checks whether they are divisible by 5 or not. The numbers that are 
divisible by 5 are printed in a comma separated sequence.

"""

sequence = input("Give me a sequence of comma separated 4 digit binary numbers - ") #input
listBinary = [] 
digits = ""
sequenceLength = len(sequence)
final = ""

for x in range(0,sequenceLength): #getting rid of commas and registering binary numbers in a list
    
    if sequence[x] != ',':
        digits = digits + sequence[x]
    else:
        listBinary.append(digits)
        digits = ""
    if x == sequenceLength - 1:
        listBinary.append(digits)

binaryLength = len(listBinary) 

for y in range(0,binaryLength): #checking if the binary numbers are divisible by 5
    if int(listBinary[y],2) % 5 == 0:
        final = final + listBinary[y] + ','
        
if(len(final) != 0): #getting rid of the last comma
    if final[-1] == ',':
        final = final[:-1]
                           
print(final)


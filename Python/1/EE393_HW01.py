# -*- coding: utf-8 -*-
"""
Created on Sat Oct  6 16:52:34 2018

Author: Mehmet Batu Özmeteler
Class: EE393.A 
School No: S009924
Department: Electrical & Electronics Engineering 

Program Description: This program takes an input in the form of an integer and checks if it's
a Fibonacci Number while printing every single Fibonacci Number smaller than it.

"""


fibonacciNumber = int (input("Give me a number ")) #Takes the input in the form of an integer

index1 = 0 #Indexes that indicate which numbers are summed up 
index2 = 1 #to calculate the next Fibonacci Number
list = [0,1] #the list where the numbers are kept
while list[-1] < fibonacciNumber: #if the last element is bigger than our input, loop breaks
    list.append(list[index1]+list[index2]) #appending the last calculated Fibonacci Number 
    index1 = index1 + 1 #incrementation of 
    index2 = index2 + 1 #the indexes

print(list)
if list[-1] == fibonacciNumber: #checks if the last element is a Fibonacci Number
    print(fibonacciNumber , "is a Fibonacci Number!!!")
else:
    print(fibonacciNumber , "is not a Fibonacci Number!!!")


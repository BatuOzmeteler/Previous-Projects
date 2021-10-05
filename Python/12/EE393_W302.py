#!/usr/bin/env python3
# -*- coding: utf-8 -*-

print ('12'.zfill(5)) #will print '00012'
print ('12'.ljust(5)) #will print '12 '
print ('12'.rjust(5)) #will print ' 12'
print ('12'.zfill(5).ljust(10)) #will print '00012 '
print ('12'.zfill(5).rjust(10)) #will print ' 00012'
print(192,168,178,42,sep=".") #will print 192.168.178.42

for i in range(4):
    print(i, end=" :-) ")
    
#Formatting the output
print('\nWe are the {} who say "{}!"'.format('knights', 'Long live the King!')) 
import math 
print('The value of PI is approximately %10.6f.' % math.pi) 

#Keyboard input
name = input("What's your name? ") 
print("Nice to meet you " + name + "!") 
age = input("Your age? ") 
print("So, you are already " + age + " years old, " + name + "!")
age = int(input("Your age?")) 
print (age*age)

#conditionals
# if statement 
x=50
if x > 0:
    print('positive') # if/else statement 
    
if x > 0:
    print('positive') 
else:
    print('zero or negative')

if x > 0:
    print('positive') 
elif x == 0:
    print('zero') 
else: 
    print('negative')

a=3.5 
if (type(a) == int):
    print(a, " is int") 
else:
    print(a, " is something else")
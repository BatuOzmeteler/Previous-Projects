#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Open a file
ff = open("foo.txt", "w")
ff.write("Python is a great language.\nYeah its great!!\nOzU students are great\n");

# Close opend file
ff.close()

# Open a file
fo = open("foo.txt", "r+")
str = fo.read(10);
print ("Read String is : ", str)

# Check current position
position = fo.tell();
print ("Current file position : ", position)

# Reposition pointer at the beginning once again
position = fo.seek(0, 0);
str = fo.read(10);
print ("Again read String is : ", str)

print ("Reading the file from the beginning...\n----------------\n")
position = fo.seek(0, 0) #go back again!!
for line in fo: #read line by line
   print(line, end = '')

# Close opend file
fo.close()
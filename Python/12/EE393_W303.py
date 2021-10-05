#!/usr/bin/env python3
# -*- coding: utf-8 -*-

count = 0 
while (count < 9):
    print ('The count is:', count) 
    count = count + 1
print ("Good bye!")

#Case 2: infinite loop
#var = 1 
#while var is 1 : # This constructs an infinite loop
#    num = input("Enter a number :")
#    print ("You entered: ", num)
#print ("Good bye!")

#Case 3: else in while statement
count = 0 
while count < 5:
    print (count, " is less than 5")
    count += 1 
else:
    print (count, " is not less than 5")

#for loops and formatted outputs
for letter in 'Python': # First Example 
    print ('Current Letter :', letter)
    
for x in range(0, 3):
    print ("We're on time %d" % (x))

#finds prime numbers between 10 and 20
for num in range(10,20): #to iterate between 10 to 20
    for i in range(2,num): #to iterate on the factors of the number 
        if num%i == 0: #to determine the first factor 
            j=num/i #to calculate the second factor 
            print ('%d equals %d * %d' % (num,i,j)) 
            break #to move to the next number, the #first FOR 
    else: # else part of the loop
        print (num, 'is a prime number')

#while vs for
#for is much like an iterator rather than a loop
colors = ["red", "green", "blue", "purple"]
i = 0
while i < len(colors):
    print(colors[i])
    i += 1

for i in range(len(colors)):    #BAD implementation
    print(colors[i])

for color in colors:  #OREFERRABLE
    print(color)

#enumeration on a list and use in for
my_list = ['apple', 'banana', 'grapes', 'pear']
for c, value in enumerate(my_list, 1): 
	print(c, value) 
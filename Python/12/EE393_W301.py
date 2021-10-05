# -*- coding: utf-8 -*-

#sample strings
s1,s2,s3="hello","Ozyegin Univ.", "Python is the best, isn't it?"

# create an empty list (two ways) 
empty_list = [] 
empty_list2 = list()

# create a list 
friends = ['ali', 'veli', 'ayse']
print ("Original list=>", friends)

# examine a list 
tmp=friends[0] #first element in the list
n=len(friends) #number of elements in the list
print(tmp,n)

# modify a list (does not return the list) 
friends.append('lisa') # append element to end 
print("lisa added", friends)

friends.extend(['smith', 'john']) #append multiple elements to end 
print("smith and john added",friends)

friends.insert(0, 'can') # insert elem at index 0 (shifts everything right)
print("can added at zero=>",friends)

friends.remove('veli') 
print("veli removed", friends)

print("zeroth is popped&removed=>", friends.pop(0) )
print(friends)

del friends[0] 
print("zeroth is deleted=>", friends)

friends[0] = 'mehmet'
print("zeroth is updated to -mehmet-=>", friends)

# concatenate lists (slower than 'extend' method) 
allfriends = friends + ['ned', 'jon', 'snow']

# find elements in a list 
n=friends.count('ali') # counts the number of instances
allfriends.index('ned') # # returns index of first instance

# list slicing [start:end:step] 
weekdays = ['mon', 'tues', 'wed', 'thurs', 'fri'] 
weekdays[0] # element 0 
weekdays[0:3] # elements 0, 1, 2 
weekdays[:3] # elements 0, 1, 2 
weekdays[3:] # elements 3, 4 
weekdays[-1] # last element (element 4) 
weekdays[::2] # every 2nd element (0, 2, 4) 
weekdays[::-1] # backwards (4, 3, 2, 1, 0)

# sort a list in place (modifies but does not return the list) 
allfriends.sort() 
allfriends.sort(reverse=True) # sort in reverse 
allfriends.sort(key=len) # sort by a key (value is a function name)

# insert into an already sorted list, and keep it sorted 
num = [10, 20, 40, 50] 
from bisect import insort #binary search algorithm 
insort(num, 30)

# create a second reference to the same list 
same_num = num 
same_num[0] = 0 # modifies both 'num' and 'same_num'

# copy a list (two ways) 
new_num = num[:] 
new_num = list(num)

# examine objects 
num is same_num #returns True (checks if they are the same object) 
num is new_num  #returns False
num == same_num #returns True (checks if they’ve same contents)
num == new_num  #returns True

# split a string into a list of substrings separated by a delimiter 
s='I like you'
s.split(' ') # returns ['I', 'like', 'you'] 
s.split() # equivalent (since space is the default delimiter) 
s2 = 'a, an, the' 
s2.split(',') # returns ['a', ' an', ' the']

# join a list of strings into one string using a delimiter 
fav = ['game', 'of', 'thrones'] 
' '.join(fav) # returns game of thrones'

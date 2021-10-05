# Oct 24, 2018
# EE393
#zipping lists
list1=['a','b','c','d',1,-1]
list2=[1,2,3,4,5,6]
list3=['x','y',0,1,'z', 't']
a=zip(list1, list2, list3)
print (list(a))

a = [1]
b = ["2", 3]
aa=list(zip(a,b))
print (aa)

#There are several methods for creating a dictionary:
#1- Standard way
myDict = {}	# empty dictionary
ymyDict = {1: 'ali', 2: 'veli'}	# dictionary with integer keys
myDict = {'name': 'Jon', 1: [2, 4, 3]} # dictionary with mixed keys

#2- Using dict() function
myDict = dict({1:'OzU', 2:'Ozyegin'})

#3- Using sequence having each item as a pair in a dict()
myDict = dict([(1,'Ozu'), (2,'Ozyegin')]) 

#4- Comprehensions
mySquares = {x: x*x for x in range(6)} 
print(mySquares) # Output: {0: 0, 1: 1, 2: 4, 3: 9, 4: 16, 5: 25}

#which is equivalent to:

mySquares = {}
for x in range(6):   
    mySquares[x] = x*x 

##Accessing items in a dictionary:
#We can use get() method and keys.

myDict = {'name':'Ali', 'age': 19}
print(myDict['name']) 		# Output will be Ali -> it is a key
print(myDict.get('age')) 	# Output will be 19 -> it is a value

#Note that trying to access keys which doesn't exist throws error (NameError)

#Adding/updating items:

#We can add new items or change the value of existing items using regular assignment operator. 
#If the key is already present, value gets updated, 
#else a new key: value pair is added to the dictionary.

myDict = {'name':'Ali', 'age': 19}
myDict['school'] = 'OzU'  	#adds a new item
myDict['age'] = 20		#updates an existing item

#Using pop(), we can remove a particular item and get the value of 
#the removed item at the same time.
 
mySquares = {1:1, 2:4, 3:9, 4:16, 5:25}
#removes item with key=4 and returns the removed item
a=mySquares.pop(4)
print (a) # Output is -> 16 
print (mySquares) # Output is {1: 1, 2: 4, 3: 9, 5: 25}
 
#The method, popitem() can be used to remove the last item 
#and return its value form the dictionary. 
 
mySquares = {1:1, 2:4, 3:9, 4:16, 5:25}  # create a dictionary of squares
a=mySquares.popitem() #removes item with key=4 and returns the removed item
print (a) # Output is -> (5,25) 
print (mySquares) # Output is {1: 1, 2: 4, 3: 9, 4: 16}
 
 
#All the items can be removed at once using the clear() method.
 
mySquares.clear()
print(mySquares) # outputs as {}

#We can also use the del keyword to remove individual items or the entire dictionary itself.

# delete a particular item
mySquares = {1:1, 2:4, 3:9, 4:16, 5:25}  # create a dictionary of squares
del mySquares[2] # (2,4) is deleted. 
print (mySquares) # Outputs {1: 1, 3: 9, 4: 16, 5: 25}
del mySquares	# deleted entire dictionary
###print (mySquares) #gives a NameError

#Some Important dictionary methods (i.e. you’ll use it .method_name() )
#items() : Return a new view of the dictionary's items (key, value).
mySquares = {1:1, 2:4, 3:9, 4:16, 5:25}
print(mySquares.items()) # dict_items([(1, 1), (2, 4), (3, 9), (4, 16), (5, 25)])

#keys() : Return a new view of the dictionary's keys (key).
#values() : Return a new view of the dictionary's values.
mySquares = {1:1, 2:4, 3:9, 4:16, 5:25} 
print(mySquares.keys()) # dict_keys([1, 2, 3, 4, 5])
print(mySquares.values()) # dict_keys([1, 2, 3, 4, 5])
 
#copy() : Creates a shallow copy.
mySquares = {1:1, 2:4, 3:9, 4:16, 5:25}  #
aa= mySquares.copy()
print (aa) # {1: 1, 2: 4, 3: 9, 4: 16, 5: 25}

mySquares = {1:1, 2:4, 3:9, 4:16, 5:25}
bb={}.fromkeys(mySquares)
print (bb) 

mySquares = {1:1, 2:4, 3:9, 4:16, 5:25}  # create a dictionary of squares
bb={}.fromkeys(mySquares, 'hello')
print (bb) # Outputs {1: 'hello', 2: 'hello', 3: 'hello', 4: 'hello', 5: 'hello'}



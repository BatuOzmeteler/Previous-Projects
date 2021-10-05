
import math
def length():
    while True:
        try:
            v1 = input('Please enter a 2x1 vector, separate values by white space:')
            v1 = v1.split()
            if len(v1) != 2:
                print('Please enter only 2 values, no more or less.')
                break
            length = math.sqrt((int(v1[0]))**2 + (int(v1[1]))**2)
            print('Length of the vector is :' + str(length))
            break
        except ValueError:
            print('Please enter an integer value.')
            break


from math import cos , sin , radians
def rotate():
   while True:
        try:    
            v1 = input('Please enter a 2x1 vector, separate values by white space(For existing the program type `Quit`):')
            v1 = v1.split()
            if v1[0] == 'Quit':
                print('Goodbye!')
                break
            if len(v1) != 2:
                print('Please enter only 2 values, no more or less.')
                break
            rotate = input('Please enter rotation degree: ')
            rotaterx = [cos(radians(float(rotate))), -sin(radians(float(rotate)))]
            rotatery = [sin(radians(float(rotate))),  cos(radians(float(rotate)))]
            v1x = (float(rotaterx[0]) * float(v1[0])) + (float(v1[1]) * float(rotaterx[1]))
            v1y = (float(rotatery[0]) * float(v1[0])) + (float(v1[1]) * float(rotatery[1]))
            print('New place of the vector is : (' + str(round(v1x,2)) + ',' + str(round(v1y,2)) + ')')
            break
        except:
            print('Please enter an integer value.')
            break

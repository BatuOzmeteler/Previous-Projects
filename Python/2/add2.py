
def add2():
    while True:
        try:
            v1 = input('Please enter first 2x1 vector, separate values by white space:')
            v1 = v1.split()
            if len(v1) != 2:
                print('Please enter only 2 values, no more or less.')
                break
            
            v2 = input('Please enter second 2x1 vector, separte values by white spaces:')
            v2 = v2.split()
            if len(v2) != 2:
                print('Please enter 2 values, no more or less.')
                break
            
            v3 = [0,0]
            for i in range (int(2)):
                v3[i] = int(v1[i]) + int(v2[i])
            print('Sum of two vectors are :' + str(v3))
            break
        
        except ValueError:
            print('Please enter integers for vector values.')
            break

            

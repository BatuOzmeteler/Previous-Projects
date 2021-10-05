
def printv():
    while True:
        try:
            v1 = input('Please enter a 2x1 vector, separate values by white space:')
            v1 = v1.split()
            if len(v1) != 2:
                print('Please enter only 2 values, no more or less.')
                break
            if v1[0].isdigit() and v1[1].isdigit():
                print('Vector form is: (' + str(v1[0]) + ',' + str(v1[1]) + ')')
                break
            else:
                print('Please enter an integer value.')
                break
            
        except ValueError:
            print ('Please enter an integer value.')
            break

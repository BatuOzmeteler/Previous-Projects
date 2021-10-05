
def transpose():
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
            dx = input('Please enter dx value:')
            if len(dx) != 1:
                print('Please enter only 1 values, no more or less.')
                break
            dy = input('Please enter dy value:')
            if len(dy) != 1:
                print('Please enter only 1 values, no more or less.')
                break
            v1[0] = int(v1[0]) + int(dx)
            v1[1] = int(v1[1]) + int(dy)
            print ('New direction of vector is : (' + str(v1[0]) + ',' + str(v1[1]) + ')' )
            break
        except:
            print('Please enter an integer value')
            break

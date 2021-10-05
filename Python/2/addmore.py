

def addmore():
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

            v3 = [0,0]
            while True:
                v2 = input('Please enter a 2x1 vector, separate values by white spaces(For existing the program type `Quit`):')
                v2 = v2.split()
                if v2[0] == 'Quit':
                    print('Final sum of all vectors are:' + str(v1) + 'Goodbye!')
                    break
                if len(v2) != 2:
                    print('Please enter 2 values, no more or less.')
                    break
                for i in range (int(2)):
                    v3[i] = int(v1[i]) + int(v2[i])
                print('Sum of vectors are now:' + str(v3))
                v1 = v3
            break

                
        except:
            print('Please enter an integer value.')
            break

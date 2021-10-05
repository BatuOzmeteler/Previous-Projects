
def scalarp():
    while True:
        try:
            v1 = input('Please enter first 2x1 vector, separate values by white space:')
            v1 = v1.split()
            if len(v1) != 2:
                print('Please enter only 2 values, no more or less.')
                break
            
            mul = input('Please enter the scalar value for multiplication:')
            if str(mul).isdigit():
                for i in range (int(2)):
                    v1[i] = int(v1[i]) * int(mul)
                print ('Scalar multiplication of vector is :' + str(v1))
                break
            else:
                print('Please enter an integer for scalar value.')
                break
            
        except ValueError:
            print('Please enter integer values.')
            break
                        

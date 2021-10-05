

def function3():
    while True:
        try:
            fib = [ '0', '1', '1']
            n = int(input('Please enter first integer value:'))
            m = int(input('Please enter second integer value:'))
            if int(n) < 0 or int(m) < 0:
                print ('Please enter positive integers.')
                break
            if str(n).isdigit() and str(m).isdigit():
                for i in range (int(m)):
                    x = int(fib[-1]) + int(fib[-2])
                    fib.append(x)
                    if int(x)>int(m):
                        del fib[-1]
                        break
                for j in range (int(m)):
                    if int(fib[0])<int(n):
                        del fib[0]
                    else:
                        break            
                print( len(fib) )
                break
        except ValueError:
            print('Please enter an positive integer.')
            break

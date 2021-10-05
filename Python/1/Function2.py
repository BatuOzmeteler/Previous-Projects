
def function2():
    while True:
        try:
            fib = [ 0, 1,]
            n = int(input('Please enter first integer value:'))
            m = int(input('Please enter second integer value:'))
            if int(n) < 0 or int(m) < 0:
                print('Please enter a positive integer.')
                break
            if int(n) > int (m):
                print('First integer have to be smaller than second integer.')
                break
            if str(n).isdigit() and str(m).isdigit():
                for i in range (int(m)):
                    x = int(fib[-1]) + int(fib[-2])
                    fib.append(x)
                    if int(x)>int(m):
                        del fib[-1]
                        break        
                for j in range (int( m )):
                    y = int(fib[0])
                    if int(y)<int(n):
                        del fib[0]
                    else:
                        break
                print(fib)
                break
        except ValueError:
            print('Please enter an positive integer.')
            break

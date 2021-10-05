

def function1():
    while True:
        try:
            fib = ['0', '1', '1']
            n = int(input('Please enter an integer to check if it is a Fibonacci number:'))
            if int(n) == 1 or int(n) == 0:
                return True
                break
            elif int(n)< 0:
                return False
                break
            else:
                if str(n).isdigit():
                    for i in range (int(n)):
                        x = int(fib[-1]) + int(fib [-2])
                        fib.append(x)
                        if int(x)>=int(n):
                            if int(fib[-1])== int(n):
                                return True
                                break
                            else:
                                return False
                                break
        except ValueError:
            print('Please enter an positive integer')
            break

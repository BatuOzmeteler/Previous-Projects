

import matplotlib.pyplot as plt
import numpy as np
def poly(x1):
    while True:
        try:
            degree = open('Degree.txt')
            degree = degree.readline()
            coef = open('Coefficients.txt')
            coef  = coef.readline()
            coef  = coef.split(',')
            value = 0
            
            poly = []
            for x in range (0,x1,1): # for f(x), x will take value 0 to x1
                for i in range (0, (int(degree)+1), 1):
                    value = value + ( int(coef[i]) * ( int(x) ** ( int(degree)-i) ) )
                poly.append(value)
                value = 0
            
            dpoly = []
            for x in range (0,x1,1): # for f'(x), x will take value 0 to x1
                for i in range (0, (int(degree)), 1):
                    if x == 0:
                        value = coef[-2]
                        break
                    else:
                        value = value + ( int(coef[i]) *(int(degree)-i) * ( int(x) ** ( int(degree)-i-1) ) )
                dpoly.append(int(value))
                value = 0
            
            plt.plot(poly)
            plt.title('Polynomial')
            plt.ylabel('f(x)')
            plt.xlabel('x')
            plt.show()
            
            plt.plot(dpoly)
            plt.title('Derivation of Polynomial')
            plt.ylabel("f'(x)")
            plt.xlabel('x')
            plt.show()
            
            break
        except:
            print('Please enter valid Degree and Coefficent values.')
            break

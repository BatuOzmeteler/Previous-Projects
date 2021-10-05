
import matplotlib.pyplot as plt
import numpy as np
import collections
from polynomial import polynomial
from dPolynomial import dPolynomial
def hw4(x1,x2):
    while True:
        try:
            degree = open('Degree.txt')
            degree = degree.readline()
            axis = []
    
            possibleRoots = []
            for i in range (x1,x2):
                guess = i
                for j in range(30):
                    if float(dPolynomial(guess)) != 0:
                        nextGuess = guess - (polynomial(guess) / dPolynomial(guess))
                        possibleRoots.append(round(nextGuess,1))
                        guess = nextGuess
                        
            counter = collections.Counter(possibleRoots)
            counter = counter.most_common(int(degree))

            roots = []
            for i in range (int(degree)):
                if counter[i][1] > 30:
                    roots.append(counter[i][0])

            roots1 = ''
            if len(roots) > 0:
                for i in range (len(roots)):
                    roots1 = roots1 + 'Root Found at x = ' + str(roots[i])+'\n'
            else:
                print('No Roots found')
            print(str(roots1))
    
            poly = []
            for i in range(x1*10,x2*10):
                i = i/10
                value = polynomial(i)
                poly.append(value)
                axis.append(i)

            dpoly = []
            for i in range(x1*10,x2*10):
                i = i/10
                value = dPolynomial(i)
                dpoly.append(value)
    
            plt.plot(axis,poly)
            plt.title('Polynomial')
            plt.ylabel('f(x)')
            plt.xlabel('x')
            plt.xlim(x1, x2)
            plt.ylim(x1, x2)
            plt.axvline(0)
            plt.axhline(0)
            for i in range(len(roots)):       
                circ1 = plt.Circle((roots[i],0),0.5)
                plt.gca().add_patch(circ1)
            plt.text(0,0,roots1)
            plt.show()
                    
            plt.plot(axis,dpoly)
            plt.title('Derivation of Polynomial')
            plt.ylabel("f'(x)")
            plt.xlabel('x')
            plt.axvline(0)
            plt.axhline(0)
            plt.show()
            
            break
        except:
            print('Please enter valid Degree, Coefficent, and input values.')
            break        
            
    

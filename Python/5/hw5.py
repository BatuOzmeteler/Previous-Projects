
from tkinter import *
import math

class calculator:
    def writeIt(self,x):
        self.equals.insert(END, x)

    def clear(self):
        problem = self.equals.get()[:-1]
        self.equals.delete(0, END)
        self.equals.insert(0, problem)

    def clearAll(self):
        self.equals.delete(0,END)

    def equalsTo(self):
        try:
            ans = eval(str(self.equals.get()))
            self.equals.delete(0, END)
            self.equals.insert(0, ans)
        except SyntaxError or NameError:
            self.equals.delete(0, END)
            self.equals.insert(0, 'Invalid Input!')
        else:
            self.equals.delete(0, END)
            self.equals.insert(0, ans)

    def decimalToBinary(self):
        try:
            ans = eval(str(self.equals.get()))
            binary = str(bin(ans)[2:])
            self.equals.delete(0, END)
            self.equals.insert(0, binary)
        except SyntaxError or NameError:
            self.equals.delete(0, END)
            self.equals.insert(0, 'Invalid Input!')
        except:
            self.equals.delete(0, END)
            self.equals.insert(0, 'Please input integer!')

    def __init__(self,master):
        master.title('Calculator')
        master.geometry()
        self.equals = Entry(master)
        self.equals.grid(column=0, row=0, columnspan=10)

        Button(master, text ='1',width=3,command=lambda:self.writeIt('1')).grid(column=0,row=2)
        Button(master, text ='2',width=3,command=lambda:self.writeIt('2')).grid(column=1,row=2)
        Button(master, text ='3',width=3,command=lambda:self.writeIt('3')).grid(column=2,row=2)
        Button(master, text ='4',width=3,command=lambda:self.writeIt('4')).grid(column=0,row=3)
        Button(master, text ='5',width=3,command=lambda:self.writeIt('5')).grid(column=1,row=3)
        Button(master, text ='6',width=3,command=lambda:self.writeIt('6')).grid(column=2,row=3)
        Button(master, text ='7',width=3,command=lambda:self.writeIt('7')).grid(column=0,row=4)
        Button(master, text ='8',width=3,command=lambda:self.writeIt('8')).grid(column=1,row=4)
        Button(master, text ='9',width=3,command=lambda:self.writeIt('9')).grid(column=2,row=4)
        Button(master, text ='.',width=3,command=lambda:self.writeIt('.')).grid(column=0,row=5)
        Button(master, text ='0',width=3,command=lambda:self.writeIt('0')).grid(column=1,row=5)
        Button(master, text ='00',width=3,command=lambda:self.writeIt('00')).grid(column=2,row=5)
        Button(master, text ='+',width=3,command=lambda:self.writeIt('+')).grid(column=3,row=2)
        Button(master, text ='-',width=3,command=lambda:self.writeIt('-')).grid(column=3,row=3)
        Button(master, text ='/',width=3,command=lambda:self.writeIt('/')).grid(column=3,row=4)
        Button(master, text ='*',width=3,command=lambda:self.writeIt('*')).grid(column=3,row=5)
        Button(master, text ='Bnr',width=3, command=lambda:self.decimalToBinary()).grid(column=4,row=2)
        Button(master, text='C', width=3, command=lambda:self.clear()).grid(column=5, row=2)
        Button(master, text='AC', width=3, command=lambda:self.clearAll()).grid(column=5, row=3)
        Button(master, text =')',width=3,command=lambda:self.writeIt(')')).grid(column=4,row=3)
        Button(master, text ='(',width=3,command=lambda:self.writeIt('(')).grid(column=4,row=4)
        Button(master, text ='=',width=3, command=lambda:self.equalsTo()).grid(column=4,row=5)

#Main loop
master = Tk()
calculator(master)
master.mainloop()

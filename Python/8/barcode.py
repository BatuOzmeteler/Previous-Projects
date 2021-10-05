# -*- coding: utf-8 -*-
"""
Created on Tue Nov  6 18:13:37 2018

Author: Mehmet Batu Özmeteler
Class: EE393.A 
School No: S009924
Department: Electrical & Electronics Engineering 
"""

def isValidBar(barcode): #checks if the barcode format is correct
    if(len(barcode) < 14 and len(barcode) > 11 and barcode.isdigit()):
        return True
    else:
        return False


def printStatusBar(barcode):#prints the status of the barcode
        if(isValidBar(barcode)):#checks if the barcode format is valid
            sumEven = 0 
            sumOdd = 0
            for x in range(0,len(barcode)-1):#adds the digits up in two separate sums
                if(x % 2 == 0):
                    sumEven = sumEven + int(barcode[x])
                else:
                    sumOdd = sumOdd + int(barcode[x])
            
            if(len(barcode) == 12):#for 12 digit barcodes
                temp1 = 3 * (sumEven % 10)#required 
                temp2 = temp1 + (sumOdd % 10)#calculations
                if(int(barcode[-1]) + (temp2 % 10) == 10):#mod10 used to get the right most digit
                    print("Barcode is a valid 12 digit barcode")#printing the valid status 
                else:
                    print("Invalid 12 digit barcode")#printing the invalid status 
            else:#for 13 digit barcodes
                temp1 = 3 * (sumOdd % 10)#required
                temp2 = temp1 + (sumEven % 10)#calculations
                if(int(barcode[-1]) + (temp2 % 10) == 10):
                    print("Barcode is a valid 13 digit barcode")#printing the valid status 
                else:
                    print("Invalid 13 digit barcode")#printing the invalid status 
        else:
            print("Invalid barcode format")#printing the invalid status 
    
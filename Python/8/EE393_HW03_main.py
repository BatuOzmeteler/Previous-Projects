# -*- coding: utf-8 -*-
"""
Created on Tue Nov  6 19:27:29 2018

Author: Mehmet Batu Özmeteler
Class: EE393.A 
School No: S009924
Department: Electrical & Electronics Engineering 

This program takes a 12 or 13 digit barcode and check if it is valid in format or not. Then,
it checks if it is a valid barcode and prints its status.
"""
try:
    import barcode #importing barcode library
    import sys
except Exception as e:
    print (e.__doc__)
    sys.exit()

bar = input("Give me a 12 or 13 digit barcode: ")#input
barcode.printStatusBar(bar)#printing the barcode status
sys.exit("Progrma terminated")




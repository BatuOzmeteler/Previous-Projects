# -*- coding: utf-8 -*-
"""
Created on Tue Aug 17 20:45:11 2021

@author: mbatu
"""
import base64

def main():
    hi = "Hello World!"
    print(base64.b64decode(hi).decode())
    
    
if __name__ == "__main__":
    main()           
            
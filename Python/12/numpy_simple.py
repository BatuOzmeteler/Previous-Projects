#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Solution of the following linear equation using numpy
3*x0 + x1   + 2*x2 = 9 
x0   + 2*x1 + 4*x2 = 8
-x0  - 2*x1 +2.5*x2= 1
"""

import numpy as np
a = np.array([[3,1,2], [1,2,4] , [-1,-2,2.5]]) 
b = np.array([9,8,1])
x = np.linalg.solve(a, b) 
print (x)


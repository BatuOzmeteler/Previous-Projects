def dPolynomial(x):
    degree = open('Degree.txt')
    degree = degree.readline()
    coef = open('Coefficients.txt')
    coef  = coef.readline()
    coef  = coef.split(',')
    value = 0
    for i in range (0, (int(degree)), 1):
        if x == 0:
            value = coef[-2]
        else:
            value = value + ( eval(coef[i]) * (int(degree)-i) * ( float(x) ** (int(degree)-i-1) ) )
    return float(value)

def polynomial(x):
    degree = open('Degree.txt')
    degree = degree.readline()
    coef = open('Coefficients.txt')
    coef  = coef.readline()
    coef  = coef.split(',')
    value = 0
    for i in range (0, (int(degree)+1), 1):
        value = value + ( eval(coef[i]) * ( float(x) ** ( int(degree)-i) ) )
    return value

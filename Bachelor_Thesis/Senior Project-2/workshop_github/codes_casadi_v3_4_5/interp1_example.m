n = 100;
x = linspace(0, 6*pi, n);
y = sin(x);
randomPermutation = randperm(n);
percentKept = 20;
numKept = round(n*percentKept/100);
xKept = x(randomPermutation(1:numKept));
yKept = y(randomPermutation(1:numKept));

% 'nearest'
% 'linear'
% 'spline'
% 'pchip'

yInterp = interp1(xKept, yKept, x, 'spline');
plot(x, yInterp, 'k-', xKept, yKept, 'ro')

u = [-0.1*exp(-1i*(pi/4)) 1]
v = [-0.1*exp(-1i*(pi/4)) 1];
y = [-0.2 1];

w = conv(u , conv(v,y));
w'

% u = [1 6]
% v = [2 0 0];
% y = [5 0 0 0];
% k = conv(v , conv(u,y))
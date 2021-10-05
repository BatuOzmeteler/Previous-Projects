
A = 1;
B = conv([-0.5 1], [0.09 -0.6 1]);

[r,p,k] = residue(A, B);

x = zeros(100,1);
x(1)=1;
x(2)=1;

y = zeros(100,1);

y(1) = x(1);
for n = 2:101
    y(n)=0.5*y(n-1)+x(n)
end

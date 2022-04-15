clear all
clc

syms x0 x1
syms u0
syms p1 p2 p3
syms lambda_x0 lambda_x1
syms lambda_u0 lambda_u1

f0 = x1;
f1 = p1*(1 - p2*x0*x0)*x1 - p3*x0 + u0;

f = [f0; f1];
x = [x0; x1];
u = [u0];

lambda_x = [lambda_x0; lambda_x1];
lambda_u = [lambda_u0; lambda_u1];

fx = jacobian(f, x);
fu = jacobian(f, u);

out_fx = fx'*lambda_x;
out_fu = fu'*lambda_u;

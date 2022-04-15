clear all
clc

syms x0 x1 x2 x3 x4 x5 x6 x7 
syms u0 u1 u2 
syms lambda_x0 lambda_x1 lambda_x2 lambda_x3 lambda_x4 lambda_x5 lambda_x6 lambda_x7 
syms lambda_u0 lambda_u1 lambda_u2 lambda_u3 lambda_u4 lambda_u5 lambda_u6 lambda_u7 

m = 1;

nx = 8;
nu = 3;

f0 = x1;
f1 = (u0/m)*cos(x7)*sin(x6); 
f2 = x3;
f3 = -(u0/m)*sin(x7);
f4 = x5; 
f5 = (u0/m)*cos(x7)*cos(x6) - 9.81;
f6 = u1;
f7 = u2;

f = [f0; f1; f2; f3; f4; f5; f6; f7];
x = [x0; x1; x2; x3; x4; x5; x6; x7];
u = [u0; u1; u2];
lambda_x = [lambda_x0; lambda_x1; lambda_x2; lambda_x3; lambda_x4; lambda_x5; lambda_x6; lambda_x7];
lambda_u = [lambda_u0; lambda_u1; lambda_u2; lambda_u3; lambda_u4; lambda_u5; lambda_u6; lambda_u7];

fx = jacobian(f, x);
fu = jacobian(f, u);

out_fx = fx'*lambda_x;
out_fu = fu'*lambda_u;



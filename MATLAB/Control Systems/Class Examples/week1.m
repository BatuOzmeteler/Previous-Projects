
A = [-2 1; 1 0];

[V,D] = eig(A)

x0 = [1;1];

alpha=inv(V)*x0;

% plot your solution y(t) for 10 seconds
t = [0:0.01:10];

lambda1 = D(1,1);
lambda2 = D(2,2);

y = alpha(1) * exp(lambda1*t) * V(1,2) + ...
    alpha(2) * exp(lambda2*t) * V(2, 2);

plot(t, y);




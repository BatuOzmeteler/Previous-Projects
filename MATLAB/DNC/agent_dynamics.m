function [y, x_dot] = agent_dynamics(x, u)

% Dynamics
A = [0 1; -1 0]; B = [0; 1]; C = [0 1]; D = 0;

% State Evolution
x_dot = A*x + B*u;
y = C*x;

end
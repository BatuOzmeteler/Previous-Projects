
clear all; clc; close all;

% Initial Values
figure_plot = figure;
C = [0 1];
Ct = blkdiag(C, C, C, C, C);
x0 = [2; 1; 1; 2; 2; 1; 2; 3; 4; 5];
y0 = Ct*x0;

% Simulation Time
timestep = 0.02;
t0 = 0:timestep:10;

% Output Trajectories
y_history = zeros(5, length(t0));

% Input Trajectories
u_history = zeros(5, length(t0));

% Communication Graph and the Laplacian
adj_matrix = [0 1 1 1 1; 1 0 1 1 1; 1 1 0 1 1; 1 1 1 0 1; 1 1 1 1 0];
out_degree = [4 0 0 0 0; 0 4 0 0 0; 0 0 4 0 0; 0 0 0 4 0; 0 0 0 0 4];
G = graph(adj_matrix);
figure(figure_plot);
plot(G)

L = out_degree - adj_matrix;

%--------------Simulation for Input and Output Trajectories--------------%

y_current = y0;
x_current = x0;

A = [0 1; -1 0]; B = [0; 1]; C = [0 1]; D = 0;
tf_agent = ss(A, B, C, D);
z_agent = c2d(tf_agent, 0.02);

Ad = z_agent.A;
Bd = z_agent.B;
Cd = z_agent.C;
Adt = blkdiag(Ad, Ad, Ad, Ad, Ad);
Bdt = blkdiag(Bd, Bd, Bd, Bd, Bd);
Cdt = blkdiag(Cd, Cd, Cd, Cd, Cd);

for i = 1:length(t0)
    
    u_current = -L*y_current;
    
    u_history(:, i) = u_current;
    y_history(:, i) = y_current;
    
%     [y1, x_dot1] = agent_dynamics(x_current(:,1), u_current(1));
%     [y2, x_dot2] = agent_dynamics(x_current(:,2), u_current(2));
%     [y3, x_dot3] = agent_dynamics(x_current(:,3), u_current(3));
%     [y4, x_dot4] = agent_dynamics(x_current(:,4), u_current(4));
%     [y5, x_dot5] = agent_dynamics(x_current(:,5), u_current(5));
    y_current = Cdt*x_current;
    x_current = Adt*x_current + Bdt*u_current ;
    
end    

subplot(2,1,1)
plot(t0, y_history(1,:), t0, y_history(2,:), t0, y_history(3,:), t0, y_history(4,:), t0, y_history(5,:))
xlabel('Time(s)')
ylabel('Output')
title('Synchronization of a Diffusively Coupled Linear System - Outputs')
legend('Agent1', 'Agent2', 'Agent3', 'Agent4', 'Agent5')

subplot(2,1,2)
plot(t0, u_history(1,:), t0, u_history(2,:), t0, u_history(3,:), t0, u_history(4,:), t0, u_history(5,:))
xlabel('Time(s)')
ylabel('Input')
title('Synchronization of a Diffusively Coupled Linear System - Inputs')
legend('Agent1', 'Agent2', 'Agent3', 'Agent4', 'Agent5')

function [y, x_dot] = agent_dynamics(x, u)

% Dynamics
A = [0 1; -1 0]; B = [0; 1]; C = [0 1]; D = 0;

% State Evolution
x_dot = A*x + B*u;
y = C*x;

end

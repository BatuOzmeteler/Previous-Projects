%% Single Robot OCP Formulation without Obstacle Avoidance Constraints
% Authors: 
% - Mehmet Batu Özmeteler
%% Setup
clear all
clc
import casadi.*
%% Parameters
disp('Setting up the Parameters')
t0 = 0;             % initial time
tf = 20;            % end time

dt = 0.02;          % integration step size => 0.02
h = 0.05;           % step size  

N = (tf - t0)/h;    % horizon length
t = t0:h:tf;        % time span
nx = 8;             % dimension of the state vector
nu = 3;             % dimension of the input vector
m = 1;              % mass of the robot (kg)
g = 9.81;           % gravitational acceleration (m/s^2)
epsilon = 0.5;      % weight of the regularization term
%% Initial Conditions
x_0 = 0; xd_0 = 0;
y_0 = 0; yd_0 = 0;
z_0 = 0; zd_0 = 0;

theta_0 = 0; phi_0 = 0;

x_init = [x_0; xd_0; y_0; yd_0; z_0; zd_0; theta_0; phi_0];

%% State Goal Vector
x_g = 30; xd_g = 0;
y_g = 20; yd_g = 0;
z_g = 30; zd_g = 0;

theta_g = 0;
phi_g = 0;

x_goal = [x_g; xd_g; y_g; yd_g; z_g; zd_g; theta_g; phi_g];

%% Input Goal Vector
u1_g = 0;
u2_g = 0;
u3_g = 0;

u_goal = [u1_g; u2_g; u3_g];

%% State Bounds
x_min = -50; x_max = 50;
xd_min = -5; xd_max = 5;

y_min = -50; y_max = 50;
yd_min = -5; yd_max = 5;

z_min = 0; z_max = 100;
zd_min = -5; zd_max = 5;

theta_min = -pi/8; theta_max = pi/8;
phi_min = -pi/8; phi_max = pi/8;

x_min = [x_min; xd_min; y_min; yd_min; z_min; zd_min; theta_min; phi_min];
x_max = [x_max; xd_max; y_max; yd_max; z_max; zd_max; theta_max; phi_max];

%% Input Bounds
u1_min = 2; u1_max = 15;              % thrust acceleration (m/s^2)
u2_min = -pi/4; u2_max = pi/4;        % angular velocity on pitch (rad/s)
u3_min = -pi/4; u3_max = pi/4;        % angular velocity on roll (rad/s)

u_min = [u1_min; u2_min; u3_min]; 
u_max = [u1_max; u2_max; u3_max];

%% OCP Formulation
disp('Formulating the OCP')
ocp = casadi.Opti();         % OCP object

X = ocp.variable(nx, N+1);   % symbolic state matrix
U = ocp.variable(nu, N);     % symbolic input matrix
J = 0;                       % cumulative cost 
Q = 1*eye(nx);               % weight matrix for states
R = 1*eye(nu);               % weight matrix for inputs

for i=1:N
    % enforce constraints at every point for dynamics    
    xf = rk4(@(t,x,u)quadrotor(t, x, u),h,t(i),X(:,i),U(:,i));
    ocp.subject_to( X(:,i+1) == xf );
    
    % state constraints
    ocp.subject_to( x_min <= X(:,i) <= x_max );
    
    % input constraints
    ocp.subject_to( u_min <= U(:,i) <= u_max );
    
    % cost function -- CHECK THE CALCULATION AGAIN FROM EXERCISE 4.2
    x = X(:,i+1);
    u = U(:,i);
    J = J + ( norm((x - x_goal)'*Q*(x - x_goal)) ) +  epsilon*( norm((u - u_goal)'*R*(u - u_goal)) );
end
% initial conditions
ocp.subject_to( X(:,1) == x_init );
% terminal constraints
ocp.subject_to( X(:,N+1) == x_goal );
disp('OCP Formulation Completed')
%% Solve OCP 
disp('Solving the OCP')
% initial guess
ocp.set_initial(X,repmat(x_init, 1, N+1));
ocp.set_initial(U,repmat(0.5*(u_max + u_min), 1, N));    
% set solver
p_opts = struct('expand', true);
s_opts = struct('max_iter', 200);
ocp.solver('ipopt', p_opts, s_opts);
%ocp.solver('ipopt');
% set objective
ocp.minimize(J);
% solve ocp
sol = ocp.solve();
% get solution
Xsol = sol.value(X);
Usol = sol.value(U);

%% Plot the Results
plotStates(t, Xsol, N, x_init, x_goal)
plotInputs(t, Usol)

% Runge-Kutta Integrator
function xf = rk4(ode, h, t, x, u)
    k1 = ode(t, x, u);
    k2 = ode(t, x+h/2*k1, u);
    k3 = ode(t, x+h/2*k2, u);
    k4 = ode(t, x+h*k3, u);
    xf = x + h/6 * (k1 + 2*k2 + 2*k3 + k4);
end

function plotStates(t, X, N, x_init, x_goal)
    line_width = 1.5;

    figure();
    sgtitle('States') 
    subplot(4,2,1)
    hold on
    plot(t, X(1,:),'Linewidth', line_width)
    plot(t(1), x_init(1), 'xg')
    plot(t(N+1), x_goal(1), 'xr')
    xlabel('time (s)')
    ylabel('x-position (m)')
    hold off
    
    subplot(4,2,2)
    hold on
    plot(t, X(2,:),'Linewidth', line_width)
    plot(t(1), x_init(2), 'xg')
    plot(t(N+1), x_goal(2), 'xr')
    xlabel('time (s)')
    ylabel('x-velocity (m/s)')
    hold off
    
    subplot(4,2,3)
    hold on
    plot(t, X(3,:),'Linewidth', line_width)
    plot(t(1), x_init(3), 'xg')
    plot(t(N+1), x_goal(3), 'xr')
    xlabel('time (s)')
    ylabel('y-position (m)')
    hold off
    
    subplot(4,2,4)
    hold on
    plot(t, X(4,:),'Linewidth', line_width)
    plot(t(1), x_init(4), 'xg')
    plot(t(N+1), x_goal(4), 'xr')
    xlabel('time (s)')
    ylabel('y-velocity (m)')
    hold off
    
    subplot(4,2,5)
    hold on
    plot(t, X(5,:),'Linewidth', line_width)
    plot(t(1), x_init(5), 'xg')
    plot(t(N+1), x_goal(5), 'xr')
    xlabel('time (s)')
    ylabel('z-position (m)')
    hold off
    
    subplot(4,2,6)
    hold on
    plot(t, X(6,:),'Linewidth', line_width)
    plot(t(1), x_init(6), 'xg')
    plot(t(N+1), x_goal(6), 'xr')
    xlabel('time (s)')
    ylabel('z-velocity (m)')
    hold off
    
    subplot(4,2,7)
    hold on
    plot(t, X(7,:),'Linewidth', line_width)
    plot(t(1), x_init(7), 'xg')
    plot(t(N+1), x_goal(7), 'xr')
    xlabel('time (s)')
    ylabel('theta (rad)')
    hold off
    
    subplot(4,2,8)
    hold on
    plot(t, X(8,:),'Linewidth', line_width)
    plot(t(1), x_init(8), 'xg')
    plot(t(N+1), x_goal(8), 'xr')
    xlabel('time (s)')
    ylabel('phi (rad)')
    hold off
    
    
end

function plotInputs(t, U)
    line_width = 1.5;

    figure();
    sgtitle('Inputs') 
    subplot(3,1,1)
    hold on
    plot(t(1:end-1), U(1,:),'Linewidth', line_width)
    xlabel('time (s)')
    ylabel('thrust acceleration')
    hold off
    
    subplot(3,1,2)
    hold on
    plot(t(1:end-1), U(2,:),'Linewidth', line_width)
    xlabel('time (s)')
    ylabel('w_{theta} (rad/s)')
    hold off
    
    subplot(3,1,3)
    hold on
    plot(t(1:end-1), U(3,:),'Linewidth', line_width)
    xlabel('time (s)')
    ylabel('w_{phi} (rad/s)')
    hold off
end

%% Single Robot OCP Formulation without Obstacle Avoidance Constraints
% Authors: 
% - Mehmet Batu Özmeteler
% - Shehabeldine Housein
%% Setup
clear all
clc
import casadi.*
%% Parameters
disp('Setting up the Parameters')
t0 = 0;             % initial time
tf = 20;            % end time

h = 0.2;           % step size  

N = (tf - t0)/h;    % horizon length
t = t0:h:tf;        % time span

num_of_robots = 2;  % number of robots
d = 1;              % distance for the formation constraint (m)

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

x_init_0 = [x_0; xd_0; y_0; yd_0; z_0; zd_0; theta_0; phi_0];
x_init_1 = formation(x_init_0, d, num_of_robots, 0);
x_init_2 = formation(x_init_0, d, num_of_robots, 1);

%% State Goal Vector
x_g_s1 = 10; xd_g_s1 = 0;
y_g_s1 = 15; yd_g_s1 = 0;
z_g_s1 = 10; zd_g_s1 = 0;
theta_g_s1 = 0; phi_g_s1 = 0;

x_g_s2 = -15; xd_g_s2 = 0;
y_g_s2 = 3; yd_g_s2 = 0;
z_g_s2 = 2; zd_g_s2 = 0;
theta_g_s2 = 0; phi_g_s2 = 0;

x_goal_0_s1 = [x_g_s1; xd_g_s1; y_g_s1; yd_g_s1; z_g_s1; zd_g_s1; theta_g_s1; phi_g_s1];
x_goal_0_s2 = [x_g_s2; xd_g_s2; y_g_s2; yd_g_s2; z_g_s2; zd_g_s2; theta_g_s2; phi_g_s2];

x_goal_1_s1 = formation(x_goal_0_s1, d, num_of_robots, 0);
x_goal_2_s1 = formation(x_goal_0_s1, d, num_of_robots, 1);

x_goal_1_s2 = formation(x_goal_0_s2, d, num_of_robots, 0);
x_goal_2_s2 = formation(x_goal_0_s2, d, num_of_robots, 1);

%% State Bounds
x_min = -50; x_max = 50;
xd_min = -5; xd_max = 5;

y_min = -50; y_max = 50;
yd_min = -5; yd_max = 5;

z_min = 0; z_max = 100;
zd_min = -5; zd_max = 5;

theta_min = -pi/4; theta_max = pi/4;
phi_min = -pi/4; phi_max = pi/4;

x_min = [x_min; xd_min; y_min; yd_min; z_min; zd_min; theta_min; phi_min];

x_max = [x_max; xd_max; y_max; yd_max; z_max; zd_max; theta_max; phi_max];

%% Input Bounds
u1_min = 0; u1_max = 15;        % thrust acceleration (m/s^2)
u2_min = -2; u2_max = 2;        % angular velocity on pitch (rad/s)
u3_min = -2; u3_max = 2;        % angular velocity on roll (rad/s)

u_min = [u1_min; u2_min; u3_min]; 

u_max = [u1_max; u2_max; u3_max];

%% OCP Formulation
disp('Formulating the OCP')
ocp = casadi.Opti();         % OCP object

X_1 = ocp.variable(nx, N+1); % symbolic system matrix
X_2 = ocp.variable(nx, N+1); % symbolic system matrix

U_1 = ocp.variable(nu, N);   % symbolic input matrix
U_2 = ocp.variable(nu, N);   % symbolic input matrix

J = 0;                       % total cumulative cost
J_1 = 0;                     % cumulative cost for agent 0
J_2 = 0;                     % cumulative cost for agent 1

Q = 10*eye(nx);              % weight matrix for states
R = 0.01*eye(nu);            % weight matrix for inputs

for i=1:N
    % enforce constraints at every point for dynamics    
    xf_1 = rk4(@(t,x,u)quadrotor(t, x, u),h,t(i),X_1(:,i),U_1(:,i));
    xf_2 = rk4(@(t,x,u)quadrotor(t, x, u),h,t(i),X_2(:,i),U_2(:,i));
    
    ocp.subject_to( X_1(:,i+1) == xf_1 );
    ocp.subject_to( X_2(:,i+1) == xf_2 );
    
    ocp.subject_to( X_2(1,i) - X_1(1,i) - d == 0 );
    ocp.subject_to( X_2(3,i) - X_1(3,i) == 0 );
    
    % state constraints
    ocp.subject_to( x_min <= X_1(:,i) <= x_max );
    ocp.subject_to( x_min <= X_2(:,i) <= x_max );
    
    % input constraints
    ocp.subject_to( u_min <= U_1(:,i) <= u_max );
    ocp.subject_to( u_min <= U_2(:,i) <= u_max );
    
    x_1 = X_1(:,i+1);
    x_2 = X_2(:,i+1);
    
    u_1 = U_1(:,i);
    u_2 = U_2(:,i);
    
    if i < (N/2 + 1)
        J_1 = J_1 + (x_1 - x_goal_1_s1)'*Q*(x_1 - x_goal_1_s1) +  epsilon*(u_1'*R*u_1);
        J_2 = J_2 + (x_2 - x_goal_2_s1)'*Q*(x_2 - x_goal_2_s1) +  epsilon*(u_2'*R*u_2);
    else
        J_1 = J_1 + (x_1 - x_goal_1_s2)'*Q*(x_1 - x_goal_1_s2) +  epsilon*(u_1'*R*u_1);
        J_2 = J_2 + (x_2 - x_goal_2_s2)'*Q*(x_2 - x_goal_2_s2) +  epsilon*(u_2'*R*u_2);
    end
    J = J_1 + J_2;
end
% initial conditions
ocp.subject_to( X_1(:,1) == x_init_1 );
ocp.subject_to( X_2(:,1) == x_init_2 );

% terminal constraints
ocp.subject_to( X_1(:,N/2 + 1) == x_goal_1_s1 );
ocp.subject_to( X_2(:,N/2 + 1) == x_goal_2_s1 );

ocp.subject_to( X_1(:,N+1) == x_goal_1_s2 );
ocp.subject_to( X_2(:,N+1) == x_goal_2_s2 );

disp('OCP Formulation Completed')
%% Solve OCP 
disp('Solving the OCP')
% initial guess
ocp.set_initial(X_1,repmat(x_init_1, 1, N+1));
ocp.set_initial(X_2,repmat(x_init_2, 1, N+1));

ocp.set_initial(U_1,repmat(0.5*(u_max + u_min), 1, N));   
ocp.set_initial(U_2,repmat(0.5*(u_max + u_min), 1, N));  

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
Xsol_1 = sol.value(X_1);
Xsol_2 = sol.value(X_2);

Usol_1 = sol.value(U_1);
Usol_2 = sol.value(U_2);

%% Plot the Results
figure();

goal_vector_1 = [x_goal_1_s1; x_goal_1_s2];
goal_vector_2 = [x_goal_2_s1; x_goal_2_s2];

plotStates(t, Xsol_1, N, x_init_1, goal_vector_1)
hold on;
plotStates(t, Xsol_2, N, x_init_2, goal_vector_2)

figure();
plotInputs(t, Usol_1)
hold on;
plotInputs(t, Usol_2)

% Runge-Kutta Integrator
function xf = rk4(ode, h, t, x, u)
    k1 = ode(t, x, u);
    k2 = ode(t, x+h/2*k1, u);
    k3 = ode(t, x+h/2*k2, u);
    k4 = ode(t, x+h*k3, u);
    xf = x + h/6 * (k1 + 2*k2 + 2*k3 + k4);
end

function plotStates(t, X, N, x_init, x_goal)
    line_width = 1.0;

    sgtitle('States') 
    subplot(4,2,1)
    hold on
    plot(t, X(1,:),'Linewidth', line_width)
    plot(t(1), x_init(1), 'xg')
    plot(t(N/2+1), x_goal(1), 'xr')
    plot(t(N+1), x_goal(9), 'xr')
    xlabel('time (s)')
    ylabel('x-position (m)')
    hold off
    
    subplot(4,2,2)
    hold on
    plot(t, X(2,:),'Linewidth', line_width)
    plot(t(1), x_init(2), 'xg')
    plot(t(N/2+1), x_goal(2), 'xr')
    plot(t(N+1), x_goal(10), 'xr')
    xlabel('time (s)')
    ylabel('x-velocity (m/s)')
    hold off
    
    subplot(4,2,3)
    hold on
    plot(t, X(3,:),'Linewidth', line_width)
    plot(t(1), x_init(3), 'xg')
    plot(t(N/2+1), x_goal(3), 'xr')
    plot(t(N+1), x_goal(11), 'xr')
    xlabel('time (s)')
    ylabel('y-position (m)')
    hold off
    
    subplot(4,2,4)
    hold on
    plot(t, X(4,:),'Linewidth', line_width)
    plot(t(1), x_init(4), 'xg')
    plot(t(N/2+1), x_goal(4), 'xr')
    plot(t(N+1), x_goal(12), 'xr')
    xlabel('time (s)')
    ylabel('y-velocity (m)')
    hold off
    
    subplot(4,2,5)
    hold on
    plot(t, X(5,:),'Linewidth', line_width)
    plot(t(1), x_init(5), 'xg')
    plot(t(N/2+1), x_goal(5), 'xr')
    plot(t(N+1), x_goal(13), 'xr')
    xlabel('time (s)')
    ylabel('z-position (m)')
    hold off
    
    subplot(4,2,6)
    hold on
    plot(t, X(6,:),'Linewidth', line_width)
    plot(t(1), x_init(6), 'xg')
    plot(t(N/2+1), x_goal(6), 'xr')
    plot(t(N+1), x_goal(14), 'xr')
    xlabel('time (s)')
    ylabel('z-velocity (m)')
    hold off
    
    subplot(4,2,7)
    hold on
    plot(t, X(7,:),'Linewidth', line_width)
    plot(t(1), x_init(7), 'xg')
    plot(t(N/2+1), x_goal(7), 'xr')
    plot(t(N+1), x_goal(15), 'xr')
    xlabel('time (s)')
    ylabel('theta (rad)')
    hold off
    
    subplot(4,2,8)
    hold on
    plot(t, X(8,:),'Linewidth', line_width)
    plot(t(1), x_init(8), 'xg')
    plot(t(N/2+1), x_goal(8), 'xr')
    plot(t(N+1), x_goal(16), 'xr')
    xlabel('time (s)')
    ylabel('phi (rad)')
    hold off
end

function plotInputs(t, U)
    line_width = 1.0;

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
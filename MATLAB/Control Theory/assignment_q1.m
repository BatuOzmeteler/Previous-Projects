
clear all 
clc

% Q1------------------------------------------------------------------

% Parameters 
M = 15;     % Mass of the Robot [kg]
theta = 30; % Angle of Inclination of the Plane [degree]
ks = 3;     % Spring Constant [N / M]
g = 10;     % Gravitational Acceleration [m / s2]

% Inputs
% U = F - 5*ks - M*g*sin(theta)
% F                 => Linear Force Provided by the Motor [m3 / min]

% States 
% x       (x1)      => Displacement of the Robot [m]
% v       (x2)      => Velocity of the Robot [m / s]

% State-space Matrices
A_mat = [0 1; -ks/M 0];
B_mat = [0; 1/M];
C_mat = eye(2);
D_mat = [0; 0];
sys_robot = ss(A_mat, B_mat, C_mat, D_mat);
poles_robot = eig(sys_robot);

%---------------------------------------------------------------------

% Q1 - 7) Use MATLAB to plot the closed-loop response. 
% Plot the states and the force supplied by the motor 
% over a time period of 20 seconds.

design1_eig = [-0.8, -1];

%K_d1 = -place(A_mat, B_mat, design1_eig);

K_d1 = [-9 -27];

poles_robot_d1 = eig(ss(A_mat + B_mat*K_d1, B_mat, C_mat, D_mat));
% New eigen values of the controlled robot are at -0.8 and -1.0 

x0 = [-5; 0]; % initial conditions 

t_span = [0 20];
[t, x_d1] = ode15s(@(t,x)ode_sys_controlled(t, x, K_d1), t_span, x0);
U_d1 = K_d1*x_d1';

% U = K*x = F - 5*ks - M*g*sin(theta)
F_d1 = U_d1 + 5*ks + M*g*sin(deg2rad(theta));


% figure(1);
% subplot(4,1,1);
% plot(t, x_d1(:,1),'b-', 'LineWidth', 2);
% xlabel('t'); ylabel('Position');
% 
% subplot(4,1,2);
% plot(t, x_d1(:,2),'b-', 'LineWidth', 2);
% xlabel('t'); ylabel('Velocity');
% 
% subplot(4,1,3);
% plot(t, F_d1, 'b-', 'LineWidth', 2);
% xlabel('t'); ylabel('Force');
% 
% subplot(4,1,4);
% plot(t, U_d1, 'b-', 'LineWidth', 2);
% xlabel('t'); ylabel('U');

%---------------------------------------------------------------------

% Q1 - 9) Design(II): Design a controller for the system such that the
% problem that resulted from Design(I) is mitigated. Specify the eigen
% values of the controlled system (closed-loop poles). The controller shall
% move the robot to the position z = 0 on the inclined plane and the robot
% shall remain there afterwards. Determine the gain matrix K and show all
% the steps of your calculations.

design2_eig = 0.75*design1_eig;

K_d2 = [-3.75, -20.25];

%K_d2 = -place(A_mat, B_mat, design2_eig);

poles_robot_d2 = eig(ss(A_mat + B_mat*K_d2, B_mat, C_mat, D_mat));

%---------------------------------------------------------------------

% Q1 - 10) Use MATLAB to plot the closed-loop response. 
% Plot the states and the force supplied by the motor 
% over a time period of 20 seconds.

[t, x_d2] = ode15s(@(t,x)ode_sys_controlled(t, x, K_d2), t_span, x0);
U_d2 = K_d2*x_d2';

% U = K*x = F - 5*ks - M*g*sin(theta)
F_d2 = U_d2 + 5*ks + M*g*sin(deg2rad(theta));

% figure(2);
% subplot(4,1,1);
% plot(t, x_d2(:,1),'b-', 'LineWidth', 2);
% xlabel('t'); ylabel('Position');
 
% subplot(4,1,2);
% plot(t, x_d2(:,2),'b-', 'LineWidth', 2);
% xlabel('t'); ylabel('Velocity');
% 
% subplot(4,1,3);
% plot(t, F_d2, 'b-', 'LineWidth', 2);
% xlabel('t'); ylabel('Force');
% 
% subplot(4,1,4);
% plot(t, U_d2, 'b-', 'LineWidth', 2);
% xlabel('t'); ylabel('U');
%---------------------------------------------------------------------

function f = ode_sys_controlled(t, x, K)

% Parameters 

M = 15;     % Mass of the Robot [kg]
theta = 30; % Angle of Inclination of the Plane [degree]
ks = 3;     % Spring Constant [N / M]
g = 10;     % Gravitational Acceleration [m / s2]


% State space matrices
A_mat = [0 1; -ks/M 0];

B_mat = [0; 1/M];

% ODE Right Hand Sides
% x_dot = A*x + B*u
% u = K*x

f = A_mat*x + B_mat*K*x;
end

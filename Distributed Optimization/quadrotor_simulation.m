%% 3D Non-linear Quadrotor Model Simulation via Runde-Kutta Numerical Integration
% Authors: 
% - Mehmet Batu Özmeteler
% - Shehabeldine Housein
%% Setup
clear all
clc
%% Parameters
t0 = 0;          % initial time
tf = 10;         % end time

h = 0.1;         % step size

N = (tf-t0)/h;   % horizon length
t = t0:h:tf;     % time span

%% Initial Conditions
x0 = [10; 0; 10; 0; 0; 0; 0; 0]; % x = 10, y = 10, z = 0

%% Constant Input Definition
f = 15;                             % f = 5 N
m = 1;                              % m = 1 kg

w_theta = 0;            % a total of pi/8 rotation on pitch
w_phi = (pi/8)*(tf/N);  % no rotation on roll

u_const = [f/m; w_theta; w_phi];    % a = f/m  ( m/s^2 )

%% Simulation
X = [ x0, zeros(length(x0),N-1) ]; 

for i = 2:N
    X(:,i) = rk4(@(t,x,u)quadrotor(t,x,u), h, t(i-1), X(:,i-1), u_const);
end

subplot(4,2,1)
plot(t(1:end-1), X(1,:))
xlabel('Time (s)')
ylabel('x-position')

subplot(4,2,2)
plot(t(1:end-1), X(2,:))
xlabel('Time (s)')
ylabel('x-dot')

subplot(4,2,3)
plot(t(1:end-1), X(3,:))
xlabel('Time (s)')
ylabel('y-position')

subplot(4,2,4)
plot(t(1:end-1), X(4,:))
xlabel('Time (s)')
ylabel('y-dot')

subplot(4,2,5)
plot(t(1:end-1), X(5,:))
xlabel('Time (s)')
ylabel('z-position')

subplot(4,2,6)
plot(t(1:end-1), X(6,:))
xlabel('Time (s)')
ylabel('z-dot')

subplot(4,2,7)
plot(t(1:end-1), X(7,:))
xlabel('Time (s)')
ylabel('theta')

subplot(4,2,8)
plot(t(1:end-1), X(8,:))
xlabel('Time (s)')
ylabel('phi')

%% Runge-Kutta Integrator
function xf = rk4(ode, h, t, x, u)
    k1 = ode(t, x, u);
    k2 = ode(t, x+h/2*k1, u);
    k3 = ode(t, x+h/2*k2, u);
    k4 = ode(t, x+h*k3, u);
    xf = x + h/6 * (k1 + 2*k2 + 2*k3 + k4);
end




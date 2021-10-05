% NLP Problem Formulation, MPC Design and Implementation 
clear all
close all
clc

% CasADi v3.5.1
addpath('C:\Program Files\casadi-windows-matlabR2016a-v3.5.1')
import casadi.*

%************************CONFIGURABLE PARAMETERS**************************%
% Constants for MPC Design
T = 0.2; % sampling time(s)
N = 20;  % prediction horizon

tol_err = 0.5; %tolerable error
x0 = 0;    % initial speed
xs = 50;   % reference speed

% Weighing Matrices for States and Controls
Q = 50; % multiplied by (current state - reference state)^2
R = 0.002; % multiplied by (current control - reference control)^2
S = 0.5; % multiplied by (next control - current control)^2

% Constraints' Boundaries, Vector and Objective Function
trq_max  = 500; trq_min = -trq_max;
v_max    = 80;  v_min   = 0;
obj = 0; g = []; 
%*************************************************************************%

% Conversion Metrics
% 1 rad/s = 1/2*pi Hz = 60/2*pi RPM
% 1 m/s   = 3.6 km/h
% 1 hP    = 745.7 W
 
% Constants for Vehicle Dynamics
Rtire  =  0.354;  % radius of tire(m)
G      =  9.81;   % earth's gravitational constant(m/s2)
mu     =  0.09;   % road friction(N) 
rho    =  1.225;  % density of air as a fluid(kg/m3)
Cd     =  0.55;   % air drag coefficient
A      =  5.1;    % vehicle frontal area(m2)
mss    =  2100;   % vehicle mass(kg)
ng     =  0.85;   % tire-specific constant
Im     =  0.18;   % motor inertia(kg*m2)
Gb     =  18.5;   % total gear ratio

% State Variables and Vectors
states = SX.sym('v');      % velocity(m/s)
n_states = length(states);

% Control Variables, Vectors
controls = SX.sym('trq');  % torque(N*m) 
n_controls = length(controls);
 
% Variables for Vehicle Dynamics                                           
Fbrake    =  0;                             % braking force(N)
Grade     =  0;                             % grade(%)
alpha     =  atan(Grade/100);               % degree of slope(rad)
rpm       =  (states/Rtire)*(60/(2*pi))*Gb; % revolutions per minute(rpm)
Faero     =  0.5*rho*A*Cd*(states^2);       % drag force against vehicle(N)
Frub      =  mu*mss*G;                      % rubbing force against tires(N)
Ftire     =  controls*Gb/Rtire;             % driving force(N)
rotSpeed  =  (2*pi/60)*rpm;                 % rotational speed (rad/s)
mechPower =  rotSpeed*controls;             % mechanical power (watt)

dvdt      =  (Ftire - Frub - Faero - mss*G*sin(alpha) - Fbrake)/(mss + Im*Gb^2/(ng*Rtire^2)); %acceleration(m/s2)

% Non-Linear Mapping Function f(x,u) and System R.H.S

f = Function('f',{states,controls},{dvdt});

U = SX.sym('U',n_controls,N);        % decision variables (controls)
P = SX.sym('P',n_states + n_states); % parameters (which include the initial and the reference state)
X = SX.sym('X',n_states,(N+1));      % a vector that represents the states over the optimization problem

%Symbolical Computation of the Objective Function and Optimal Speed Trajectory
X(1) = P(1); % initial state
for k = 1:N
    current_state = X(k);  current_control = U(k);
    f_value  = f(current_state,current_control);
   
    next_state  = current_state + (T*f_value);
    X(k+1) = next_state;
end

ff = Function('ff',{U,P},{X}); %a function to get the optimal speed trajectory
%when the optimal solution is calculated

for k = 1:N
     current_state = X(k);  current_control = U(k);
    if k == N
        next_control = U(k);
    else
        next_control = U(k+1);
    end
    obj = obj + (Q*((current_state-P(2))^2)) + (S*((next_control-current_control)^2))+ ...
        (R*(current_control^2));
end

%Computation of Constraints 
g = X;

% Non-Linear Programming Problem Initialization
% make the decision variables one column vector
opt_variables = reshape(U,N,1);
nlp_prob = struct('f', obj, 'x', opt_variables, 'g', g, 'p', P);

opts = struct;
opts.ipopt.max_iter = 100;
opts.ipopt.print_level =0;%0,3
opts.print_time = 0;
opts.ipopt.acceptable_tol =1e-8;
opts.ipopt.acceptable_obj_change_tol = 1e-6;

solver = nlpsol('solver', 'ipopt', nlp_prob, opts);
args = struct;

%State Constraints
args.lbg = v_min;
args.ubg = v_max;   

%Controls Constraints
args.lbx = trq_min; 
args.ubx = trq_max; 


%              THE SIMULATION              %
%-------------------------------------------
t0 = 0;

xx(1) = x0; %the history of states
t(1)  = t0;

u0 = zeros(N,1); % control inputs for N steps

simu_time = 20;  % maximum simulation time(s)

% MPC
mpc_iter = 0;
xx_his = [];
u_cl=[];

% the main simulation loop works as long as the error is greater
% than the tolerable amount and the number of mpc steps is less 
% than its maximum value

main_loop = tic;
while((xs-full(x0) > tol_err) && mpc_iter < simu_time / T)
    args.p  = [x0;xs]; % set the values of the parameters vector
    args.x0 = reshape(u0',N,1);
    %tic
    sol = solver('x0', args.x0, 'lbx', args.lbx, 'ubx', args.ubx,...
            'lbg', args.lbg, 'ubg', args.ubg,'p',args.p);   
    %toc
    u = reshape(full(sol.x)',1,N)';
    ff_value = ff(u',args.p);
    xx_his(:,mpc_iter+1)= full(ff_value); % history of the state predictions
    
    u_cl= [u_cl ; u(1)];
    t(mpc_iter+1) = t0;
    
    [t0, x0, u0] = move(T, t0, x0, u, f);
        
    xx(mpc_iter+2) = full(x0);
       
    mpc_iter
    mpc_iter = mpc_iter + 1;
end;
main_loop_time = toc(main_loop);
average_mpc_time = main_loop_time/(mpc_iter+1)

%Plot Velocity and Torque
plot_v_trq(xx, u_cl, t, trq_max, xs, T, N);

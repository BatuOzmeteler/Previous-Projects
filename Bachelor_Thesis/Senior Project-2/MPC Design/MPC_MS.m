% NLP Problem Formulation, MPC Design and Implementation 

% In single shooting technique, X vector which represents the optimal speed
% trajectory is recursively filled with non-linear mapping function 
% outputs considering f(w, x0, t0) = x0 as the only equality constraint.
% However, with longer prediction horizons, the non-linearity in X propagates.
% Therefore, multiple shooting should be used to improve convergence,
% decrease computation time. In addition, MS allows the user to initialize
% the state trajectory with a known guess.  

% Multiple shooting is an approach where the key idea is to break down
% the system integrals into short-time intervals, i. e. use the system
% model as a state constraint at each optimization step. It is a lifted 
% single shooting where the NLP solved is larger but sparser.

clear all
close all
clc

% CasADi v3.5.1
addpath('C:\Program Files\casadi-windows-matlabR2016a-v3.5.1')
import casadi.*

%************************CONFIGURABLE PARAMETERS**************************%
% Constants for MPC Design
T = 0.2; % sampling time(s)
N = 10;  % prediction horizon

tol_err = 0.1; %tolerable error
x0 = 0;    % initial speed
xs = 60;   % reference speed

% Weighing Matrices for States and Controls
Q = 5; % multiplied by (current state - reference state)^2
R = 0.0001; % multiplied by (current control - reference control)^2
S = 0.01; % multiplied by (next control - current control)^2

% Constraints' Boundaries, Vector and Objective Function
trq_max  = 250; trq_min = -trq_max;
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
v = SX.sym('v');      % velocity(m/s)
states = v;       
n_states = length(states);

% Control Variables, Vectors
trq = SX.sym('trq');  % torque(N*m)
controls = trq; 
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
mechPower =  rotSpeed*controls/745.7;       % mechanical power (hP)

plot_rpm        =  Function('plot_rpm', {states},{rpm});
plot_mechPower  =  Function('plot_mechPower', {states,controls},{mechPower});

dvdt      =  (Ftire - Frub - Faero - mss*G*sin(alpha) - Fbrake)/(mss + Im*Gb^2/(ng*Rtire^2)); %acceleration(m/s2)

%try to find how to set variable constraints
%motor_inv_max_trq_output   = [250,250,249,243,215,162,132.5,112.5,97,84,79.6];
%motor_inv_max_rpm_input    = [0,4000,5000,5500,6000,7000,8000,9000,10000,11000,12000];
%maxAvailableTrq = interp1(motor_inv_max_rpm_input,motor_inv_max_trq_output,rpm);
%trq_max =  maxAvailableTrq; trq_min = -maxAvailableTrq;

% Non-Linear Mapping Function f(x,u) and System R.H.S
rhs = (Ftire - Frub - mss*G*sin(alpha) - Fbrake)/(mss + Im*Gb^2/(ng*Rtire^2));
f = Function('f',{states,controls},{rhs});

U = SX.sym('U',n_controls,N);        % decision variables (controls)
P = SX.sym('P',n_states + n_states); % parameters (which include the initial and the reference state)
X = SX.sym('X',n_states,(N+1));      % a vector that represents the states over the optimization problem

% Computation of the Objective Function and Constraints Vector

init_state = X(1);       % initial state
g = [g;(init_state-P(1))]; % initial condition constraints
for k=1:N
    current_state = X(k); current_control = U(k);
    if k == N
        next_control = U(k);
    else
        next_control = U(k+1);
    end
    obj = obj + ((current_state-P(2))*Q*(current_state-P(2))) + ...
        current_control*R*current_control + ...
        ((next_control-current_control)*S*(next_control-current_control));
   
    f_value  = f(current_state,current_control);
    next_state = X(k+1);  
    next_state_pred = current_state + (T*f_value);
    g = [g;next_state - next_state_pred];
end

% Non-Linear Programming Problem Initialization
opt_variables = [reshape(X,N+1,1);reshape(U,N,1)];
nlp_prob = struct('f', obj, 'x', opt_variables, 'g', g, 'p', P);

opts = struct;
opts.ipopt.max_iter = 2000;
opts.ipopt.print_level =0;%0,3
opts.print_time = 0;
opts.ipopt.acceptable_tol =1e-8;
opts.ipopt.acceptable_obj_change_tol = 1e-6;

solver = nlpsol('solver', 'ipopt', nlp_prob, opts);
args = struct;

%Equality Constraints
args.lbg(1:N+1) = 0;    
args.ubg(1:N+1) = 0;     

%States Upper and Lower Bounds
args.lbx(1:(N+1)) = v_min; %velocity lower bound
args.ubx(1:(N+1)) = v_max; %velocity upper bound

%Controls Upper and Lower Bounds
args.lbx((N+1)+1:(2*N)+1) = trq_min; %torque lower bound
args.ubx((N+1)+1:(2*N)+1) = trq_max; %torque upper bound


%              THE SIMULATION              %
%-------------------------------------------
t0 = 0;

xx(1) = x0; %the history of states
t(1)  = t0;

u0 = zeros(N,1); % control inputs for N steps
X0 = repmat(x0,1,N+1)'; % initialization of the states decision variables

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
    if(mpc_iter == 1) %reiterate to calculate g 
        g = [];
        g = [g;(init_state-P(1))];
        for k=1:N  
            current_state = X(k); current_control = U(k);         
            f_value  = f(current_state,current_control);
            next_state = X(k+1);  
            next_state_pred = current_state + (T*f_value);
            g = [g;next_state - next_state_pred];
        end
        %re-initialize NLP Problem and Solver
        nlp_prob = struct('f', obj, 'x', opt_variables, 'g', g, 'p', P); 
        solver = nlpsol('solver', 'ipopt', nlp_prob, opts);
    end    
    
    args.p  = [x0;xs]; % set the values of the parameters vector
    args.x0  = [reshape(X0',N+1,1);reshape(u0',N,1)];
    %tic
    
    
    sol = solver('x0', args.x0, 'lbx', args.lbx, 'ubx', args.ubx,...
    'lbg', args.lbg, 'ubg', args.ubg,'p',args.p);   
    %toc
    u = reshape(full(sol.x((N+1)+1:end))',1,N)';
    
    xx_his(:,mpc_iter+1) = full(sol.x(1:(N+1))); % history of the state predictions
    
    u_cl= [u_cl ; u(1)];
    t(mpc_iter+1) = t0;
    
    [t0, x0, u0] = move(T, t0, x0, u, f);
    
    xx(mpc_iter+2) = full(x0);
    
    X0 = full(sol.x(1:(N+1))); % get solution trajectory
    % Shift trajectory to initialize the next step
    X0 = [X0(2:end);X0(end)];
    
    mpc_iter;
    mpc_iter = mpc_iter + 1;
end;
main_loop_time = toc(main_loop);
average_mpc_time = main_loop_time/(mpc_iter+1)

mpc_iter
xx(end)

%Plot Velocity and Torque
plot_v_trq(xx, u_cl, t, trq_max, xs, T, N);

%Plot RPM, Mechanical Power and Rotational Speed
%plot_rpm_power(xx, u_cl, t, T, plot_rpm, plot_mechPower);

%Print Details
print_details(N, T, xx, t)



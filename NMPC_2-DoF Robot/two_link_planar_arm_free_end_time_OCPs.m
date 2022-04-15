%% Two Link Planar Arm - Free-End-Time OCP Formulations
% Author: Mehmet Batu Özmeteler
%% Setup
clear all
clc
import casadi.*                           
%% Parameters
disp('Parameter Setup')

N = 100;            % number of control intervals 
t = 0:1/N:1;        % dimensionless time array after time transformation

nx = 4;             % dimension of the state vector
nu = 2;             % dimension of the input vector

wR = 1;             % weight of R matrix    
wT = 1e6;           % weight of T matrix

control_task = 1;   % Control Task => Minimal Time
                    %              => Minimal Time + Minimal Control Energy 

%% Initial Conditions
q1_0 = -5;   q2_0 = -4;
q1_d_0 = 0;  q2_d_0 = 0; 

x_init = [q1_0; q2_0; q1_d_0; q2_d_0];

%% State Goal Vector
q1_g = pi/2; q2_g = 0;
q1_d_g = 0;  q2_d_g = 0;

x_goal = [q1_g; q2_g; q1_d_g; q2_d_g];

%% State Bounds
q1_min = -Inf; q1_max = Inf;
q2_min = -Inf; q2_max = Inf;

q1_d_min = -3*pi/2;  q1_d_max = 3*pi/2;
q2_d_min = -3*pi/2;  q2_d_max = 3*pi/2;

x_min = [q1_min; q2_min; q1_d_min; q2_d_min];
x_max = [q1_max; q2_max; q1_d_max; q2_d_max];

%% Input Bounds
tao_1_min = -1000;  tao_1_max = 1000;           
tao_2_min = -1000;  tao_2_max = 1000;

u_min = [tao_1_min; tao_2_min]; 
u_max = [tao_1_max; tao_2_max]; 

%% Initial Guesses
x_init_guess = zeros(nx, 1);
u_init_guess = zeros(nu, 1);
t_init_guess = 2;

%% OCP Formulation
disp('OCP Formulation')

ocp = casadi.Opti();         % OCP object

X = ocp.variable(nx, N+1);   % symbolic state matrix
U = ocp.variable(nu, N);     % symbolic input matrix

T = ocp.variable();          % final (optimal) time

h = T/N;                     % integration step size

J = wT*T;                    % objective function

R = wR*eye(nu);              % weight matrix for inputs

for i = 1:N
    % enforce constraints at every point for dynamics
    xf = heun(@(x,u) T*two_link_planar_arm_model(x, u), h, X(:,i), U(:,i));
    ocp.subject_to( X(:,i+1) == xf );
    
    % update objective function 
    if control_task
        u = U(:,i);
        x = X(:,i+1);
        J = J + (u'*R*u); 
    end
end

lbX = repmat(x_min, N+1, 1); ubX = repmat(x_max, N+1, 1);
lbU = repmat(u_min, N, 1); ubU = repmat(u_max, N, 1);

% state constraints
ocp.subject_to( lbX <= vertcat(X(:)) <= ubX );

% input constraints
ocp.subject_to( lbU <= vertcat(U(:)) <= ubU );

% initial conditions
ocp.subject_to( X(:,1) == x_init );

% terminal constraint
ocp.subject_to( X(:,N+1) == x_goal );

% constraint on the time variable
ocp.subject_to(T >= 0);

%% Solve OCP 
disp('Problem Solution')

% initial guess
ocp.set_initial(X, repmat(x_init_guess, 1, N+1));
ocp.set_initial(U, repmat(u_init_guess, 1, N));  
ocp.set_initial(T, t_init_guess);

% set objective
ocp.minimize(J);

% set solver
%p_opts = struct('expand', true);
%s_opts = struct('max_iter', 100);
%ocp.solver('ipopt', p_opts, s_opts);
ocp.solver('ipopt');

% solve ocp
sol = ocp.solve();

% get solution
Xsol = sol.value(X);
Usol = sol.value(U);
Tsol = sol.value(T);

%% Plot Results
disp(['Minimal Time: ' num2str(Tsol) ' seconds']);

Xsol_pred = Xsol;

plotStates(t, Xsol, x_init, x_goal, nx)
plotInputs(t, Usol, nu)
plot_q1q2(Xsol, Xsol_pred, x_init, x_goal)
plot_xy(Xsol, Xsol_pred, x_init, x_goal)

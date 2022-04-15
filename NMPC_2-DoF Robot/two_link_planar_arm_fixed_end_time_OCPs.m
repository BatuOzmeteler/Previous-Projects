%% Two Link Planar Arm - Fixed-Time OCP Formulations
% Author: Mehmet Batu Özmeteler
%% Setup
clear all
clc
import casadi.*
%% Simulation Options
disp('Parameter Setup')

stabilization_method = 0;   % Stabilization Method => 0 to stabilize w/ terminal penalty
                            %                      => 1 to stabilize w/ terminal constraint
                        
integrator_type = 0;        % Integrator Type      => 0 to use RK4
                            %                      => 1 to use Heun
                            
%% Parameters
t0 = 0;                 % initial time for OCP
tf = 3;                 % end time for OCP

h = 0.05;               % integration step size

N = (tf - t0)/h;        % horizon length
t = t0:h:tf;            % time span

nx = 4;                 % dimension of the state vector
nu = 2;                 % dimension of the input vector

wQ = 10;                % weight of Q matrix
wR = 0.1;               % weight of R matrix
wP = 1e8;               % weight of P matrix

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

%% OCP Formulation
disp('OCP Formulation')

ocp = casadi.Opti();         % OCP object

X = ocp.variable(nx, N+1);   % symbolic state matrix
U = ocp.variable(nu, N);     % symbolic input matrix
J = 0;                       % cumulative cost 

Q = wQ*eye(nx);               % weight matrix for the states
R = wR*eye(nu);               % weight matrix for the inputs
P = wP*eye(nx);               % weight matrix for the terminal state

for i = 1:N
    % enforce constraints at every point for dynamics   
    if integrator_type
        xf = heun(@(x, u) two_link_planar_arm_model(x, u), h, X(:,i), U(:,i));
    else
        xf = rk4(@(x, u) two_link_planar_arm_model(x, u), h, X(:,i), U(:,i));
    end
    
    ocp.subject_to( X(:,i+1) == xf );
     
    % piecewise constant state for the computation of cost function 
    x = X(:,i+1);
    u = U(:,i);
    J = J + (x - x_goal)'*Q*(x - x_goal) + (u'*R*u);
end

lbX = repmat(x_min, N+1, 1); ubX = repmat(x_max, N+1, 1);
lbU = repmat(u_min, N, 1); ubU = repmat(u_max, N, 1);

% state constraints
ocp.subject_to( lbX <= vertcat(X(:)) <= ubX );

% input constraints
ocp.subject_to( lbU <= vertcat(U(:)) <= ubU );

% initial conditions
ocp.subject_to( X(:,1) == x_init );

if stabilization_method
    % Stabilizing w/ Terminal Constraint
    ocp.subject_to( X(:,N+1) == x_goal );
else
    % Stabilizing w/ Terminal Penalty
    J = J + (X(:, N+1) - x_goal)'*P*(X(:, N+1) - x_goal);
end

%% Solve OCP 
disp('Problem Solution')

% initial guess
ocp.set_initial(X, repmat(x_init_guess, 1, N+1));
ocp.set_initial(U, repmat(u_init_guess, 1, N));  

% set solver
%p_opts = struct('expand', true);
%s_opts = struct('max_iter', 100);
%ocp.solver('ipopt', p_opts, s_opts);
ocp.solver('ipopt');

% set objective
ocp.minimize(J);

% solve ocp
sol = ocp.solve();

% get solution
Xsol = sol.value(X);
Usol = sol.value(U);

%% Plot Results
Xsol_pred = Xsol;

plotStates(t, Xsol, x_init, x_goal, nx)
plotInputs(t, Usol, nu)
plot_q1q2(Xsol, Xsol_pred, x_init, x_goal)
plot_xy(Xsol, Xsol_pred, x_init, x_goal)

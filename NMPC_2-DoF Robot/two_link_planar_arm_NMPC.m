%% Two Link Planar Arm - NMPC
% Author: Mehmet Batu Özmeteler
%% Setup
clear all
clc
import casadi.*
%% Simulation Options
disp('Parameter Setup')

stabilization_method = 0;   % Stabilization Method   => 0 to stabilize w/ terminal penalty
                            %                        => 1 to stabilize w/ terminal constraint
                        
integrator_type = 0;        % Integrator Type        => 0 to use RK4
                            %                        => 1 to use Heun

parametric_uncertainty = 0; % Parametric Uncertainty => 0 to use the correct model
                            %                        => 1 to use the incorrect model
                            
measurement_noise = 0;      % Measurement Noise      => 0 to exclude measurement noise
                            %                        => 1 to include measurement noise
                            
%% Parameters
t0 = 0;                     % initial time for OCP
tf = 3;                     % end time for OCP 

h = 0.05;                   % integration step size
N = (tf-t0)/h;              % horizon length

nx = 4;                     % dimension of the state vector
nu = 2;                     % dimension of the input vector

wQ = 10;                    % weight of Q matrix
wR = 0.1;                   % weight of R matrix
wP = 1e8;                   % weight of P matrix

dt = 0.1;                   % sampling time

NMPC_max_iter = 150;        % number of maximum NMPC iterations
NMPC_conv_tol = 0.001;      % convergence tolerance of NMPC

sigma = 1e-3;               % variance of the measurement noise

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

%% OCP Formulation
disp('OCP Formulation')

ocp = casadi.Opti();         % OCP object

X = ocp.variable(nx, N+1);   % symbolic state matrix
U = ocp.variable(nu, N);     % symbolic input matrix

X0 = ocp.parameter(nx, 1);   % symbolic vector for the initial condition

J = 0;                       % cumulative cost 

Q = wQ*eye(nx);              % weight matrix for the states
R = wR*eye(nu);              % weight matrix for the inputs
P = wP*eye(nx);              % weight matrix for the terminal state

for i = 1:N
    % enforce constraints at every point for dynamics
    if parametric_uncertainty
        if integrator_type
            xf = heun(@(x, u) two_link_planar_arm_incorrect_model(x, u), h, X(:,i), U(:,i));
        else
            xf = rk4(@(x, u) two_link_planar_arm_incorrect_model(x, u), h, X(:,i), U(:,i));
        end
    else
        if integrator_type
            xf = heun(@(x, u) two_link_planar_arm_model(x, u), h, X(:,i), U(:,i));
        else
            xf = rk4(@(x, u) two_link_planar_arm_model(x, u), h, X(:,i), U(:,i));
        end
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
ocp.subject_to( X(:,1) == X0 );

if stabilization_method
    % Stabilizing w/ Terminal Constraint
    ocp.subject_to( X(:,N+1) == x_goal );
else
    % Stabilizing w/ Terminal Penalty
    J = J + (X(:, N+1) - x_goal)'*P*(X(:, N+1) - x_goal);
end

% set solver
%p_opts = struct('expand', true);
%s_opts = struct('max_iter', 100);
%ocp.solver('ipopt', p_opts, s_opts);
ocp.solver('ipopt');

% set objective
ocp.minimize(J);

%% Open-loop Solution 
disp('Open-loop Solution')

ocp.set_initial(X, repmat(x_init, 1, N+1));
ocp.set_initial(U, repmat(zeros(nu, 1), 1, N)); 
    
ocp.set_value(X0, x_init)

open_loop_sol = ocp.solve();

XX_pred = open_loop_sol.value(X);
UU_pred = open_loop_sol.value(U);

disturbances = zeros(nx, N); % disturbances

if measurement_noise
    % create disturbances for the horizon
    for i = 1:N
        disturbance = wgn(nx, 1, sigma, 'linear');
        disturbances(:,i) = disturbance;
    end     
end

for i = 1:N
    % integrate the system with the predicted inputs
    if integrator_type
        x_next = heun(@(x, u) two_link_planar_arm_model(x, u), h, XX_pred(:,i), UU_pred(:,i));
    else
        x_next = rk4(@(x, u) two_link_planar_arm_model(x, u), h, XX_pred(:,i), UU_pred(:,i));
    end
    
    x_next = x_next + disturbances(:, i);
    
    XX_pred(:,i+1) = x_next;
end

%% MPC Loop 
disp('Closed-loop MPC Simulation')

t = [t0];                             % simulation time

XX = zeros(nx, 1);                    % state trajectory
UU = zeros(nu, 1);                    % input trajectory

Xsol = repmat(x_init, 1, N+1);        % solution for states
Usol = repmat(zeros(nu, 1), 1, N);    % solution for inputs

% set initial state
XX(:, 1) = x_init; 

for i = 1:NMPC_max_iter    
    % shift the solution for the initial guess
    x_init_guess = [Xsol(:,2:end) Xsol(:,end)];
    u_init_guess = [Usol(:,2:end) Usol(:,end)];
    
    % assign initial guess (for warm-start)
    ocp.set_initial(X, x_init_guess);
    ocp.set_initial(U, u_init_guess);  
    
    % assign initial condition
    ocp.set_value(X0, XX(:, i))
    
    % solve OCP
    sol = ocp.solve();
    
    % extract solutions
    Xsol = sol.value(X); 
    Usol = sol.value(U);
    
    % apply the first control input
    if integrator_type
        x_next = heun(@(x, u) two_link_planar_arm_model(x, u), dt, XX(:,i), Usol(:, 1));
    else
        x_next = rk4(@(x, u) two_link_planar_arm_model(x, u), dt, XX(:,i), Usol(:, 1));
    end
    
    % add measurement noise
    if measurement_noise
        disturbance = wgn(nx, 1, sigma, 'linear');
        x_next = x_next + disturbance;
    end
    
    % append to trajectories
    XX(:, i+1) = x_next;
    UU(:, i) = Usol(:, 1);
    t(i+1) = t(i) + dt; 
    
    % check convergence
    error = abs(XX(:, i) - x_goal);
    if( error <= NMPC_conv_tol)
        fprintf('\nTotal NMPC Iterations : %i\n', i);
        break;
    end
    
    if i == NMPC_max_iter
        fprintf(['\nMaximum NMPC Iterations (' num2str(NMPC_max_iter) ') Reached\n']);
    end

end

%% Plot Results
fprintf('\n Final Error => q_1: %.2f, q_2: %.2f, q_1_dot: %.2f, q_2_dot: %.2f\n',...
    error(1), error(2), error(3), error(4) );

plotStates(t, XX, x_init, x_goal, nx)
plotInputs(t, UU, nu)
plot_q1q2(XX, XX_pred, x_init, x_goal)
plot_xy(XX, XX_pred, x_init, x_goal)

%% Main
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function runs open-loop or closed-loop DMPC simulations. 
%%
function main()
    clear all
    clc

    disp('Setting up the Parameters')
    param.N_agents = 2;           % number of agents
    param.N = 20;                 % prediction horizon
    param.dt = 0.2;               % step size

    param.nx = 8;                 % dimension of the states
    param.nu = 3;                 % dimension of the inputs

    param.m = 1;                  % mass of the quadrotors
    param.d = 1;                  % distance of formation

    param.wQ = 100;               % weight of Q matrix
    param.wR = 1;                 % weight of R matrix

    param.wP = 0;                 % weight of P matrix
    param.term_constraint = 1;    % with/without terminal constraint

    % Setting up the initial conditions and the goal points
    x_init = 0; xd_init = 0; y_init = 0; yd_init = 0;
    z_init = 0; zd_init = 0; theta_init = 0; phi_init = 0;

    x_init = [x_init; xd_init; y_init; yd_init; z_init; zd_init; theta_init; phi_init];

    x_g = 5; xd_g = 0; y_g = 5; yd_g = 0;
    z_g = 5; zd_g = 0; theta_g = 0; phi_g = 0;

    x_goal = [x_g; xd_g; y_g; yd_g; z_g; zd_g; theta_g; phi_g];

    x_inits = cell(1, param.N_agents);
    x_goals = cell(1, param.N_agents); 

    for i = 1:param.N_agents
        x_inits{i} = create_formation(x_init, param.d, param.N_agents, i);
        x_goals{i} = create_formation(x_goal, param.d, param.N_agents, i);
    end

    param.x_inits = x_inits;
    param.x_goals = x_goals;    
    
    opts.alg = 'ADMM';
    opts.locSol = 'ipopt';
    opts.solveQP = 'MA57';
    opts.rho = 100;
    opts.rho0 = 100;
    opts.maxiter = 5;
    
    t0 = 0;
    tf = 4; 
    
    open_loop_DMPC(param, opts)
    %closed_loop_DMPC(param, opts, t0, tf)
    %closed_loop_DMPC_with_ETC(param, opts, t0, tf)
    
end

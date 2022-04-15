%% Initialize Elements
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function initializes the elements for the problem creation.
%%
function [data, elem] = init_elem(param, data)

    import casadi.*

    % Deal parameters
    [N_agents, N, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~] ...
    = deal_param(param);

    % Deal data
    [~, ~, ~, n_z, n_g, ~, n_cc] ...
        = deal_data(data);
    
    % Fix n_h size
    [A_obs, b_obs] = build_A_obs_b_obs(param, data);
    data.n_h = size(b_obs{1}, 1)*(N+1);
    n_h = data.n_h;
    
    % Initialize elements
    z = cell(1,N_agents);           % decision vectors 

    f = cell(1,N_agents);           % local objective functions 
    h = cell(1,N_agents);           % local inequality constraints 
    g = cell(1,N_agents);           % local equality constraints

    llbx = cell(1,N_agents);        % lower bounds for z 
    uubx = cell(1,N_agents);        % upper bounds for z 

    zz0 = cell(1,N_agents);         % initial guesses for z
    lam0 = cell(1,N_agents);        % initial guesses for Lagrange multipliers

    AA = cell(1,N_agents);          % coupling constraints

    H_bar = cell(1,N_agents);       % condensed matrices for obj. functions
    
    for i = 1:N_agents
        % Initialize symbolic decision vector z
        varstring = sprintf('z_%i_', i);
        
%        z{i} = sym(varstring, [n_z 1], 'real'); 
        z{i} = SX.sym(varstring, [n_z 1]); 

        % Initialize g, h
%        g{i} = sym('g', [n_g 1]);
%        h{i} = sym('h', [n_h 1]);
        
        g{i} = SX.sym('g', [n_g 1]);
        h{i} = SX.sym('h', [n_h 1]);
        
        % Initialize llbx, uubx
        llbx{i} = zeros(n_z, 1);
        uubx{i} = zeros(n_z, 1);

        % Initialize AA
        AA{i} = zeros(n_cc, n_z);
    end

    elem.z = z;
    
    elem.H_bar = H_bar;
    
    elem.f = f;
    elem.g = g;
    elem.h = h;
    
    elem.llbx = llbx;
    elem.uubx = uubx;
    
    elem.AA = AA;
    
    elem.zz0 = zz0;
    elem.lam0 = lam0;
    
end

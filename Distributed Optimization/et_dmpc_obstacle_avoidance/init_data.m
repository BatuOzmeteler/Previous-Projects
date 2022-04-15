%% Initialize Data
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function initializes the data for the problem creation.
%%
function data = init_data(param)

    % Deal parameters
    [N_agents, N, ~, nx, nu, ~, ~, ~, ~, ~, ~, ~, term_constraint, ~, ~, ~] ...
    = deal_param(param);

    % Build state and input bounds
    bounds = build_bounds();
    
    % Build neighbours
    [neighbours, n_neighbours] = build_neighbours(param);
    
    % Set the number of decision variables
    n_z = (N+1)*nx + N*nu + N*n_neighbours*nx;    
    
    % Set the number of equality constraints (dynamics + formation)
    if term_constraint
        n_g = (N+1)*nx + (2*N)*n_neighbours + nx; % with terminal constraint
    else
        n_g = (N+1)*nx + (2*N)*n_neighbours;      % without terminal constraint
    end
    
    % Set the number of inequality constraints (obstacle avoidance)
    n_h = 0;                                      
    
    % Set the number of coupling constraints 
    n_cc = N_agents*n_neighbours*(N*nx);           
    
    % Set the problem data
    data.bounds = bounds;
    
    data.neighbours = neighbours;
    data.n_neighbours = n_neighbours;
    
    data.n_z = n_z;
    data.n_g = n_g;
    data.n_h = n_h;
    data.n_cc = n_cc;
    
end
%% Linear ADMM Implementation for a Coupled Spring-Mass-Damper System
% Authors: 
% - Mehmet Batu Özmeteler
%% Each agent solves a local QP in the form below:
%
% min sum 1/2 * (z_i' * H_i * z_i) + g_i' * z_i
%
% s.t.
%      A_eq_i*z_i = b_eq_i     => equality constraints (system dynamics, initial conditions) for each agent
%      A_ineq_i*z_i = b_ineq_i => inequality constraints (input constraints) for each agent
%      sum{i=1 to N_agents}(c_c_i*z_i) = 0 => coupling constraint for agents
%
% z_i = quadprog(H_i, g_i, A_ineq_i, b_ineq_i, A_eq_i, b_eq_i, lb, ub, x0, options)
% 
%% ---------------VARIABLE DESCRIPTIONS--------------- %%
%
% z = [zi ... zN_agents]
% z contains the all decision vectors
% z_i contains the ith agent's decision vector

% z_i' = [x_i_0' ... x_i_N', u_i_0' ... u_i_N-1', v']
% x = [x_i_0' ... x_i_N'] contains the state trajectory for each agent
% u = [u_i_0' ... u_i_N-1'] contains the input trajectory for each agent
% v contains state trajectory copies of the neighbors for each agent

% A_d contains state (system) matrices
% B_d contains input (system) matrices
% C_d contains output (system) matrices
% N_agents contains number of agents

% A_eq_i * z_i = b_eq_i
% A_eq contains the equality constraint matrices
% b_eq contains the equality constraint vector

% A_ineq_i * z_i = b_ineq_i
% A_ineq contains the inequality constraint matrices
% B_ineq contains the inequality constraint vector

% c_c_i*z_i = 0

% x_k contains the optimal decision vectors for each agent after solving local QPs 
% where each agent obtains what it thinks its optimal decision
% variable z_i is without considering coupling constraints

% lambda_k contains the optimal multipliers vectors for each agent

% v_k contains the optimal state trajectories of the current agent calculated by the 
% current agent and neighbours of the agent after local QPs are solved

% traj_state_avg_k contains the state trajectory average for each agent where the average 
% state trajectory is calculated from the sum of local optimal state trajectory and 
% copy state trajectories at the neighbours

% z_k contains the optimal decision vectors where all constraints are considered

%%
function [x, lambda, iter] = admm_practice(N_agents, N, x0)
    
    % Parameters
    rho = 100;                       % penalization term
    
    n_x = 2*ones(1,N_agents);        % number of states for agents 
    n_u = ones(1,N_agents);          % number of inputs for agents
    
    x_k = cell(1, N_agents);         % optimal decision variables w/o c_c
    x = cell(1, N_agents);           % temp. decision variables w/o c_c
    
    v_k = cell(1, N_agents);      
    traj_state_avg_k = cell(1, N_agents);
    
    z_k = cell(1, N_agents);         % optimal decision variables w/ c_c (z_bar)
    z = cell(1, N_agents);           % temp. decision variables w/ c_c
    
    lambda_k = cell(1, N_agents);    % optimal multipliers
    lambda = cell(1, N_agents);      % temp. multipliers
       
    H = cell(1, N_agents);           % updated Hessians
    g = cell(1, N_agents);           % updated terms for the local QP
    
    traj_copies = cell(1, N_agents); % keeps the trajectory copies of the agents
    % that are present in other agents' decision vectors
    
    % Generate the system with N_agents and a prediction horizon of N 
    [A_d, B_d, C_d, sizes_z_i, neighbours, H_bar, c_c,...
    A_eq, b_eq, A_ineq, b_ineq] = N_mass_spring_damper(N_agents, N, x0);

    % Initialization
    for i = 1:N_agents
       x_k{i} = zeros(sizes_z_i{i}, 1);
       x{i} = zeros(sizes_z_i{i}, 1);
       
       z_k{i} = zeros(sizes_z_i{i}, 1);
       z{i} = zeros(sizes_z_i{i}, 1);
       
       traj_state_avg_k{i} = zeros(n_x(i)*N, 1);
       v_k{i} = zeros(n_x(i)*N, size(neighbours{i},2)*2);
       
       lambda_k{i} = zeros(size(c_c{i}, 1), 1);
       lambda{i} = zeros(size(c_c{i}, 1), 1);
       
       H{i} = zeros(sizes_z_i{i});
       g{i} = zeros(size(c_c{i}, 1), 1);
    end
    
    % ADMM Algorithm
    iter_max = 200;
    for iter = 1:iter_max   
        
    % 0) Reset v_k, z_k and traj_copies
        for i = 1:N_agents
          z_k{i} = zeros(sizes_z_i{i}, 1);
          v_k{i} = double.empty(n_x(i)*N, 0);
          traj_copies{i} = double.empty(n_x(i)*N, 0);
        end
        
    % 1) Solve local optimizations with quadprog in parallel to update x_k
        opt = optimoptions('quadprog','Display','off');
        for i = 1:N_agents
            H{i} = H_bar{i} + rho*c_c{i}.'*c_c{i};
            g{i} = c_c{i}.'*lambda{i} - rho*c_c{i}.'*c_c{i}*z{i};
            x_k{i} = quadprog(H{i},g{i},A_ineq{i},b_ineq{i},A_eq{i},b_eq{i},[],[],[],opt);
        end
        
    % 2) Collect trajectory copies from other neighbours 
        for i = 1:N_agents
            for j = 0:size(neighbours{i},2)-1
                idx = neighbours{i}(1, j+1);
                offset = (N+1)*n_x(i) + N*n_u(i);
                traj_copies{idx} = [traj_copies{idx},...
                    x_k{i}( (offset + (N*n_x(i)*j) + 1) : (offset + (N*n_x(i)*(j+1))) )];
            end
        end
        
    % 3) Update v_k after communication
        for i = 1:N_agents
            for j = 1:size(neighbours{i},2)
                v_k{i} = [v_k{i}, traj_copies{i}(:,j), x_k{i}(1:N*n_x(i))];
            end
        end
        
    % 4) Calculate average trajectories for every agent
        for i = 1:N_agents
            traj_state_avg_k{i} = mean(v_k{i},2);
        end
        
    % 5) Update z_k by stacking the trajectory average, state at N+1,
    % optimal input sequence and optimal neighbour trajectories
    
        % Stack the trajectory average, state at N+1 and 
        % optimal input sequence for every agent
        for i = 1:N_agents
           z_k{i}(1:N*n_x(i)) = traj_state_avg_k{i};
           z_k{i}(N*n_x(i)+1:(N+1)*n_x(i)+N*n_u(i)) = x_k{i}(N*n_x(i)+1:(N+1)*n_x(i)+N*n_u(i));
        end
        % Update the state trajectory copies to be optimal for every agent
        for i = 1:N_agents
            for j = 0:size(neighbours{i},2)-1
                neighbour = neighbours{i}(j+1);
                offset = (N+1)*n_x(i) + N*n_u(i);
                z_k{i}( (offset + (N*n_x(i)*j) + 1) : (offset + (N*n_x(i)*(j+1))) ) = traj_state_avg_k{neighbour};
            end
        end
        
     % 6) Check if convergence occurs
       isConverged = 1; 
       for i = 1:N_agents
           r = c_c{i}*x_k{i} - c_c{i}*z_k{i};
           d = -rho*c_c{i}*(z_k{i} - z{i});
           if norm(r,inf) > 1e-6*max([norm(c_c{i}*x_k{i},inf),norm(c_c{i}*x{i},inf)]) || norm(r,inf) > 1e-6
               isConverged = 0;
               break;
           end
           if norm(d,inf) > 1e-3*norm(lambda{i},inf) || norm(d,inf) > 1e-3
               isConverged = 0;
               break;
           end           
       end
       
     % 7) Exit the loop if convergence occurs
       if isConverged
           break;
       end
        
     % 8) Update lambda_k
       for i = 1:N_agents
           lambda_k{i} = lambda{i} + rho*c_c{i}*(x_k{i} - z_k{i});
       end
       z = z_k;
       lambda = lambda_k;
    end

    if iter == iter_max
        error('ADMM did not converge');
    end
    x = x_k; 
    lambda = lambda_k;

end

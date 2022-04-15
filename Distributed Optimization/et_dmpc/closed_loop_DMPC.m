%% Closed-loop DMPC Simulation
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function runs an closed-loop DMPC 
% simulation with a certain parameter set.
%%
function closed_loop_DMPC(param, opts, t0, tf)
    %% Initial Problem Creation
    disp('Creating the Initial Problem')
    tic 
    [sProb, elem] = create_sProb(param);
    toc

    %% Simulation Initialization
    [N_agents, N, dt, nx, nu, ~, ~, ~, ~, ~, x_inits, x_goals, ~] ...
            = deal_param(param);

    t = t0:dt:tf;                   % time horizon

    XX = cell(1, N_agents);         % state trajectories
    UU = cell(1, N_agents);         % input trajectories

    N_sim = size(t,2);              % simulation steps

    solutions = cell(1, N_agents);  % solution trajectories

    for i = 1:N_agents
        % Set sizes of state, input and solution trajectories
        XX{i} = zeros(nx, N_sim);
        UU{i} = zeros(nu, N_sim - 1);
        solutions{i} = zeros(size(elem.z{i}, 1),  N_sim - 1);

        % Set initial conditions
        XX{i}(:, 1) = x_inits{i};
    end

    disp('---------------------------------------------------------')
    fprintf('Iteration: %i/%i \n',1,N_sim);

    for k = 1:N_agents
        fprintf('Current State - Agent %i => x: %.2f, y: %.2f, z: %.2f \n',...
            k, x_inits{k}(1), x_inits{k}(3), x_inits{k}(5));
    end
   
    %% DMPC Loop
    for i = 2:N_sim
        disp('---------------------------------------------------------')
        fprintf('Iteration: %i/%i \n',i,N_sim);

        % Solve the distributed optimization problem
        disp('---------------------------------------------------------')
        fprintf('Solving the Problem \n')
        tic

        iter = i;
        solADMM  = run_ADMM_nlp(sProb, opts);
        for j = 1:N_agents
            solutions{j}(:, iter - 1) = solADMM.xxOpt{j};
        end
        toc

        % Extract the first part of the optimal input trajectory
        disp('---------------------------------------------------------')
        fprintf('Extracting and Applying the Input \n')
        tic
        [~, U_star] = extract_XX_UU(param, solADMM, 'predicted input', 1);

        for k = 1:N_agents
            fprintf('Current Input - Agent %i => u1: %.2f, u2: %.2f, u3: %.2f \n',...
                k, U_star{k}(1), U_star{k}(2), U_star{k}(3));
        end

        % Apply the input
        [XX, UU, param] = move_subsystems(param, iter, XX, UU, U_star);
        toc

        disp('---------------------------------------------------------')
        fprintf('Reinitializing the Problem \n')
        % Reinitialize the problem with new initial conditions
        tic
        sProb = reinit_sProb(param, elem, sProb, solADMM);
        toc

        [X_star, ~] = extract_XX_UU(param, solADMM, 'predicted state', 2);

        for k = 1:N_agents
            fprintf('Predicted State - Agent %i => x: %.2f, y: %.2f, z: %.2f \n',...
                k, X_star{k}(1), X_star{k}(3), X_star{k}(5));
            fprintf('Current State - Agent %i => x: %.2f, y: %.2f, z: %.2f \n \n',...
                k, param.x_inits{k}(1), param.x_inits{k}(3), param.x_inits{k}(5));
        end

    end

    %% Plots
    predicted_input_traj = cell(1, N_agents);

    for i = 1:N_agents
        temp = zeros(nu, N);
        input_offset = (N+1)*nx;

        for j = 0:N-1
            temp(:, j+1) = solutions{i}( input_offset + (j*nu) + 1 : input_offset + ((j+1)*nu), 1);
        end
        predicted_input_traj{i} = temp;
    end

    plotStates(t, XX, x_inits, x_goals, N_agents)
    plotInputs(t, UU, N_agents)
    plotInputs(t, predicted_input_traj, N_agents)

end

%% Move the Subsystems
function [XX, UU, param] = move_subsystems(param, iter, XX, UU, U_star)
    
    % Deal parameters
    [N_agents, ~, dt, ~, ~, m, ~, ~, ~, ~, x_inits, ~, ~] ...
        = deal_param(param);
    
    % Move the subsystems
    for i = 1:N_agents
        xf = rk4(@(x,u)quadrotor_model(x, u, m), dt, XX{i}(:,iter-1), U_star{i});
        XX{i}(:, iter) = xf;
        UU{i}(:, iter-1) = U_star{i};
        x_inits{i} = xf;
    end

    param.x_inits = x_inits;
    
end

%% Reinitialize sProb
function sProb = reinit_sProb(param, elem, sProb, solADMM)
    
    import casadi.*

    % Deal parameters
    [N_agents, ~, ~, nx, ~, ~, ~, ~, ~, ~, x_inits, ~, ~] ...
        = deal_param(param);

    % Deal elements
    [z, ~, ~, g, ~, ~, ~, ~, zz0, lam0] ...
        = deal_elem(elem);
    
    % Re-initialize the problem
    for i = 1:N_agents
        g{i}(1:nx) = z{i}(1:nx) - x_inits{i}; 
        g{i} = Function(['g' num2str(i)],{z{i}},{g{i}});
        
        % Warm-start
        zz0{i} = solADMM.xxOpt{i};
        lam0{i} = solADMM.llam{i}.eqlin;
    end   
    
   sProb.locFuns.ggi = g; 
   sProb.zz0 = zz0;
   sProb.lam0 = lam0;
   
end

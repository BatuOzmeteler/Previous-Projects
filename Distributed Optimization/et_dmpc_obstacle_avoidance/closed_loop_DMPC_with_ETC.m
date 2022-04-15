%% Closed-loop DMPC with Event-triggering Mechanism
% Authors: 
% - Mehmet Batu Özmeteler
% - Shehabeldine Housein
% - Ricardo França
%% Summary
% This function runs a closed-loop DMPC simulation 
% with an event-triggering mechanism.
%%
function closed_loop_DMPC_with_ETC(param, opts, t0, tf)
    %% Initial Problem Creation
    disp('Creating the Initial Problem')
    tic 
    [sProb, elem] = create_sProb(param);
    toc

    %% Simulation Initialization
    [N_agents, N, dt, nx, nu, ~, ~, ~, ~, ~, x_inits, x_goals, ~,...
        size_obs, pos_obs, vel_obs] = deal_param(param);

    t = t0:dt:tf;                   % time horizon
    N_sim = size(t,2);              % simulation steps
    
    N_pointer = 1;                  % keeps track of which input to apply
    triggers = [];                  % trigger times
    
    XX = cell(1, N_agents);         % state trajectories
    UU = cell(1, N_agents);         % input trajectories
    SS = cell(1, N_agents);         % solution trajectories
    
    XX_sol = cell(1, N_agents);     % truncated state trajectories
    UU_sol = cell(1, N_agents);     % truncated input trajectories
    
    XX_pred = cell(1, N_agents);    % predicted state trajectories
    UU_pred = cell(1, N_agents);    % predicted input trajectories
    
    U_star = cell(1, N_agents);     % applied inputs
    X_star = cell(1, N_agents);     % predicted next states
    
    noise = cell(1, N_agents);      % disturbances
    
    pos_rob = zeros(N_agents, 3);   % positions of robots (xyz)
    
    for i = 1:N_agents
        XX{i} = zeros(nx, N_sim);
        UU{i} = zeros(nu, N_sim - 1);
        SS{i} = double.empty(size(elem.z{i}, 1),  0);
        
        noise{i} = zeros(nx, 1);    
        
        X_star{i} = x_inits{i};
        U_star{i} = zeros(nu, 1);
        
        XX{i}(:, 1) = x_inits{i};
    end
    
    pos_rob = update_pos_rob(param, pos_rob);
    
    %% Limits for Event Triggering 
    % Limits to trigger an event if an obstacle is within
    x_lim = 1.5; y_lim = 1.5; z_lim = 1.5;   
    pos_lims = [x_lim, y_lim, z_lim];

    % Limit to trigger an event if the current trajectory diverges 
    div_lim = 1;   
    
    %% DMPC Loop
    disp('---------------------------------------------------------')
    fprintf('Iteration: %i/%i \n',1,N_sim);

    for k = 1:N_agents
        fprintf('Current State - Agent %i => x: %.2f, y: %.2f, z: %.2f \n',...
            k, x_inits{k}(1), x_inits{k}(3), x_inits{k}(5));
    end
    
    for i = 2:N_sim
        iter = i;
        
        disp('---------------------------------------------------------')
        fprintf('Iteration: %i/%i \n', iter, N_sim);

        % Check if an event is triggered
        etc = check_etc(param, pos_rob, pos_lims, div_lim,...
            X_star, N_pointer, iter);
        
        % If an event is triggered, solve the problem
        if etc
            %Record the times when an event is triggered
            triggers = [triggers, (iter-2)*dt];

            % Solve the distributed optimization problem
            disp('---------------------------------------------------------')
            fprintf('Solving the Problem \n')
            tic
            solADMM  = run_ADMM_nlp(sProb, opts); 
            for j = 1:N_agents
                SS{j} = [SS{j}, solADMM.xxOpt{j}];
                XX_pred{j} = [XX_pred{j}; solADMM.xxOpt{j}( 1 : (N+1)*nx )];
                UU_pred{j} = [UU_pred{j}; solADMM.xxOpt{j}( (N+1)*nx + 1 : (N+1)*nx + N*nu )];
            end
            toc
            
            % Reset the pointer to the first input
            N_pointer = 1;
        end
        
        % Extract the relevant part of the predicted input trajectory
        disp('---------------------------------------------------------')
        fprintf('Extracting and Applying the Input \n')
        tic
        [~, U_star] = extract_XX_UU(param, solADMM, 'predicted input', N_pointer);

        for k = 1:N_agents
            fprintf('Current Input - Agent %i => u1: %.2f, u2: %.2f, u3: %.2f \n',...
                k, U_star{k}(1), U_star{k}(2), U_star{k}(3));
        end

        % Apply the input
        [XX, UU, param] = move_subsystems(param, iter, XX, UU, U_star, noise);
        toc

        % Update robot positions
        pos_rob = update_pos_rob(param, pos_rob);
        
        % Move the pointer to the next input from the optimal solution
        N_pointer = N_pointer + 1;
        
        % Reinitialize the problem with new initial conditions
        disp('---------------------------------------------------------')
        fprintf('Reinitializing the Problem \n')
        tic
        sProb = reinit_sProb(param, elem, sProb, solADMM);
        toc

        % Extract the relevant part of the predicted state trajectory
        [X_star, ~] = extract_XX_UU(param, solADMM, 'predicted state', N_pointer);

        for k = 1:N_agents
            XX_sol{k} = XX{k}(:,1:iter);
            UU_sol{k} = UU{k}(:,1:iter-1);
            
            fprintf('Predicted State - Agent %i => x: %.2f, y: %.2f, z: %.2f \n',...
                k, X_star{k}(1), X_star{k}(3), X_star{k}(5));
            fprintf('Current State - Agent %i => x: %.2f, y: %.2f, z: %.2f \n \n',...
                k, param.x_inits{k}(1), param.x_inits{k}(3), param.x_inits{k}(5));
        end        
        
        plot_multi_robot_ADMM(XX_sol, UU_sol, XX_pred, UU_pred, t(1:iter), tf, param, triggers)
    end

    %% Plots
    plotStates(t, XX, x_inits, x_goals, N_agents)
    plotInputs(t, UU, N_agents)

end

%% Move the Subsystems
function [XX, UU, param] = move_subsystems(param, iter, XX, UU, U_star, noise)
    
    % Deal parameters
    [N_agents, ~, dt, ~, ~, m, ~, ~, ~, ~, x_inits, ~, ~, ~, ~, ~] ...
        = deal_param(param);
    
    % Move the subsystems
    for i = 1:N_agents
        xf = rk4(@(x,u)quadrotor_model(x, u, m), dt, x_inits{i}, U_star{i});
        
        noisy_xf = xf + noise{i};
        
        XX{i}(:, iter) = noisy_xf;
        UU{i}(:, iter-1) = U_star{i};
        x_inits{i} = noisy_xf;
    end

    param.x_inits = x_inits;
    
end

%% Reinitialize sProb
function sProb = reinit_sProb(param, elem, sProb, solADMM)
    
    import casadi.*

    % Deal parameters
    [N_agents, ~, ~, nx, ~, ~, ~, ~, ~, ~, x_inits, ~, ~, ~, ~, ~] ...
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

%% Update Robot Positions
function pos_rob = update_pos_rob(param, pos_rob)

    % Deal parameters
    [N_agents, ~, ~, ~, ~, ~, ~, ~, ~, ~, x_inits, ~, ~, ~, ~, ~] ...
        = deal_param(param);
    
    for i = 1:N_agents
       pos_rob(i, :) = [x_inits{i}(1), x_inits{i}(3), x_inits{i}(5)];
    end
    
end

%% Check If an Event Occurs
function etc = check_etc(param, pos_rob, pos_lims, div_lim, X_star, N_pointer, iter)

        % Deal parameters
        [N_agents, N, ~, ~, ~, ~, ~, ~, ~, ~, x_inits, ~, ~, ~, pos_obs, ~] ...
            = deal_param(param);
    
        % Initialize the event-triggering flag
        etc = 0;    
        
        % Event-triggering Criteria #1
        % Recompute at the beginning of every horizon
        if iter == 2 || N_pointer > N
            fprintf('Recalculating at the Beginning of the Horizon \n')
            etc = 1;
        end
     
        % Event-triggering Criteria #2
        % Recompute if an obstacle is within limits     
        for i = 1:N_agents
            for j = 1:size(pos_obs,1)
                if abs(pos_rob(i,:) - pos_obs(j,:)) - pos_lims <= 0
                    fprintf('Recalculating due to Obstacle \n')
                    etc = 1;
                end
            end
        end
        
        % Event-triggering Criteria #3
        % Recompute if the trajectories diverge more than the limit
        for i = 1:N_agents
            error_pos = abs(x_inits{i} - X_star{i}) - div_lim;
            if error_pos(1) > 0 || error_pos(3) > 0 || error_pos(5) > 0
                fprintf('Recalculating due to Divergence \n')
                etc = 1;
            end
        end
        
end









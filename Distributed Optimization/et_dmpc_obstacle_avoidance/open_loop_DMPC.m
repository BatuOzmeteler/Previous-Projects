%% Open-loop DMPC Simulation
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function runs an open-loop DMPC 
% simulation with a certain parameter set.
%%
function open_loop_DMPC(param, opts)
    %% Problem Creation
    disp('Creating the Problem')
    tic
    [sProb, elem] = create_sProb(param);
    toc

    %% Problem Solution
    disp('Solving the Problem')
    solADMM  = run_ADMM_nlp(sProb, opts);

    %% Plots
    t = 0:param.dt:param.N*param.dt;

    [XX, UU] = extract_XX_UU(param, solADMM, 'trajectory', 1);

    plotStates(t, XX, param.x_inits, param.x_goals, param.N_agents)
    plotInputs(t, UU, param.N_agents)
end

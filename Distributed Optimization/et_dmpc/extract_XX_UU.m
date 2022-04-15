%% Extract State and Input Trajectories
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function extracts the optimal state, input trajectories, the relevant part
% of the input trajectory or the predicted state trajectory from solADMM.
%%
function [XX, UU] = extract_XX_UU(param, solADMM, mode, N_iter)
    
    % Deal parameters
    [N_agents, N, ~, nx, nu, ~, ~, ~, ~, ~, ~, ~, ~] ...
        = deal_param(param);
    
    XX = cell(1, N_agents);
    UU = cell(1, N_agents);
    input_offset = (N+1)*nx;
    
    if strcmp(mode, 'trajectory')
        % Extract input and state trajectories
        for i = 1:N_agents
            temp_X = [];
            temp_U = [];
            for j = 0:N-1
                temp_X = [temp_X, solADMM.xxOpt{i}((j*nx)+1 : (j+1)*nx)];
                temp_U = [temp_U, solADMM.xxOpt{i}(input_offset + (j*nu)+1 : input_offset + (j+1)*nu)];
            end
            temp_X = [temp_X, solADMM.xxOpt{i}((N*nx)+1 : (N+1)*nx)];

            XX{i} = temp_X;
            UU{i} = temp_U;
        end
    elseif strcmp(mode, 'predicted input')
        % Extract the relevant part of the predicted inputs
        for i = 1:N_agents
            UU{i} = solADMM.xxOpt{i}(input_offset + (nu*(N_iter - 1)) + 1 : ...
                input_offset + (nu*(N_iter - 1)) + nu);
        end    
    elseif strcmp(mode, 'predicted state')
        % Extract the relevant part of the predicted states
        for i = 1:N_agents
            XX{i} = solADMM.xxOpt{i}((nx*(N_iter - 1)) + 1 : (nx*(N_iter - 1)) + nx);
        end  
    end
end

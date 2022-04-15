%% Build Inequality Constraints
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function builds h which contains the inequality constraints
% (obstacle avoidance) for the problem creation.
%%
function elem = build_h(param, data, elem)
    
    % Deal parameters
    [N_agents, N, dt, nx, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~] ...
        = deal_param(param);
    
    % Deal elements
    [z, ~, ~, ~, h, ~, ~, ~, ~, ~] ...
        = deal_elem(elem);
    
    % Build A_obs, b_obs
    [A_obs, b_obs] = build_A_obs_b_obs(param, data);
    
    % Build h
    t = 0:dt:N*dt;
    
    for i = 1:N_agents
        temp_h = [];
        temp_xyzt = [];
        
        for j = 0:N
            temp_xyzt = [z{i}((j*nx)+1); z{i}((j*nx)+3); z{i}((j*nx)+5);  t(j+1)];
            temp_h = A_obs{i}*temp_xyzt - b_obs{i};
            
            h{i}(j*size(b_obs{i}, 1) + 1 : (j+1)*size(b_obs{i}, 1)) = temp_h;
        end
    end

    elem.h = h;
    
end

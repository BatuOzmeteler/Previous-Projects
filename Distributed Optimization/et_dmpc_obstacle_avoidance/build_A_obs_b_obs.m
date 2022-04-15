%% Build Obstacle Avoidance Constraints 
% Authors: 
% - Mehmet Batu Özmeteler
% - Shehabeldine Housein
%% Summary
% This function builds A_obs, b_obs which contains the obstacle
% avoidance constraints (free convex space in xyzt plane) for 
% the problem creation.
%%
function [A_obs, b_obs] = build_A_obs_b_obs(param, data)

    % Deal parameters
    [N_agents, N, dt, ~, ~, ~, d, ~, ~, ~, x_inits, ~,...
    ~, size_obs, pos_obs, vel_obs] = deal_param(param);

    % Deal data
    [bounds, ~, ~, ~, ~, ~, ~] ...
        = deal_data(data);

    % Build A_obs, b_obs        
    map_bounds = [bounds{1}(1), bounds{2}(1); bounds{1}(3), bounds{2}(3); bounds{1}(5), bounds{2}(5)];
    [A_obs, b_obs] = free_pose_time_space(0, N*dt, dt, size_obs, pos_obs, vel_obs, map_bounds, x_inits, d, N_agents);
    
end

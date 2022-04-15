%% Build Obstacle Avoidance Constraints 
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function builds A_obs, b_obs which contains the obstacle
% avoidance constraints (free convex space in xyzt plane) for 
% the problem creation.
%%
function [A_obs, b_obs] = build_A_obs_b_obs(param, data)

    % Deal parameters
    [N_agents, N, dt, ~, ~, ~, d, ~, ~, ~, x_inits, ~, ~] ...
        = deal_param(param);

    % Deal data
    [bounds, ~, ~, ~, ~, ~, ~] ...
        = deal_data(data);

    % Build A_obs, b_obs    
    size_obs = 0.5;
    obs_x0 = [0, 0, 0];
    obs_v0 = [1.0, 1.2, 1.2];
    
    map_bounds = [bounds{1}(1), bounds{2}(1); bounds{1}(3), bounds{2}(3); bounds{1}(5), bounds{2}(5)];
    
    [A_obs, b_obs] = free_pose_time_space(0, N*dt, dt, size_obs, obs_x0, obs_v0, map_bounds, x_inits, d, N_agents);
    
end

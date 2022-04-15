%% Build Neighbours 
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function builds the neighbours for the problem creation.
%%
function [neighbours, n_neighbours] = build_neighbours(param)

    % Deal parameters
    [N_agents, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~] ...
        = deal_param(param);

    % Build neighbours
    neighbours = cell(1, N_agents);
    
    if N_agents == 2
       for i = 1:N_agents
           if i == 1
            neighbours{i} = 2;
           else
            neighbours{i} = 1;
           end
       end
       n_neighbours = 1;
    else
       for i = 1:N_agents
           if i == 1 || i == 3
            neighbours{i} = [2,4];             
           else
            neighbours{i} = [1,3]; 
           end
       end
       n_neighbours = 2;
    end
    
end

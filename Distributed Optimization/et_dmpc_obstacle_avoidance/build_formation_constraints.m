%% Build Formation Constraints for Agent i
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function builds the formation constraints 
% for agent i for the problem creation.
%%
function data_xy = build_formation_constraints(param, elem, agent_id)

    % Deal parameters
    [N_agents, N, ~, nx, nu, ~, d, ~, ~, ~, ~, ~, ~, ~, ~, ~] ...
    = deal_param(param); 
    
    % Deal elements
    [z, ~, ~, ~, ~, ~, ~, ~, ~, ~] ...
        = deal_elem(elem);

    neighbour_off1 = (N+1)*nx + N*nu;               % first neighbour offset 
    neighbour_off2 = neighbour_off1 + N*nx;         % second neighbour offset
    
    data_xy = [];
    
    if N_agents == 2
        for k = 0:N-1
            % Position error on x and y relative to the first neighbour 
            data_x1 = z{agent_id}((k*nx) + 1) - z{agent_id}(neighbour_off1 + (k*nx) + 1);
            data_y1 = z{agent_id}((k*nx) + 3) - z{agent_id}(neighbour_off1 + (k*nx) + 3);
            
            switch agent_id
                case 1
                    % Agent 1's position relative to agent 2;
                    data_x1 = data_x1 + d;
                    data_y1 = data_y1;
                otherwise
                    % Agent 2's position relative to agent 1;
                    data_x1 = data_x1 - d;
                    data_y1 = data_y1;
            end
            data_xy = [data_xy; data_x1; data_y1];
        end      
    else
        for k = 0:N-1
            % Position error on x and y relative to the first neighbour 
            data_x1 = z{agent_id}((k*nx) + 1) - z{agent_id}(neighbour_off1 + (k*nx) + 1);
            data_y1 = z{agent_id}((k*nx) + 3) - z{agent_id}(neighbour_off1 + (k*nx) + 3);
            
            % Position error on x and y relative to the second neighbour 
            data_x2 = z{agent_id}((k*nx) + 1) - z{agent_id}(neighbour_off2 + (k*nx) + 1);
            data_y2 = z{agent_id}((k*nx) + 3) - z{agent_id}(neighbour_off2 + (k*nx) + 3);

            switch agent_id
                case 1
                    % Agent 1's position relative to agent 2;
                    data_x1 = data_x1 + d;
                    data_y1 = data_y1;
                    
                    % Agent 1's position relative to agent 4;
                    data_x2 = data_x2;
                    data_y2 = data_y2 + d;
                    
                case 2
                    % Agent 2's position relative to agent 1;
                    data_x1 = data_x1 - d;
                    data_y1 = data_y1;
                    
                    % Agent 2's position relative to agent 3;
                    data_x2 = data_x2;
                    data_y2 = data_y2 + d;
                    
                case 3
                    % Agent 3's position relative to agent 2;
                    data_x1 = data_x1;
                    data_y1 = data_y1 - d;
                    
                    % Agent 3's position relative to agent 4;
                    data_x2 = data_x2 - d;
                    data_y2 = data_y2;
                    
                otherwise
                    % Agent 4's position relative to agent 1;
                    data_x1 = data_x1;
                    data_y1 = data_y1 - d;
                    
                    % Agent 4's position relative to agent 3;
                    data_x2 = data_x2 + d;
                    data_y2 = data_y2;
            end
            data_xy = [data_xy; data_x1; data_y1; data_x2; data_y2];
        end
    end
    
end

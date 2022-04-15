%% Build Goal Vector for Agent i
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function builds the goal vector 
% for agent i for the problem creation.
%%
function elem = build_f(param, data, elem)

    % Deal parameters
    [N_agents, N, ~, nx, nu, ~, ~, ~, ~, ~, ~, x_goals, ~] ...
        = deal_param(param);
    
    % Deal data
    [~, neighbours, n_neighbours, n_z, ~, ~, ~] ...
        = deal_data(data);
    
    % Deal elements
    [z, H_bar, f, ~, ~, ~, ~, ~, ~, ~] ...
        = deal_elem(elem);
    
    % Build f
    input_offset = (N+1)*nx;
    copy_offset = input_offset + N*nu;
    
    for i = 1:N_agents
        
        % Build z_goal
        z_goal = zeros(n_z, 1);
        
        for j = 0:N-1
            z_goal((nx*j)+1 : (j+1)*nx) = x_goals{i};
            z_goal(input_offset + (nu*j)+1 : input_offset + (j+1)*nu) = zeros(nu, 1);
        end

        z_goal((nx*N)+1 : (N+1)*nx) = x_goals{i};

        for j = 0:n_neighbours-1
            neighbour_id = neighbours{i}(j+1);
            for k = 0:N-1
                z_goal( copy_offset + (N*nx)*j + (nx*k)+1 : ... 
                        copy_offset + (N*nx)*j +(k+1)*nx ) = x_goals{neighbour_id};
            end
        end
        
        f{i} = 0.5 * (z{i} - z_goal).'*H_bar{i}*(z{i} - z_goal);
    end
    
    elem.f = f;
    
end

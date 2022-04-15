%% Build Equality Constraints
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function builds g which contains the equality constraints
% (dynamics + formation) for the problem creation.
%%
function elem = build_g(param, elem)

    % Deal parameters
    [N_agents, N, dt, nx, nu, m, ~, ~, ~, ~, x_inits, x_goals, term_constraint] ...
    = deal_param(param);
    
    % Deal elements
    [z, ~, ~, g, ~, ~, ~, ~, ~, ~] ...
        = deal_elem(elem);

    % Set initial states
    for i = 1:N_agents
        g{i}(1:nx) = z{i}(1:nx) - x_inits{i}; 
    end

    % Set equality constraints for dynamics
    offset_z = (N+1)*nx;
    
    if term_constraint
        offset_g = (N+2)*nx; % with terminal constraint
    else
        offset_g = (N+1)*nx; % without terminal constraint
    end
    
    for i = 1:N_agents
        for j = 0:N-1
            xf = rk4(@(x,u)quadrotor_model(x, u, m), dt, z{i}((nx*j)+1:nx*(j+1)),...
                z{i}( offset_z + (nu*j)+1 : offset_z + nu*(j+1) ));

            g{i}((nx*(j+1))+1:nx*((j+1)+1)) =  xf - z{i}( (nx*(j+1))+1 : nx*((j+1)+1) );    % xf - x_k+1 = 0
        end
        % Set terminal constraint
        if term_constraint
            g{i}((offset_g - nx) + 1 : offset_g) =  z{i}( (offset_z - nx) + 1 : offset_z ) - x_goals{i};
        end
        
        % Set formation constraints
        g{i}(offset_g + 1 : end) = build_formation_constraints(param, elem, i);
    end
    
    elem.g = g;
    
end

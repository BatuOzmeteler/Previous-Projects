%% Build Initial Guesses
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function builds the initial guesses for the 
% decision vectors and the Lagrange multipliers.
%%
function elem = build_zz0_lam0(param, data, elem)
    
    % Deal parameters
    [N_agents, N, ~, ~, nu, ~, ~, ~, ~, ~, x_inits, ~, ~, ~, ~, ~] ...
    = deal_param(param);

    % Deal data
    [~, neighbours, n_neighbours, ~, ~, ~, n_cc] ...
        = deal_data(data);

    % Deal elements
    [~, ~, ~, ~, ~, ~, ~, ~, zz0, lam0] ...
        = deal_elem(elem);
    
    % Build zz0, lam0
    for i = 1:N_agents
        temp_xi_init = repmat(x_inits{i}, [N+1 1]);
        temp_ui_init = zeros(N*nu, 1);
        temp = [temp_xi_init; temp_ui_init];
        for j = 1:n_neighbours
           neighbour_id = neighbours{i}(j);
           temp_xj_init = repmat(x_inits{neighbour_id}, [N 1]);
           temp = [temp; temp_xj_init];
        end

       zz0{i} = temp;
       lam0{i} =  zeros(n_cc, 1);
    end
    
    elem.zz0 = zz0;
    elem.lam0 = lam0;
    
end

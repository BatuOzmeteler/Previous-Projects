%% Build Upper and Lower Bounds for the Decision Vector
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function builds llbx, uubx which contains
% the upper and lower bounds for the decision 
% vector for the problem creation.
%%
function elem = build_llbx_uubx(param, data, elem)
    
    % Deal parameters
    [N_agents, N, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~, ~] ...
        = deal_param(param);

    % Deal data
    [bounds, ~, n_neighbours, ~, ~, ~, ~] ...
        = deal_data(data);
    
    % Deal elements
    [~, ~, ~, ~, ~, ~, llbx, uubx, ~, ~] ...
        = deal_elem(elem);
    
    % Build llbx, uubx
    temp_lbx = repmat(bounds{1}, [N 1]);
    temp_ubx = repmat(bounds{2}, [N 1]);
    
    temp_lbu = repmat(bounds{3}, [N 1]);
    temp_ubu = repmat(bounds{4}, [N 1]);    
    
    for i = 1:N_agents
       llbx{i} = [temp_lbx; bounds{1}; temp_lbu];
       uubx{i} = [temp_ubx; bounds{2}; temp_ubu];
       for j = 1:n_neighbours
            llbx{i} = [llbx{i}; temp_lbx];
            uubx{i} = [uubx{i}; temp_ubx];
       end
    end
    
    elem.llbx = llbx;
    elem.uubx = uubx;
    
end

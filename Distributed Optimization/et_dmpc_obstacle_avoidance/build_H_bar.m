%% Build H_bar
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function builds the H_bar matrix which contains 
% the Q, R and P matrices for the problem creation.
%%
function elem = build_H_bar(param, data, elem)

    % Deal parameters
    [N_agents, N, ~, nx, nu, ~, ~, wQ, wR, wP, ~, ~, ~, ~, ~, ~] ...
    = deal_param(param);

    % Deal data
    [~, neighbours, n_neighbours, ~, ~, ~, ~] ...
        = deal_data(data);
    
    % Deal elements
    [~, H_bar, ~, ~, ~, ~, ~, ~, ~, ~] ...
        = deal_elem(elem);
    
    % Initialize matrices
    Q = cell(1, N_agents);
    R = cell(1, N_agents);
    P = cell(1, N_agents);
    H = cell(1, N_agents);
    
    Q_bar = cell(1, N_agents);
    R_bar = cell(1, N_agents);
    
    % Fill Q, R, P, Q_bar, R_bar matrices
    for i = 1:N_agents
       Q{i} = wQ*eye(nx)/(n_neighbours + 1);
       R{i} = wR*eye(nu);
       P{i} = wP*eye(nx);
  
       Q_bar{i} = kron(Q{i}, eye(N)); 
       R_bar{i} = kron(R{i}, eye(N));
    end
    
    % Fill H, H_bar matrices
    for i = 1:N_agents
        H{i} = {Q_bar{i}, P{i}, R_bar{i}};
        temp = H{i};
        for j = 1:n_neighbours
            temp{3 + j} = Q_bar{neighbours{i}(j)};
        end
        H{i} = temp;
        H_bar{i} = blkdiag(H{i}{:});
    end
    
    elem.H_bar = H_bar;
    
end

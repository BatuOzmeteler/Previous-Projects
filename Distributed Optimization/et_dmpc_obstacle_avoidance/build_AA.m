%% Build Coupling Constraints
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function builds the consensus matrix AA which contains 
% the coupling constraints for the problem creation.
%%
function elem = build_AA(param, data, elem)
    
    % Deal parameters
    [N_agents, N, ~, nx, nu, ~, ~, ~, ~, ~, ~, ~,...
    ~, ~, ~, ~] = deal_param(param);
    
    % Deal data
    [~, ~, n_neighbours, ~, ~, ~, ~] ...
        = deal_data(data);
    
    % Deal elements
    [~, ~, ~, ~, ~, AA, ~, ~, ~, ~] ...
        = deal_elem(elem);
    
    % Set block sizes
    size_x = N*nx;                      % state block size
    size_xN = nx;                       % terminal state block size
    size_u = N*nu;                      % input block size
    size_nn = size_x*n_neighbours;      % copy block size

    sizes = [size_x, size_xN, size_u, size_nn];
    
    % Create blocks
    local_var = create_local_variable_block(sizes, n_neighbours); 
    copy_vars = create_copy_variable_blocks(sizes, n_neighbours);
    zero_var = zeros(size_nn, sum(sizes));
    
    % Build AA
    if N_agents == 2
        % N_agents = 2
        % local_var = [I 0 0 0] 
        % copy_vars = {0 0 0 -I}
        
        AA{1} = [local_var; copy_vars{1}];
        AA{2} = [copy_vars{1}; local_var];
    else
        % N_agents = 4
        % local_var =  [I 0 0 0 0
        %               I 0 0 0 0] 
        % copy_vars = {0 0 0 0  0,  0 0 0 0  0,  0 0 0 -I 0,  0 0 0 0 -I}
        %             {0 0 0 -I 0,  0 0 0 0 -I,  0 0 0  0 0,  0 0 0 0  0}  
        
        AA{1} = [local_var; copy_vars{3}; zero_var; copy_vars{4}];
        AA{2} = [copy_vars{3}; local_var; copy_vars{4}; zero_var];
        AA{3} = [zero_var; copy_vars{1}; local_var; copy_vars{2}];
        AA{4} = [copy_vars{1}; zero_var; copy_vars{2}; local_var];
    end
    
    elem.AA = AA;
    
end

function local_var = create_local_variable_block(sizes, n_neighbours)
    
    local_var = [];
    local_to_add = [ eye(sizes(1)), zeros(sizes(1), sizes(2)), zeros(sizes(1), sizes(3)), zeros(sizes(1), sizes(4)) ];
    
    for j = 1:n_neighbours
        local_var = [local_var; local_to_add];
    end
    
end

function copy_vars = create_copy_variable_blocks(sizes, n_neighbours)
    
    copy_to_add = cell(1, n_neighbours);

    for i = 1:n_neighbours
        base_copy = [ zeros(sizes(1), sizes(1)), zeros(sizes(1), sizes(2)), zeros(sizes(1), sizes(3))];
        for j = 1:n_neighbours
            if i == j
                base_copy = [base_copy, -eye(sizes(1))];
            else
                base_copy = [base_copy, zeros(sizes(1), sizes(1))];
            end
        end
        copy_to_add{i} = base_copy;
    end
    
    temp = [];
    for i = 1:n_neighbours
        copy_var = [];
        for j = 1:n_neighbours
            if i == j
                copy_var = [copy_var; copy_to_add{i}];
            else
                copy_var = [zeros(sizes(1), sum(sizes)); copy_var];
            end
        end
        temp = [ temp ; copy_var ];
    end

    for i = 1:n_neighbours
        copy_var = [];
        for j = 1:n_neighbours
            if i == j
                copy_var = [copy_to_add{i}; copy_var];
            else
                copy_var = [copy_var; zeros(sizes(1), sum(sizes))];
            end
        end
        temp = [ temp ; copy_var ];
    end    
    
    copy_vars = cell(1, n_neighbours*n_neighbours);
    
    for i = 1:n_neighbours*n_neighbours
        j = i - 1;
        copy_vars{i} = temp(j*n_neighbours*sizes(1) + 1 : n_neighbours*(j+1)*sizes(1), :);
    end
    
end

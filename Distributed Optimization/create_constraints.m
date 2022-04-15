
% constraint = [idx_mat_lhs idx_block_lhs idx_mat_rhs idx_block_rhs];
function constraints = create_constraints(N_agents, n_cc, neighbours)
    constraints = cell(1,n_cc);
    idx_cc = 1;
    for i = 1:N_agents
        idx_mat_lhs = i;
        idx_block_lhs = 1;
         for j = 1:size(neighbours{i},2)
            idx_mat_rhs = neighbours{i}(j);
            if idx_mat_rhs == 1 || idx_mat_rhs == N_agents
                idx_block_rhs = 4;
            else
                if idx_mat_lhs < idx_mat_rhs
                    idx_block_rhs = 4;
                else
                    idx_block_rhs = 5;
                end
            end
            constraints{idx_cc} = [idx_mat_lhs idx_block_lhs idx_mat_rhs idx_block_rhs];
            idx_cc = idx_cc + 1;
         end
    end
   
end
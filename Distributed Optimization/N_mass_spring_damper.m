%% A Coupled Spring-Mass-Damper System Model
% Authors: 
% - Mehmet Batu Özmeteler
%%
function [A_d, B_d, C_d, sizes_z_i, neighbours, H_bar, c_c,...
    A_eq, b_eq, A_ineq, b_ineq] = N_mass_spring_damper(N_agents, N, x0)

% N-agents is the number of agents
% N is the time horizon

% z vector contains all decision variables z_i

% z_i vector contains the state trajectory from k = 0 to k = N, the input
% trajectory from k = 0 to k = N - 1, and the state copy trajectories from
% k = 0 to k = N - 1 for every copy.

mass = ones(N_agents,1);         % masses 
k = 3*ones(N_agents-1,1);        % spring constants, N/m
d = 3*ones(N_agents-1,1);        % damping constants, Ns/m

T = 0.2;                         % step size for forward discretization

A_c = cell(N_agents,N_agents);   % system matrices (continuous-time)
B_c = cell(N_agents,N_agents);   % input matrices (continuous-time)
C_c = cell(1,N_agents);          % output matrices (continuous-time)

A_d = cell(N_agents,N_agents);   % system matrices (discrete-time)
B_d = cell(N_agents,N_agents);   % input matrices (discrete-time)
C_d = cell(1,N_agents);          % output matrices (discrete-time)

n_x = 2*ones(1,N_agents);        % number of states for agents 
n_u = ones(1,N_agents);          % number of inputs for agents

sizes_z_i = cell(1,N_agents);    % keeps the sizes of the decision vectors for agents
                                 
neighbours = cell(1,N_agents);   % keeps the indices of its neighbours

H = cell(1,N_agents);            
H_bar = cell(1,N_agents);        % Hessian matrices for agents

Q = cell(1,N_agents);            
Q_bar = cell(1,N_agents);        % Q matrices for agents

R = cell(1,N_agents);            
R_bar = cell(1,N_agents);        % R matrices for agents

P = cell(1,N_agents);            % P matrices for agents

c_c = cell(1,N_agents);          % keeps the coupling constraint matrices

A_eq = cell(1,N_agents);         % keeps the equality constraint matrices
b_eq = cell(1,N_agents);         

A_ineq = cell(1,N_agents);       % keeps the inequality constraint matrices
b_ineq = cell(1,N_agents);

% Fill system, input and output matrices
for i = 1:N_agents
    % A_ii is the system matrix for agent i. 
    % A_ij is the coupling matrix of agent i with agent j.
    if i == 1
        A_c{i,i} = [0,1;-k(i)/mass(i), -d(i)/mass(i)];
        A_c{i,i+1} = [0,0;k(i)/mass(i), d(i)/mass(i)];    
    elseif i == N_agents
        A_c{i,i} = [0,1;-k(i-1)/mass(i), -d(i-1)/mass(i)];
        A_c{i,i-1} = [0,0;k(i-1)/mass(i), d(i-1)/mass(i)];        
    else
        A_c{i,i} = [0,1;-(k(i-1)+k(i))/mass(i), -(d(i-1)+d(i))/mass(i)];
        A_c{i,i-1} = [0,0;k(i-1)/mass(i), d(i-1)/mass(i)];
        A_c{i,i+1} = [0,0;k(i)/mass(i), d(i)/mass(i)];
    end
    B_c{i,i} = [0;1/mass(i)];
    C_c{i} = [1,0];
end

% Apply Euler forward discretization
for i = 1:N_agents
    for j = 1:N_agents
        if i ~= j
            A_d{i,j} = T*A_c{i,j};
        else
            A_d{i,i} = eye(n_x(i)) + T*A_c{i,i};
        end
        B_d{i,i} = T*B_c{i,i};
        C_d{i} = C_c{i};
    end
end

% Calculate decision vector sizes and neighbour indices for each agent
n_cc = 0;      % amount of coupling constraints
n_eqc = N+1;   % amount of equality constraints
n_ineqc = 2*N; % amount of inequality constraints 

for i = 1:N_agents
   if i == 1 || i == N_agents
       if i == 1
        neighbours{i} = i + 1;
       end
       if i == N_agents
        neighbours{i} = i - 1;
       end
       n_cc = n_cc + 1;
       sizes_z_i{i} = (N+1)*n_x(i) + N*n_u(i) + N*n_x(i);
   else
       neighbours{i} = [i-1, i+1];
       n_cc = n_cc + 2;
       sizes_z_i{i} = (N+1)*n_x(i) + N*n_u(i) + N*2*n_x(i);
   end
end

% Fill Q, Q_bar, R, R_bar, P matrices
for i = 1:N_agents
   weight_q = 10;
   weight_r = 1;
   weight_p = 0;
   Q{i} = weight_q*eye(n_x(i));
   R{i} = weight_r*eye(n_u(i));
   P{i} = weight_p*eye(n_x(i));
   if i == 1 || i == N_agents
       Q{i} = weight_q*eye(n_x(i))/2;
   else
       Q{i} = weight_q*eye(n_x(i))/3;
   end
   Q_bar{i} = kron(Q{i}, eye(N)); 
   R_bar{i} = kron(R{i}, eye(N));
end

% Fill H, H_bar matrices
for i = 1:N_agents
    H{i} = {Q_bar{i}, P{i}, R_bar{i}};
    temp = H{i};
    for j = 1:size(neighbours{i}, 2)
        temp{3 + j} = Q_bar{neighbours{i}(j)};
    end
    H{i} = temp;
    H_bar{i} = blkdiag(H{i}{:});
end

c_constraints = create_constraints(N_agents, n_cc, neighbours);

% Set sizes for coupling, inequality and equality constraint matrices
for i = 1:N_agents
    c_c{i} = zeros(N*n_x(i)*n_cc, sizes_z_i{i}); % coupling constraints
    
    A_eq{i} = zeros(n_eqc*n_x(i), sizes_z_i{i}); % equality constraints (dynamics) 
    % for the whole horizon + initial condition
    b_eq{i} = zeros(n_eqc*n_x(i), 1);
    
    A_ineq{i} = zeros(n_ineqc*n_u(i), sizes_z_i{i}); % two inequality constraints per input
    b_ineq{i} = zeros(n_ineqc*n_u(i), 1);
end

% Fill c_c
for i = 0:n_cc-1
    cc_idx = i+1;
    c_constraint = c_constraints{cc_idx};
    for j = 1:N_agents 
        size_x = N*n_x(j);
        size_u = N*n_u(j);
        if j == c_constraint(1)
            if j == 1 || j == N_agents
                c_c{j}( ((i*size_x) + 1):((i+1)*size_x), : ) = [eye(size_x),...
                    zeros(size_x, n_x(j)), zeros(size_x, size_u), zeros(size_x, size_x)];
            else
                c_c{j}( ((i*size_x) + 1):((i+1)*size_x), : ) = [eye(size_x),...
                    zeros(size_x, n_x(j)), zeros(size_x, size_u), zeros(size_x, size_x),...
                    zeros(size_x, size_x)];
            end
        elseif j == c_constraint(3)
            if j == 1 || j == N_agents
                c_c{j}( ((i*size_x) + 1):((i+1)*size_x), : ) = [zeros(size_x, size_x),...
                    zeros(size_x, n_x(j)), zeros(size_x, size_u), -eye(size_x)];
            else
                if c_constraint(4) == 4
                    c_c{j}( ((i*size_x) + 1):((i+1)*size_x), : ) = [zeros(size_x, size_x),...
                    zeros(size_x, n_x(j)), zeros(size_x, size_u), -eye(size_x), zeros(size_x, size_x)];
                else
                    c_c{j}( ((i*size_x) + 1):((i+1)*size_x), : ) = [zeros(size_x, size_x),...
                    zeros(size_x, n_x(j)), zeros(size_x, size_u), zeros(size_x, size_x), -eye(size_x)];
                end
            end
        else
            if j == 1 || j == N_agents
               c_c{j}( ((i*size_x) + 1):((i+1)*size_x), : ) = [zeros(size_x, size_x),...
                    zeros(size_x, n_x(j)), zeros(size_x, size_u), zeros(size_x, size_x)];
            else
               c_c{j}( ((i*size_x) + 1):((i+1)*size_x), : ) = [zeros(size_x, size_x),...
                    zeros(size_x, n_x(j)), zeros(size_x, size_u), zeros(size_x, size_x),...
                    zeros(size_x, size_x)];
            end
        end   
    end
end

% Fill A_eq, b_eq
for i = 1:N_agents
    for j = 0:n_eqc-1
        if j ~= n_eqc-1
            temp = [zeros(n_x(i), j*n_x(i)), A_d{i,i}, -eye(n_x(i)), zeros(n_x(i), ((N-1)*n_x(i))-j), B_d{i,i}, ...
                    zeros(n_x(i), ((N-1)*n_u(i))+j), A_d{i,neighbours{i}(1)}];
            if size(neighbours{i}, 2) > 1
                temp = [temp, zeros(n_x(i), (N-1)*n_x(neighbours{i}(1))), A_d{i,neighbours{i}(2)}];
                temp = [temp, zeros(n_x(neighbours{i}(2)),sizes_z_i{i} - size(temp,2))];
            else
                temp = [temp, zeros(n_x(neighbours{i}(1)),sizes_z_i{i} - size(temp,2))];
            end
        else
            temp = [ eye(n_x(i)), zeros(n_x(i), (sizes_z_i{i} - n_x(i)) )];
            b_eq{i}(j*n_x(i)+1:end) = x0{i};
        end
        A_eq{i}((j*n_x(i))+1:(j+1)*n_x(i),:) = temp;
    end
end

% Fill A_ineq, b_ineq
H_u = [1;-1]; 
b_u = [1;1];
for i = 1:N_agents
    for j = 0:N-1
        temp = [zeros(2*n_u(i), j) ,zeros(2*n_u(i), 2*(N+1)), H_u];
        temp = [temp, zeros(2*n_u(i), sizes_z_i{i} - size(temp,2))];
        A_ineq{i}((j*2*n_u(i))+1:(j+1)*2*n_u(i),:) = temp;
        b_ineq{i}((j*2*n_u(i))+1:(j+1)*2*n_u(i), 1) = b_u;
    end
end
    
    
    
    


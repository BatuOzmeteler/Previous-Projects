function [x,globalFloats,localFloats,lambda,admm_iter] = alex_admm(H,grad,Aineq,bineq,Aeq,beq,A,x0,lambda0)
% Wrapper function: use run_ADMM of the Aladin-alpha toolbox for DCG paper.

N_agents = size(H,2);

import casadi.*

x = cell(1,N_agents);
f = cell(1,N_agents);
h = cell(1,N_agents);
g = cell(1,N_agents);
nx = zeros(1,N_agents);
for i = 1:N_agents
    varstring = sprintf('x%i',i);
    nx(i) = size(H{i},1);
    x{i} = sym(varstring,[nx(i), 1], 'real');    
end

x{1} = sym('x1',[nx(1), 1], 'real');

for i = 1:N_agents
   f{i} = 0.5*x{i}.'*H{i}*x{i} + grad{i}.'*x{i};
   if ~isempty(beq) && ~isempty(Aeq)
       g{i} = beq{i} - Aeq{i}*x{i};
   end
   h{i} = - bineq{i} + Aineq{i}*x{i};
end


%% convert symbolic variables to MATLAB functions
for i = 1:N_agents
   f{i} = matlabFunction(f{i},'Vars',x(i));
   if ~isempty(g{i})
       g{i} = matlabFunction(g{i},'Vars',x(i));
   else
       g{i} = @(x) [];
   end
   h{i} = matlabFunction(h{i},'Vars',x(i)); 
end
% g{1} = matlabFunction(g{1},'Vars',{x{1}}); 


%% build sProb
sProb.locFuns.ffi  = f;
sProb.locFuns.hhi  = h; %ineq
sProb.locFuns.ggi  = g; %eq

sProb.llbx = cell(1,N_agents);
sProb.uubx = cell(1,N_agents);
sProb.AA = A;



sProb.zz0 = x0;

sProb.lam0 = lambda0{1}; %somehow run_ADMM only expects one lambda0 vector, because it uses the same lambda0 for all subsystems


ADMMopts = struct('rho0',100,'scaling','false','rhoUpdate','false','maxiter',500,'plot','false','loggFl','false');

sol_ADMM         = run_ADMM_custom(sProb, ADMMopts);

x = sol_ADMM.xxOpt;
globalFloats = sol_ADMM.floats;
localFloats = 0;
lambda = sol_ADMM.lamOpt;
admm_iter = sol_ADMM.iter;








end
%% Deal Parameters 
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function deals the parameters to separate
% variables for the problem creation.
%%
function [N_agents, N, dt, nx, nu, m, d, wQ, wR, wP, x_inits, x_goals, term_constraint] = deal_param(param)

    N_agents = param.N_agents;
    N = param.N;
    dt = param.dt;

    nx = param.nx;
    nu = param.nu;

    m = param.m;
    d = param.d;

    wQ = param.wQ;
    wR = param.wR;
    wP = param.wP;

    x_inits = param.x_inits;
    x_goals = param.x_goals;

    term_constraint = param.term_constraint;
    
end

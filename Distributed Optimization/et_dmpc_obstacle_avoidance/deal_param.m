%% Deal Parameters 
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function deals the parameters to separate
% variables for the problem creation.
%%
function [N_agents, N, dt, nx, nu, m, d, wQ, wR, wP, x_inits, x_goals,...
    term_constraint, size_obs, pos_obs, vel_obs] = deal_param(param)

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
    
    size_obs = param.size_obs;
    pos_obs = param.pos_obs;
    vel_obs = param.vel_obs;
    
end

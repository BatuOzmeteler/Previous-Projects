%% Linear ADMM on a Coupled Spring-Mass-Damper System
% Authors: 
% - Mehmet Batu Özmeteler
%% Setup
clear all
clc
%% Parameters
N_agents = 3;
N = 5;
x0 = {[0.27, 0], [0, 0], [0, 0]};
%% Model Initialization
[A_d, B_d, C_d, sizes_z_i, neighbours, H_bar, c_c,...
    A_eq, b_eq, A_ineq, b_ineq] = N_mass_spring_damper(N_agents, N, x0);
%% Linear ADMM 
[x, lambda, iter] = admm_practice(N_agents, N, x0);


%% Problem Structure Creation for ADMM
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function creates the problem structure for a set of quadrotors with 
% 2 or 4 agents of homogeneous dynamics and same number of neighbours.
%%
function [sProb, elem] = create_sProb(param)
%% Setup

import casadi.*

assert(param.N_agents == 2 || param.N_agents == 4)

%% Initialization

% Initialize data
data = init_data(param);

% Initialize elements
[data, elem] = init_elem(param, data);

%% Build the Elements of the Problem

% Build H_bar
elem = build_H_bar(param, data, elem);

% Build f
elem = build_f(param, data, elem);

% Build g
elem = build_g(param, elem);

% Build h
elem = build_h(param, data, elem);

% Build AA
elem = build_AA(param, data, elem);

% Build llbx, uubx
elem = build_llbx_uubx(param, data, elem);

% Build zz0, lam0
elem = build_zz0_lam0(param, data, elem);

%% Build sProb

[z, ~, f, g, h, AA, llbx, uubx, zz0, lam0] = deal_elem(elem);

for i = 1:param.N_agents
%        f{i} = matlabFunction(f{i},'Vars', {z{i}} );
%        g{i} = matlabFunction(g{i},'Vars', {z{i}} );
%        h{i} = matlabFunction(h{i},'Vars', {z{i}} ); 
       
       f{i} = Function(['f' num2str(i)],{z{i}},{f{i}});
       g{i} = Function(['g' num2str(i)],{z{i}},{g{i}});
       h{i} = Function(['h' num2str(i)],{z{i}},{h{i}});
       
end

sProb.locFuns.ffi  = f;
sProb.locFuns.hhi  = h; 
sProb.locFuns.ggi  = g; 

sProb.llbx = llbx;
sProb.uubx = uubx;
sProb.AA = AA;

sProb.zz0 = zz0;
sProb.lam0 = lam0;

end

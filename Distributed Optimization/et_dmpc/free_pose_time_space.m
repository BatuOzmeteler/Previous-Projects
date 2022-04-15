function [AA,bb] = free_pose_time_space(t0, T, h, s_dyn, x0, u, map_bounds, centroid, d, robot_num)

% This function creates a dynamic obstacle with constant velocity model 
% INPUT:  t0             start time
%         T              time horizon
%         h              step size
%         s_dyn          size of the dynamic obstacle
%         x0             initial position of the dynamic obstacle [x,y,z]
%         u              constant velocity of the dynamic obstacle [x,y,z]
%         map_bounds     spatial map limits [x_lb, x_ub; y_lb, y_ub; z_lb, z_ub]
%         centroid       formation center as a state vector [x,x_dot, ...]
%         d              formation distance
%         robot_num      number of robots
%
% OUTPUT: cell array A,b for each agent. A = {A1,A2, ..., A_n}, A = {b1,b2, ..., b_n}

import iris.inflate_region
import iris.drawing.*

N  = (T-t0)/h;
dim = 4;        % environment dimension
n_obs_d = 1;    % number of dynamic obstacles

%environment bounds
lb = [map_bounds(1,1); map_bounds(2,1); map_bounds(3,1); -t0];  %x,y,z,t
ub = [map_bounds(1,2); map_bounds(2,2); map_bounds(3,2); T];    %x,y,z,t

% creating obstacles
obstacles = zeros(dim, 2*dim*(N+1), n_obs_d);
obstacles(:,:,1) = create_dynamic_obstacle(x0',u,h,t0,T,s_dyn);

A_bounds = [];
for j = 1:dim
  row = zeros(1,dim);
  row(j) = -1;
  A_bounds(j,:) = row;
  b_bounds(j) = -lb(j);
end

for j = 1:dim
  row = zeros(1,dim);
  row(j) = 1;
  A_bounds(end+1,:) = row;
  b_bounds(end+1) = ub(j);
end
b_bounds = reshape(b_bounds,[],1);


AA = {};
bb = {};
for i = 1:robot_num
    %s = create_formation(centroid, d, robot_num, i);
    s = centroid{i};
    start = [s(1); s(3); s(5); t0];
    [A,b] = iris.inflate_region(obstacles, A_bounds, b_bounds, start);
    
    AA{i} = A;
    bb{i} = b;
end
% animate for debugging
% if (animate)
%     animate_results(results, false);
% end

end

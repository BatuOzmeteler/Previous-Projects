clear all 
clc

addpath('C:\Program Files\casadi-windows-matlabR2014b-v3.5.1')
load('X'); load('Y'); load('Z'); load('Speed'); load('Torque');
x = X(2,:); y = Y(2,:); z = Z(2,:);

param_T = 0.2;
param_N = 10;
param_Q = 2;
param_R = 0;
param_Fbrake = 0;
param_init_speed = 0;

Speed(2,end) = 0; %negative value fix

param_ref_speed = floor(Speed(2,:) ./ 3.6);
param_traj_len = length(Speed);
param_ref_control = zeros(1,param_traj_len);

for i = 1:param_traj_len
    grade(i) = acos(sqrt((x(i)^2) + (y(i)^2)) / sqrt((x(i)^2) + (y(i)^2) + (z(i)^2))) * 100;
end

set_param('mpc_v2','StartTime','0','StopTime', num2str(param_T*(param_traj_len-1)),'Solver','FixedStepDiscrete','FixedStep',num2str(param_T));


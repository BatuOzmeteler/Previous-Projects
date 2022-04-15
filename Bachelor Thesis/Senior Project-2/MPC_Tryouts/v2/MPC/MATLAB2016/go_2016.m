clear all 
clc

load('X_2016'); load('Y_2016'); load('Z_2016'); load('Speed_2016'); load('Torque_2016');
x = X(2,:); y = Y(2,:); z = Z(2,:);

param_T = 1;
param_N = 15;
param_Q = 3;
param_R = 0.001;
param_Fbrake = 0;
param_init_speed = 0;

Speed(2,end) = 0; %negative value fix

param_ref_speed = Speed(2,:) ./ 3.6;
param_traj_len = length(Speed);
param_ref_control = zeros(1,param_traj_len);

for i = 1:param_traj_len
    grade(i) = acos(sqrt((x(i)^2) + (y(i)^2)) / sqrt((x(i)^2) + (y(i)^2) + (z(i)^2))) * 100;
end

set_param('trial_2016','StartTime','0','StopTime', string(param_T*(param_traj_len-1)),'Solver','FixedStepDiscrete','FixedStep',string(param_T));


clear all 
clc
param_T = 0.2;
param_N = 10;
param_Q = 2;
param_R = 0;
param_Fbrake = 0;
param_Grade = 0;
param_init_speed = 0;

set_param('trial','StartTime','0','StopTime','9.8','Solver','FixedStepDiscrete','FixedStep','0.2');
for i = 1:50
    param_ref_speed(i) = 1*i;
end
param_ref_control = zeros(1,50);
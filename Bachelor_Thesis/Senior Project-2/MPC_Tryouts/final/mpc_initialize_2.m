% clear all
% clc
load('plant_EV.mat');
T = 0.1; %controller sample time
% %% load MAT files
% load('X'); load('Y'); load('Z'); load('Speed'); load('Torque');
% x = X(2,:); y = Y(2,:); z = Z(2,:);
% 
% %% generate grade values over the trajectory
% traj_len = length(x);
% t = 1:1:traj_len;
% for i = 1:traj_len
%     grade_data(i) = acos(sqrt((x(i)^2) + (y(i)^2)) / sqrt((x(i)^2) + (y(i)^2) + (z(i)^2))) * 100;
% end
% grade = [t', grade_data'];
% %% create state space model of the plant
% A = 1.00; B = 0.00010937; C = 3.60; D = 0;
% plant_EV = ss(A, B, C, D, 0.01, 'InputName', 'trq', 'OutputName', 'v', 'StateName', {'v'},...
% 'StateUnit',{'m/s'}, 'InputUnit', {'N.m'}, 'OutputUnit', {'km/h'});
% plant_EV.InputGroup.Manipulated = 1;
% plant_EV.OutputGroup.Measured = 1;
%% create MPC controller object with sample time
mpc_controller_v1 = mpc(plant_C, T);
%% specify prediction horizon
mpc_controller_v1.PredictionHorizon = 50;
%% specify control horizon
mpc_controller_v1.ControlHorizon = 4;
%% specify nominal values for inputs and outputs
mpc_controller_v1.Model.Nominal.U = 0;
mpc_controller_v1.Model.Nominal.Y = 0;
%% specify overall adjustment factor applied to weights
beta = 7.3891;
%% specify weights
mpc_controller_v1.Weights.MV = 0*beta;
mpc_controller_v1.Weights.MVRate = 0.1/beta;
mpc_controller_v1.Weights.OV = 1*beta;
mpc_controller_v1.Weights.ECR = 100000;
%% specify overall adjustment factor applied to estimation model gains
alpha = 1.6596;
%% adjust default output disturbance model gains
setoutdist(mpc_controller_v1, 'model', getoutdist(mpc_controller_v1)*alpha);
%% adjust default measurement noise model gains
mpc_controller_v1.Model.Noise = mpc_controller_v1.Model.Noise/alpha;
% range = generateExplicitRange(mpc_controller_v1);
% 
% range.State.Min(:) = 0;
% range.State.Max(:) = 51;
% range.Reference.Min = 0;
% range.Reference.Max = 51;
% range.ManipulatedVariable.Min = -290;
% range.ManipulatedVariable.Max = 290;
% 
% opt = generateExplicitOptions(mpc_controller_v1);
% opt.polyreduction = 1;
% EMPCobj = generateExplicitMPC(mpc_controller_v1,range,opt);

%set_param('mpc_v1','StartTime','0','StopTime', 'inf', 'Solver','FixedStepDiscrete','FixedStep','auto');

% plot_observer.m

% This file plots the results from the simulation
% of reactor_observer.mdl model

close all;
clc;

% C_A_ss = 1.6329*ones(length(tout),1);
% C_B_ss = 1.1101*ones(length(tout),1);
% T_R_ss = 398.6581*ones(length(tout),1);
% T_J_ss = 397.3736*ones(length(tout),1);
% 
% figure(1);
% subplot(2,1,1)
% plot(tout, d_C_A_obs, 'b', tout, d_C_A, 'r');
% xlabel('Time [s]');
% ylabel('C_A [kmol / m3]');
% legend('Estimated delta_C_A', 'delta_C_A');
% 
% subplot(2,1,2)
% plot(tout, d_C_B_obs, 'b', tout, d_C_B, 'r');
% xlabel('Time [s]');
% ylabel('C_B [kmol / m3]');
% legend('Estimated delta_C_B', 'delta_C_B');
% 
% figure(2)
% subplot(2,1,1)
% plot(tout, d_T_R_obs, 'b', tout, d_T_R, 'r');
% xlabel('Time [s]');
% ylabel('T_R [K]');
% legend('Estimated delta_T_R', 'delta_T_R');
% 
% subplot(2,1,2)
% plot(tout, d_T_J_obs, 'b', tout, d_T_J, 'r');
% xlabel('Time [s]');
% ylabel('T_J [K]');
% legend('Estimated delta_T_J', 'delta_T_J');
% 
% figure(3)
% subplot(4,1,1)
% plot(tout, C_A_obs, 'b', tout, C_A_ss, 'r');
% xlabel('Time [s]');
% ylabel('C_A [kmol / m3]');
% legend('C_A', 'Steady-State');
% 
% subplot(4,1,2)
% plot(tout, C_B_obs, 'b', tout, C_B_ss, 'r');
% xlabel('Time [s]');
% ylabel('C_B [kmol / m3]');
% legend('C_B', 'Steady-State');
% 
% subplot(4,1,3)
% plot(tout, T_R_obs, 'b', tout, T_R_ss, 'r');
% xlabel('Time [s]');
% ylabel('T_R [K]');
% legend('T_R', 'Steady-State');
% 
% subplot(4,1,4)
% plot(tout, T_J_obs, 'b', tout, T_J_ss, 'r');
% xlabel('Time [s]');
% ylabel('T_J [K]');
% legend('T_J', 'Steady-State');

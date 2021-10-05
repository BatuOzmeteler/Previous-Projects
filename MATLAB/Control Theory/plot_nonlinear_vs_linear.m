
% plot_nonlinear_vs_linear.m

% This file plots the results from the simulation
% of reactor_nonlinear_vs_linear_model_simulation.mdl model

close all;
clc;

% C_A_ss = 1.6329*ones(length(tout),1);
% C_B_ss = 1.1101*ones(length(tout),1);
% T_R_ss = 398.6581*ones(length(tout),1);
% T_J_ss = 397.3736*ones(length(tout),1);

% figure(1);
% subplot(2,1,1)
% plot(tout, l_C_A, 'b', tout, nl_C_A, 'r', tout, C_A_ss, 'g');
% xlabel('Time [s]');
% ylabel('C_A [kmol / m3]');
% legend('Linear', 'Non-Linear', 'Steady-State');
% 
% subplot(2,1,2)
% plot(tout, l_C_B, 'b', tout, nl_C_B, 'r', tout, C_B_ss, 'g');
% xlabel('Time [s]');
% ylabel('C_B [kmol / m3]');
% legend('Linear', 'Non-Linear', 'Steady-State');
% 
% figure(2)
% subplot(2,1,1)
% plot(tout, l_T_R, 'b', tout, nl_T_R, 'r', tout, T_R_ss, 'g');
% xlabel('Time [s]');
% ylabel('T_R [K]');
% legend('Linear', 'Non-Linear', 'Steady-State');
% 
% subplot(2,1,2)
% plot(tout, l_T_J, 'b', tout, nl_T_J, 'r',  tout, T_J_ss, 'g');
% xlabel('Time [s]');
% ylabel('T_J [K]');
% legend('Linear', 'Non-Linear', 'Steady-State');

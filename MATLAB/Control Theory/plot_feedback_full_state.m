
% plot_feedback_feedback_full_state.m

% This file plots the results from the simulation
% of reactor_feedback_full_state.mdl model

close all;
clc;

% C_A_ss = 1.6329*ones(length(tout),1);
% C_B_ss = 1.1101*ones(length(tout),1);
% T_R_ss = 398.6581*ones(length(tout),1);
% T_J_ss = 397.3736*ones(length(tout),1);
% 
% Fr_ss = 0.002365*ones(length(tout),1); 
% Qj_ss = 18.5583*ones(length(tout),1);

% figure(1);
% subplot(2,1,1)
% plot(tout, C_A_feedback, 'b', tout, C_A_ss, 'r');
% xlabel('Time [s]');
% ylabel('C_A [kmol / m3]');
% legend('C_A', 'Steady-State');
% 
% subplot(2,1,2)
% plot(tout, C_B_feedback, 'b', tout, C_B_ss, 'r');
% xlabel('Time [s]');
% ylabel('C_B [kmol / m3]');
% legend('C_B', 'Steady-State');
% 
% figure(2)
% subplot(2,1,1)
% plot(tout, T_R_feedback, 'b', tout, T_R_ss, 'r');
% xlabel('Time [s]');
% ylabel('T_R [K]');
% legend('T_R', 'Steady-State');
% 
% subplot(2,1,2)
% plot(tout, T_J_feedback, 'b', tout, T_J_ss, 'r');
% xlabel('Time [s]');
% ylabel('T_J [K]');
% legend('T_J', 'Steady-State');
% 
% figure(3)
% subplot(2,1,1)
% plot(tout, Fr_feedback, 'b', tout, Fr_ss, 'r');
% xlabel('Time [s]');
% ylabel('Fr [m3 / min]');
% legend('Fr', 'Steady-State');
% 
% subplot(2,1,2)
% plot(tout, Qj_feedback, 'b', tout, Qj_ss, 'r');
% xlabel('Time [s]');
% ylabel('Qj [kJ / min]');
% legend('Qj', 'Steady-State');

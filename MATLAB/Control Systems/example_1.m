close all
clear all
clc

G = tf([1 2],[1 4 13]); 
H = 1;
T = feedback(G,H);
rlocus(G*H);
sgrid(0.4,4)

% T_without_K = feedback(G, H);
% T_with_K = feedback(6*G, H);
% subplot(2,1,1);step(T_without_K); grid on;
% subplot(2,1,2); step(T_with_K); grid on;


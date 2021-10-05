clear all
clc

%G(s) = 120(s+2)/(s+3)(s+4)
G = tf([120 240],[1 7 12]);
H = 1;

T = feedback(G,H);
ss = 0.048;
%ss_new = ss*0.5; %we want steady state error to be halved

pc = 0.01;
zc = pc*41/20;
K = 1; %because transient response stays the same
G_lag = K*tf([1 zc],[1 pc]);
Tc = feedback(series(G,G_lag),H);

subplot(2,1,1);step(T)
subplot(2,1,2);step(Tc)
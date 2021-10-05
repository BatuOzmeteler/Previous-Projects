clear all
clc

%G(s) = 1 / (s+1)*(s+2)*(s+3)
G = tf(1,conv([1 3 2],[1 3]));
H = 1;
T = feedback(G,H);

[y,t] = step(T);
ss = 1 - y(end);
OS = 30;
zeta = sqrt((log(OS/100)^2)/((log(OS/100)^2)+(pi*pi)));

G_pi = tf([1 0.001],[1 0]);

rlocus(series(G,G_pi)*H)
sgrid(zeta,[])
K = 13.4;

Tc_1 = feedback(series(G_pi,G),H);
Tc_2 = feedback(K*series(G_pi,G),H);

p = roots([1 6 11 7 0.001]);
%second order assumption is invalid 

% subplot(3,1,1);step(T);
% subplot(3,1,2);step(Tc_1);
% subplot(3,1,3);step(Tc_2);
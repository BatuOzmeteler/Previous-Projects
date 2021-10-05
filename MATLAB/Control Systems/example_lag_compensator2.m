clear all
clc

%G(s) = 1 / (s + 5)(s + 6)(s + 20)
G = tf(1,conv([1 20],[1 11 30]));
H = 1;
rlocus(G*H)
sgrid(0.82,[])
%K = 200 when zeta = 0.82
K = 200;
T = feedback(K*G,H);
%step(T)
ss_err = 1 - 0.25;
ss_new = ss_err/10;

%zc / pc = 40
pc = 0.01;
zc = 40*pc;
G_lag = K*tf([1 zc],[1 pc]);

T_k = feedback(K*G,H);
Tc = feedback(series(G,G_lag),H);

subplot(2,1,1);step(T_k)
subplot(2,1,2);step(Tc)



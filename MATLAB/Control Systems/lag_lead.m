clear all
clc

% G = 1 / (s+1)(s^2 + s + 1)(s+15) %H = 1
% 2 second reduction in settling time and zeta = 0.8

G = tf(1, conv([1 1 1], [1 16 15]));
H = 1;
zeta = 0.8;

%K = 430000;
rlocus(G*H);
sgrid(zeta, []); %K = 430000, wn = 29.2;
wn = 29.2;
% Ts = 4 / (zeta*wn);

T_uncomp = feedback(K*G,H);    %without damping ratio
step(T_uncomp); %settling time = 8.7 seconds;

new_Ts = 8.7 - 2;

wn_new = 4 / (zeta*new_Ts);

p = roots([1 2*zeta*wn_new wn_new^2]);
s_star = p(1);
zc = 1;

theta = -pi -angle(((s_star^2) + s_star + 1)*(s_star + 15));
pc = (imag(s_star) - real(s_star)*tan(theta)) / tan(theta);

G_lead = tf([1 zc], [1 pc]);

rlocus(G*G_lead*H); % gain = 0.241;
sgrid(zeta, wn_new);

K_comp = 0.241;
T_comp = feedback(K_comp*G_lead*G,H);
step(T_comp);
pzmap(G*H)
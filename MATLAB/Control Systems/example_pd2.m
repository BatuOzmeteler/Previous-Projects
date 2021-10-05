clear all
clc

%G(s) = 1/(s+2)(s+3)(s+10), zeta = 0.673 and wn = 2.94
G = tf(1,conv([1 10],[1 5 6]));
H = 1;
T = feedback(G,H);
zeta = 0.673;
wn = 2.94;

% rlocus(G*H);
% sgrid(zeta,[]); %rlocus doesn't pass through these points

p = roots([1 2*zeta*wn wn^2]);
s_star = p(1);

%angle(s_star+zc) - angle((s_star+2)*(s_star+3)*(s_star+10)) = pi
theta = angle((s_star+2)*(s_star+3)*(s_star+10)) + pi;
zc = (imag(s_star) - real(s_star)*tan(theta)) / tan(theta);
G_pd = tf([1 zc],1);

Gc = series(G,G_pd);

rlocus(Gc*H)
sgrid(zeta,wn);

%Tc = feedback(Gc,H);


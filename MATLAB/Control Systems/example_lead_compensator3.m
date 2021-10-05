% G =  1 / (s+2)(s+3)(s+10)
% zeta= 0.673 wn = 2.94
zeta = 0.673;
wn = 2.94;
G = tf(1, conv([1 5 6],[1 10]));
T = feedback(G,H);
rlocus(GH);
sgrid(zeta,wn);
zc = 5;
% lead compansator
p = roots([1 2*zeta*wn wn^2]);
s_star = p(1);
theta = angle(s_star  + 5) - angle((s_star+2)*(s_star+3)*(s_star+10)) - pi;
beta = (imag(s_star) - real(s_star)*tan(theta)) / tan(theta);
Glead = tf([1 zc],[1 beta]);

Gc = series(Glead,G);

rlocus(Gc*H)
sgrid(zeta,wn)

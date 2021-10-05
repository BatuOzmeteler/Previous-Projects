clear all
clc

%G = (s+3) / (s+5)(s+6)(s+7)
%OS = 10, Ts' = Ts/2

G = tf([1 3],conv([1 7],[1 11 30]));
H = 1;
T = feedback(G,H);

OS = 10;
zeta = sqrt((log(OS/100)^2)/((log(OS/100)^2)+(pi*pi)));
% rlocus(G*H)
% sgrid(zeta,[])% K = 97.2 , wn = 12.5
wn = 12.5;
wn_new = wn*2;
Tc1 = feedback(97.2*G,H);% %OS condition holds

p = roots([1 2*zeta*wn_new wn_new^2]);
s_star = p(1);
zc = 5;
%-angle(s+beta) + angle((s+3)) - angle((s+6)(s+7)) = pi
theta = angle(s_star+3) - angle((s_star+6)*(s_star+7)) - pi;
beta = (imag(s_star) - real(s_star)*tan(theta)) / tan(theta);
Glead = tf([1 zc],[1 beta]);

Gc = series(Glead,G);

% rlocus(Gc*H)%K = 424;
% sgrid(zeta,wn_new)
K = 424;
Tc2 = feedback(K*Gc,H);

subplot(3,1,1); step(T);
subplot(3,1,2); step(Tc1);
subplot(3,1,3); step(Tc2);



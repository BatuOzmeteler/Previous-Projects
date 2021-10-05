clear all
clc

%G = 1 / (s+1)(s+4)
%H = 1;

G = zpk([], [-1 -4], 1);
H = 1;
zeta = 0.8;

rlocus(G*H);
sgrid(zeta, []); %K = 5.77 and wn = 3.13
K = 5.77;
wn = 3.13;

% T_uncomp = feedback(K*G,H);
% step(T_uncomp); 
%peak response = 1.68 from graph; 
%peak time = 1.67 from formula Tp = pi/ (wn *  sqrt((1 - (zeta^2))))

wn_new = pi / (1.047 * sqrt((1 - (zeta^2))));

p = roots([1 2*zeta*wn_new wn_new^2]);
s_star = p(1);

zc = 1;
theta = -pi -angle(s_star + 4);
pc = (imag(s_star) - real(s_star)*tan(theta)) / tan(theta);

G_lead = tf([1 zc],[1 pc]);

rlocus(G_lead*G*H);
sgrid(zeta, wn_new); %K_comp = 9.03;

K_comp = 9.03;
T_comp = feedback(K_comp*G_lead*G,H);
% step(T_comp);

G_pi = tf([1 0.01], [1 0]);
T_final = feedback(G_pi*K_comp*G_lead*G,H);
step(T_final);


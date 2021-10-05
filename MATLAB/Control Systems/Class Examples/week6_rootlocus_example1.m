
% take a feedback system with:
%
% G(s) = (s+2) / (s^2 + 4s + 13)
% H(s) = 1

% Plot root-locus using matlab
G = tf([1 2], [1 4 13]);
H = 1;
T = feedback(G, H);

rlocus(G*H)

% b) Does s_star=-4.06+2.18j satisfy the angle
% condition?

s_star=-4.06+1i*2.18
angle( (s_star+2) / (s_star^2 + 4*s_star + 13) * 1) % = pi

% s_star satisfies the angle condition. Therefore, s_star should
% be on root-locus

% c) Find K such that s_star becomes a pole of the system

% using the magnitude condition
K = 1/abs( (s_star+2) / (s_star^2 + 4*s_star + 13) * 1 )


% d) Set K=4.12 and show that you get a pole at -4.06+1i*2.18

T2 = feedback(4.12*G, H)

p = roots([1 8.12 21.24])


% e) Can we find a K such that T(s) will have a pole at
% s_star = -4+j. If yes, find the K. If no, explain.

% If root-locus passes through -4+j, then it is possible to
% find a K such that -4+j becomes a pole. Otherwise, it is not
% possible to put a pole at -4+j.

% If -4+j is on root-locus, then it should satisfy the 
% angle condition

s_star=-4+1i
angle( (s_star+2) / (s_star^2 + 4*s_star + 13) * 1) % != pi

% s_star is NOT on root-locus because it does not satisfy the
% angle condition.


% f) Find K such that Ts of the system is reduced by 1/2.


T_without_K = feedback(G, H);
T_with_K = feedback(6*G, H);

subplot(2,1,1);step(T_without_K); grid on;
subplot(2,1,2); step(T_with_K); grid on;


% g) Find K such that zeta=0.4 and wn=4
%
% Root-locus needs to pass through zeta=0.4 and wn=4 before
% we can pick a K that satisfies the constraints.

rlocus(G*H)
sgrid

% because root-locus does not pass through zeta=0.4 and wn=4,
% we cannot find a K that will give us zeta=0.4 and wn=4.

% If zeta=0.4 and wn=4, then the denominator of a 2nd order system
% is s^2 + 2*(0.4)*4 + 4^2

p = roots([1 2*0.4*4 4^2]);

% root-locus does not pass through those poles (p(1) and p(2)).
% Therefore, I cannot find a K that will make system have poles
% at p(1) and p(2).





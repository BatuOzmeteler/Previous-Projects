
% G = 1 / (s+1)(s+2)(s+10)
% H = 1

% a
G = tf(1, conv([1 10], [1 3 2]));
H = 1;

T = feedback(G,H);

step(T)


% b
G_pi = tf([1 0.001], [1 0]);

Tc = feedback( series(G, G_pi), H);
step(Tc)


% c
rlocus( series(G, G_pi)*H )
sgrid(0.59, [])

% When K=32, zeta=0.59 line cross the root-locus

Tc2 = feedback( series(G, 32*G_pi), H);

% z = -0.001
% poles:
% -10.4048 + 0.0000i
%  -1.2973 + 1.8201i
%  -1.2973 - 1.8201i
%  -0.0006 + 0.0000i

% 2nd order assumption is valid:
%
% 4th pole cancels out with zero
% 1st pole is far away from the dominant poles.




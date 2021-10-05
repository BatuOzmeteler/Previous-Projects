

G = tf(25, [1 5 0]);
H = 1;

T = feedback(G,H)

wn = 5;
zeta = 0.5

OS = exp(-zeta*pi/sqrt(1-zeta^2))*100
Tp = pi/(wn*sqrt(1-zeta^2))
Ts = 4 / (zeta*wn)


step(T)

% homework: find the same parameters AND the rise time
% experimentally; i.e., not using the formula but doing
% it numerically like we did in the class before.

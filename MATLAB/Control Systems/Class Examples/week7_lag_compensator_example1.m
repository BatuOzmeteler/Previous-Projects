

% G(s) = 120(s+2) / (s+3)(s+4)
% H(s) = 1

close all

G = tf([120 240], [1 7 12]);
H = 1;

T = feedback(G, H);

step(T, [0:0.001:1])

ess = 1 - 0.956;

Glag = tf([1 0.01*41/20], [1 0.01]);

Tc = feedback( series(Glag, G), H);

hold on; step(Tc, [0:0.001:1])

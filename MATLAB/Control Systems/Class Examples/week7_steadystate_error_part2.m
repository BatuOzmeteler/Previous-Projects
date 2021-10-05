
% Feedback system with G(s) = 120(s+2) / (s+3)(s+4), H(s) = 1

G = tf([120 240], [1 7 12]);
H = 1;

T = feedback(G, H);

step(T)

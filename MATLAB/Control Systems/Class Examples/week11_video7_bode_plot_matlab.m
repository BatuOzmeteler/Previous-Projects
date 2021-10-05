

% G(s) = (s+1) / (s+2)

G = tf([1 1], [1 2]);

bode(G)
grid on


G = tf([1 1 1], [1 0 2 5]);

bode(G)
grid on
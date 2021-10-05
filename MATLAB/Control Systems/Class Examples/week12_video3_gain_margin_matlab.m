
G = zpk([], [-2 -3 -4],1)

nyquist(G)

% Nyquist diagram crossing the x-axis at 0.0046
% If the gain (K) is larger than 1/0.0046, then there will rotations around
% x=-1 and the system will become unstable
gain_margin=20*log10(1/0.0046)


bode(G)


% Example-2

H = 1;
G = tf(1, conv([1 2 2], [1 2]) );

% a) Find K such that T(s) is stable from the Nyquist plot and root-locus
% b) Compute gain margin (Gm) from the Bode plot.

nyquist(G*H)
% x = 0.0491, nyquist diagram crosses the x-axis

K = 1/0.0491 % 20log10(K) = 26.1784
nyquist(K*G*H)


% unstable
T = feedback( 30*G, H);
pzmap(T)

% unstable
T = feedback( K*G, H);
pzmap(T)

% stable
T = feedback( 10*G, H);
pzmap(T)


rlocus(G*H)


bode(G*H) % Gm = -26.2




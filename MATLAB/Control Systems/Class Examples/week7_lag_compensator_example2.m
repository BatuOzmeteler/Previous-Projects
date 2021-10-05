

G = tf(1, conv([1 1], [1 12 20]));
H = 1;

rlocus(G*H)

sgrid(0.174, [])

% K = 160

T = feedback( 160*G, H )


% You can ignore the third pole because it is far away
% from the dominant poles. Thus, you can make a 2nd order
% assumption.

step(T)

ess = 1 - 0.888; % 0.1120


Gc = tf( [1 0.089], [1 0.008])
Tc = feedback( 160*G*Gc, H )

step(Tc)

ess_c = 1 - 0.983; % 0.0170
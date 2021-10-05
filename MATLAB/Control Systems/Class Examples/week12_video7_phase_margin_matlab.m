
G = tf(6, conv([1 2 2], [1 2]))

nyquist(G)


bode(G)

% angle where the magnitude is 1 is -114 (alpha = 180 - 114 = 66)

% phase_margin = 180 - 66 = 114 from the Bode plot.


% Example-2

% G(s) = K/(s+2)(s+4)(s+5)
% H(s) = 1

% a) Find range of K for stability using the Nyquist approach

G = zpk([], [-2 -4 -5], 1)
H = 1

nyquist(G*H)

% nyquist diagram is crossing the x-axis at x=-0.0026

Gm = 20*log10(1/0.0026)

% b) Find range of K for stability using the bode plot
bode(G*H)

% when the w=6.19, phase is 180, Nyquist diagram is crossing the x-axis.

% solve the same problem using root-locus
rlocus(G*H)
% K < 377 for stability

% c) Use the Nyquist criterion to show that the system is stable when K = 1
nyquist(G*H)

% N = P - Z
% N = 0 from the Nyquist plot
% P = 0 (poles of G on the right handside)
% Z = P - N = 0.
%
% Z is the number of zeroes of 1+G(s)H(s).
% Remember that is also equal to the number of poles of T(s)
% System is stable!


% d) Set K = 500 and show that the system is unstable using the Nyquist
% criterion

nyquist(500*G*H)

% N = P - Z
% N = -2 from the Nyquist plot because rotations are clockwise
% P = 0 (poles of G on the right handside)
% Z = P - N = 2.
%
% Z is the number of zeroes of 1+G(s)H(s).
% Remember that is also equal to the number of poles of T(s) on the
% right-handside
% System is unstable!

T = feedback(500*G, H)
pzmap(T)


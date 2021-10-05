

% Example-1
G = zpk([], [2 3 4],1)

nyquist(G)

% N = P - Z
% N = 0 from the Nyquist plot
% P = 3 (poles of G on the right handside)
% Z = P - N = 3.
%
% Z is the number of zeroes of 1+G(s)H(s).
% Remember that is also equal to the number of poles of T(s)
% System is unstable!

T = feedback(G, 1)
pzmap(T)



% Example-2
G = zpk([], [-2 -3 -4],1)

nyquist(G)

% N = P - Z
% N = 0 from the Nyquist plot
% P = 0 (poles of G on the right handside)
% Z = P - N = 0.
%
% Z is the number of zeroes of 1+G(s)H(s).
% Remember that is also equal to the number of poles of T(s)
% System is stable!

T = feedback(G, 1)
pzmap(T)


% Example-3
G = zpk([-3 -5], [3 4],1)
H = 1


nyquist(G*H)

% N = P - Z
% N = 2 from the Nyquist plot
% P = 2 (poles of G on the right handside)
% Z = P - N = 0.
%
% Z is the number of zeroes of 1+G(s)H(s).
% Remember that is also equal to the number of poles of T(s)
% System is stable!
T = feedback(G, 1)
pzmap(T)


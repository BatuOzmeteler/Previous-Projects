

G = zpk([-3 -5], [3 4],1)

% P : 2
% Z : 0
%
% N = P -Z

nyquist(G); % N = 2 from the Nyquist plot.

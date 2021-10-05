
% In a feedback system, we have 1+KG(s)H(s)
%
% We want to determine the range of K such that the feedback system is
% stable


G = zpk([-3 -5], [3 4],1)
H = 1

nyquist(G*H)

% nyquist plot is crossing the x-axis at x=-1.14. If we set K such that it
% crosses the x-axis at x=-1, then P = 0 in this case. That happens when
% K=1/1.14.

K = 1/1.14
nyquist(K*G*H); % N = 0; Z = P - N = 2

% K = 1/1.14 is critical. If we want a stable system K > 1/1.14.
rlocus(G*H); % K > 0.873 from root locus

% subplot(1,3,1); nyquist(G*H);
% subplot(1,3,2); nyquist(0.5*G*H);
% subplot(1,3,3); nyquist(2*G*H);
% 

% N = P - Z
% N = 2 from the Nyquist plot
% P = 2 (poles of G on the right handside)
% Z = P - N = 0.
%
% Z is the number of zeroes of 1+G(s)H(s) on the right handside of splane.
% Remember that is also equal to the number of poles of T(s)
% System is stable!
T = feedback(G, 1)
pzmap(T)



%% Example-2

% G(s) = 1 / s(s+3)(s+5)
% H(s)
%
% Is the system stable? Check using the Nyquist criterion.
% Find the range of K that makes the system stable


G = zpk([], [0 -3 -5],1)
H = 1

nyquist(G*H)

% N = P - Z
% N = 0 from the Nyquist plot
% P = 0 (poles of G on the right handside)
% Z = P - N = 0 % T(s) is stable

T = feedback(G, 1)
pzmap(T)

% Find range of K that will make this system stable from Nyquist
%
% nyquist plot is crossing the x-axis at 0.00817
%
% If we expand the Nyquist plot by multiplying it with K=1/0.00817
% Then the Nyquist plot will pass through the critical point of x=-1

K=1/0.00817;
nyquist(K*G*H);

% K < 1/0.00817 for the system to be stable.

% Below is an example where K=200>1/0.00817 and the system becomes
% unstable.
K=200;
nyquist(K*G*H);
% N = P - Z
% N = -2 from the Nyquist plot
% P = 0 (poles of G on the right handside)
% Z = P - N = 2 % T(s) has two poles on the right hand plane. system is
% unstable

T = feedback(200*G, 1)
pzmap(T)

rlocus(G*H)


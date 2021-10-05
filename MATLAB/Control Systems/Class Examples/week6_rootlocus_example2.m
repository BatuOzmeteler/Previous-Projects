
% Take a feedback system with:
%
% G(s) = (s+15)/(s(s+1)(s+10))
% H(s) = 1
%
% Find a K such that %OS is 1.52

% %OS = exp( -(zeta*pi/sqrt(1-zeta^2) ) ) * 100

% Homework: Find zeta given %OS
%
% The answer in this example is zeta=0.8

zeta=0.8
exp( -(zeta*pi/sqrt(1-zeta^2) ) ) * 100


G = tf([1 15], [1 11 10 0]);
H = 1;

rlocus(G*H)

% root-locus is ALWAYS symmetric around the x-axis
% number of branches = number of poles
% each branch is shown with a different color in Matlab
% each branch starts from a pole and ends either at a zero or at infinity.

sgrid(0.8, [])


% when K = 0.25 root-locus is passing through zeta=0.8 line

T = feedback(0.25*G, H)

% T is NOT a 2nd order system. Can we make a second order
% approximation for T. Poles and zeros that we want to ignore
% should be far away from the dominant poles. Far away was
% defined as the real parts should be at least 3 times of the
% real parts of the dominant poles.

p = roots([1 11 10.25 3.75])
z = roots([0.25 3.75])

% poles:
% p1 = -10.0138 + 0.0000i
% p2 = -0.4931 + 0.3624i --> dominant pole
% p3 =  -0.4931 - 0.3624i --> dominant pole

% real(p1) = -10.0138 is far away from the real(dominant poles)=0.49
% Therefore, we can ignore p1.

% The zero of the syste is z=-15 which is far away from 
% real(dominant poles)=0.49. Therefore, we can ignore the zero

% Because we can ignore additional poles and zeroes. We can
% assume that the 2nd order assumption is valid.

step(T)


% step response of T has a steady-state value of 1. If %OS=1.52,
% then we expect the step response to make a peak at 1.0152.
% That's what we observe in the plot when we plot step response.


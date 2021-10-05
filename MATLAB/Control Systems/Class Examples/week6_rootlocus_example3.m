
% Take a feedback system with G(s)= 1 / ((s+2)(s+4)(s+6)) and the
% system is a unity feedback system (H(s)=1)

% a) Assuming the 2nd order assumption is true, find K that yields
% 10% OS.

%OS = exp( -(zeta*pi/sqrt(1-zeta^2) ) ) * 100 = 10

% -(zeta*pi/sqrt(1-zeta^2) ) = log(0.1)

% zeta^2 * pi^2 = log(0.1)^2 * (1 - zeta^2)

% zeta^2 ( pi^2 + log(0.1)^2 ) = log(0.1)^2

% zeta^2 = log(0.1)^2 / ( pi^2 + log(0.1)^2 )

% zeta = 0.59

% To find K, that gives us zeta=0.59
G = tf(1, conv([1 2], [1 10 24]));
H = 1;

rlocus(G*H)
sgrid(0.59, [])

% From root-locus, K=44.3 so that we get zeta = 0.59

% b) Is the 2nd order assumption valid?
T = feedback(44.3*G, H)

pzmap(T)

p = roots([1 12 44 92.3])

% We can ignore the 3rd pole of the system and the 2nd order
% approximation is valid.



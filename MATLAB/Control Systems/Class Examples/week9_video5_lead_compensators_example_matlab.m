
G = tf(1, [1 10 24 0]);
H = 1;

% need a lead compensator
%
% zeta = 0.35
% Ts' = 0.5*Ts
% wn' = 2*wn


rlocus(G*H)
sgrid(0.35, [])

% If we set K = 65, zeta=0.35 and wn = 2.86

wn = 2*2.86;
zeta = 0.35;

r = roots([1 2*zeta*wn wn^2]);
s_star = r(1);


rlocus(G*H)
sgrid(0.35, wn)


theta = -pi - angle(s_star * (s_star + 6));

beta = (imag(s_star) - tan(theta)*real(s_star)) / tan(theta);

Glead = tf([1 4], [1 beta]);

rlocus(Glead*G*H)
sgrid(0.35, wn)


Glead = tf(663*[1 4], [1 beta]);



T = feedback(65*G, H);
Tc = feedback(Glead*G, H);


subplot(2,1,1); step(T);
subplot(2,1,2); step(Tc);


% Is the 2nd order assumption valid?
pzmap(T); % valid
pzmap(Tc); % valid






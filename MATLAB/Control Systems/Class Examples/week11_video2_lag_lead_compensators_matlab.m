

G = zpk([-8], [-3 -6 -10], 1);

zeta = 0.456;

rlocus(G)
sgrid(zeta, [])

wn = 11.9; % comes from root-locus

wn = 11.9*3/2;


r = roots([1 2*zeta*wn wn^2]);

s_star = r(1)


rlocus(G)
sgrid(zeta, wn)


theta = -pi + angle( (s_star+8) / ((s_star+6)*(s_star+10)) );

b = imag(s_star) / tan(theta) - real(s_star)


Glead = tf([1 3], [1 b]);

rlocus(Glead*G);
sgrid(zeta, wn)

K = 258;
Glead = K*Glead;


T = feedback(Glead*G, 1)
pzmap(T) % 2nd order assumption is valid


% simulate the system with and without the compensator at zeta=0.456
T = feedback(118*G, 1)
subplot(2,1,1); step(T)


T = feedback(Glead*G, 1)
subplot(2,1,2); step(T)



Gpi = tf([1 0.1], [1 0]);


T = feedback(G, 1);
step(T) 

% ss = 0.0424
steady_state_error = 1 - 0.0424


T = feedback(Gpi*Glead*G, 1);
step(T) 






G = tf(1, conv([1 1], [1 7 10]));
H = 1;


rlocus(G*H)
sgrid(0.5, 5)


% reminder about stability
%T = feedback(78.6*G, H);
%impulse(T)


% how to find s_star
zeta=0.5;
wn=5;
p = roots([1 2*zeta*wn wn^2]);

s_star=p(1);


theta = angle( (s_star+1) * (s_star+2) * (s_star+5) )
zc = (4.33 + 2.5*tan(pi+theta)) / tan(pi+theta)


Gpd = tf([1 zc], 1);

Gc = series(23.3*Gpd, G);

rlocus(Gc*H)
sgrid(0.5, 5)

T = feedback(G, H);
Tc = feedback(Gc, H);


% verify that the 2nd order assumption is valid
pzmap(T) % valid
pzmap(Tc) % not valid.





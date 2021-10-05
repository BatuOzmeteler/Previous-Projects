
G = tf([1 8], conv([1 3], [1 16 60]));
H = 1;


rlocus(G*H);
sgrid(0.456, []);

% K = 125
% wn = 12

zeta = 0.456;
wn = 3*12/2;

r = roots([1 2*zeta*wn wn^2]);

s_star = r(1);

rlocus(G*H);
sgrid(zeta, wn);


theta = pi - angle( (s_star+8) / ( (s_star+3)*(s_star+6)*(s_star+10)) );

zc = ( imag(s_star) - tan(theta)*real(s_star) ) / tan(theta)


Gpd = tf([1 zc], 1);

rlocus(G*Gpd*H);
sgrid(zeta, wn);





G = tf([1 8], conv([1 3], [1 16 60]));
H = 1;

Tu = feedback(125*G, H);

zc = 55.45;
Kpd = 5.7;

Gpd = tf(Kpd*[1 zc], 1);


G2 = Gpd * G;

T = feedback(G2, H);
step(T)


Gpi = tf([1 0.2], [1 0]);

Gpid = Gpi * Gpd * G;

rlocus(Gpid*H);
zeta = 0.456;
wn = 3*12/2;
sgrid(zeta, wn);

Kpi = 0.952;

Gpid2 = Kpi * Gpid;

Tc = feedback(Gpid2, H);
step(Tc, 10);

% 2nd order assumption
% valid for the uncompensated system
% valid for the compensated system

subplot(2,1,1); step(Tu);
subplot(2,1,2); step(Tc);




G = tf(1, [1 16 60 0]);
H = 1;

zeta = 0.456;

rlocus(G*H)
sgrid(zeta, []);

wn=3.9
K=188;

Tu = feedback(188*G, H);
step(Tu);

% Ts of the uncompensated system is about 3 seconds.
% We want Ts the compensated system is about 1.5 seconds.
%
% to get that
wn_c = 2*wn;

rlocus(G*H)
sgrid(zeta, wn_c);

r = roots([1 2*zeta*wn_c wn_c^2]);
s_star = r(1);

theta = pi + angle(s_star*(s_star+6)*(s_star+10));

zc = (imag(s_star) - tan(theta) * real(s_star)) / tan(theta)


Gpd = tf(63.6*[1 zc], 1);

rlocus(Gpd*G*H)
sgrid(zeta, wn_c);


Tc = feedback(G*Gpd, H);
step(Tc);


Glag = tf([1 0.00186], [1 0.1]);

Tc = feedback(G*Gpd, H);
[y,t] = step(Tc*tf(1, [1 0]));

plot(t,y)

steady_state_error = abs(y(end) - t(end))

% remember the ramp function is r(t)=t
%
% r(129.5235) = 129.5235
%
% steady_state_error = 129.5235 - 129.4118=0.1117



Tc = feedback(G*Gpd*Glag, H);
[y,t] = step(Tc*tf(1, [1 0]));

plot(t,y)


steady_state_error = abs(y(end) - t(end))




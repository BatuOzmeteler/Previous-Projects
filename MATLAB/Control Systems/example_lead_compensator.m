clear all 
close all
clc

%% G(s) = 1 / s * (s + 3) * (s + 7)
%% Design a lead compensator where %OS = 20 and Ts' = Ts/3
OS = 20;
G = tf(1, [1 10 21 0]);
H = 1;
zeta = sqrt((log(OS/100)^2)/((log(OS/100)^2)+(pi*pi)));

T = feedback(G,H);
%rlocus(G*H);
%sgrid(zeta,[]); %K = 39.4 and wn = 2.22 when zeta = 0.453 for %OS = 20
K = 39.4;
%% So, we need to find what poles we need to include in our system if these
%% conditions are met. First, check if the current root locus passes through
%% these coordinates.
wn = 3*2.22;
%rlocus(G*H);
%sgrid(zeta, wn);

%% Since it doesn't, we need to find which poles to include in our system.
r = roots([1 2*zeta*wn wn^2]);
s_star = r(1);

%% We need to design a lead compensator Glead(s) = K * ((s + zc)/(s + beta))
%% Pick zc such that it eliminates the most dominant pole. zc = 3
%% Glead(s) = K * ((s + 3)/(s + beta)), Gc(s) = Glead(s) * G(s)
%% S* should satisfy the angle condition angle(Gc(s*)) = pi

theta = -pi -angle((s_star+7)*s_star);
beta = (imag(s_star) - tan(theta)*real(s_star)) / tan(theta); 
Glead = tf([1 3],[1 beta]);
% rlocus(Glead*G*H);
% sgrid(zeta, wn);
K_c = 2440;

T_gain = feedback(K*G, H);
Tc = feedback(K_c*Glead*G, H);

subplot(3,1,1); step(T);
subplot(3,1,2); step(T_gain);
subplot(3,1,3); step(Tc);
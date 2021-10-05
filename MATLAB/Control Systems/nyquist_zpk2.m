clear all
clc

%G = K / (s+1)^2
%H = 1

G = tf(1 , [1 2 1]);
H = 1;

%a) Find the gain margin from the Bode Plot
bode(G*H, {0.1 , 10000}); %dB = -94.5 

%b) Find the gain margin from the Nyquist Diagram
nyquist(G*H); 
K = 1 / (4e-24);
dB = 20*log10(K);
%nyquist diagram passes intersects with x-axis at x = 4e-24
%which is very close to 0. Therefore, the calculation from the Nyquist plot
%and bode plot do not match.

%c) Find the phase margin from the Bode Plot if K = 9
bode(9*G*H); %phase = -141 when magnitude = 0 dB and w = 2.83
pM = 180 - (-141 + 180);
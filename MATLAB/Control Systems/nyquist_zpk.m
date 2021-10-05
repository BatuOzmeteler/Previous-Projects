clear all
clc

%G = 1 / (s+2)(s+6)(s+11) STABLE
%H = 1

G1 = zpk([],[-2 -6 -11],1);
G2 = tf(1,conv([1 11], [1 8 12]));
H = 1;

nyquist(G1*H); %doesn't rotate around -1
%nyquist diagram passes intersects with x-axis at x = -0.000563

%Nyquist Criterion N = P - Z
%N = 0 from nyquist

%P = 0 from G not having any poles on the right-hand side 
%G*H poles = 1 + G*H poles => 1st property

%Z = P - N
%1 + G*H zeroes = T poles => 2nd property
%Z = 0, therefore the system is stable

%The range of K such that the system within 
%stays stable is K < 1/0.000563
K = 1/0.000563; 

%To find gain margin, we can also check root locus.
rlocus(G1*H); % K = 1770 which is the same result obtained nyquist plot.

%Lastly, we can find the gain margin from the bode plot.
bode(G1*H); %-65db can be seen at w = 10
grid on

gM = 20*log10(K);

%To find phase margin from Bode plot, we first check 
%the frequency in the magnitude plot where dB = 0
bode(500*G2*H);
grid on

%T = feedback(G1,H);
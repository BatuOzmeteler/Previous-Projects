clear all
clc
%G(s) = 1 / s(s+4)(s+6) 
G = tf(1,conv([1 0],[1 10 24]));
H = 1;

OS = 16;
zeta = sqrt((log(OS/100)^2)/((log(OS/100)^2)+(pi*pi)));
%rlocus(G*H);
sgrid(zeta,[])%K = 43.3 and wn = 2.39 when zeta = 0.504
K = 43.3;
wn = 2.39;
wn_new = 3*wn;

p = roots([1 2*zeta*wn_new wn_new^2]); %to find poles of 
%the new system where the Ts' = Ts/3 and OS% = 16
s_star = p(1);

theta = angle((s_star+4)*(s_star+6)*(s_star)) + pi;
%theta = arctan(im(z)/re(z));
%im(z)/re(z) = tan(theta);
%s_star + zc ==> Im(s_star) / Re(s_star) + zc = tan(theta)
%Im(s_star) = tan(theta)*zc + Re(s_star)*tan(theta)
%zc = (Im(s_star) - Re(s_star)*tan(theta)) / tan(theta)
zc = (imag(s_star) - real(s_star)*tan(theta)) / tan(theta);
G_pd = tf(K*[1 zc],1);

rlocus(G*G_pd*H);
sgrid(zeta,wn_new);
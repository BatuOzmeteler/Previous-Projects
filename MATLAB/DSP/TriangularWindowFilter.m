M = 64; %% ucgen filterin genisligi
w = zeros(M+1, 1);
for n = 0:M
    if n <= M/2
        w(n+1) = 2*n/M;
    else
        w(n+1) = 2 - 2*n/M;
    end
end
plot(w)
ss = 2*pi/512; %% sample size
% because window function is denoted with w
% and frequency is also denoted with w, we use
% f here for frequency
f = [-pi:ss:pi-ss];
H = zeros(512, 1);
for k = 1:512
    for n = 0:64
        H(k) = H(k) + w(n+1)*exp(-1i*f(k)*n);
    end
end
Htri = H;
plot(f, abs(Htri));  %%% TRIANGULAR WINDOW
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%TRIANGULAR FILTER bulma%%%%%%%%%
H = zeros(512, 1);
h_lp = zeros(65,1);
wc = pi/4; %%cutoff frequency
for n = -32:32 %%% triangular M -yarisi:yarisi
    h_lp(n+33) = sin(wc*n) / (pi*n);
end
h_lp(33) = 1; %% sinc(0) = 1 oldugundan bu fix bu
plot(h_lp)

h_tri = h_lp .* w;
plot(h_tri)

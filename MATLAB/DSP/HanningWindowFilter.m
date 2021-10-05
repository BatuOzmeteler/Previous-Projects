% Hanning window
M = 64; %% SIZE OF WIDOWN
w = zeros(M+1, 1);
for n = 0:M
    w(n+1) = 0.5 - 0.5*cos(2*pi*n/M);
end
ss = 2*pi/512;
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
Hhan = H;
plot(abs(Hhan))

H = zeros(512, 1);
h_lp = zeros(65,1);
wc = pi/2; %%cutoff frequency
for n = -32:32 %%% triangular M -yarisi:yarisi
    h_lp(n+33) = sin(wc*n) / (pi*n);
end
h_lp(33) = 1; %% sinc(0) = 1 oldugundan bu fix bu
plot(h_lp)

h_HANNING = h_lp .* w;
plot(h_HANNING)



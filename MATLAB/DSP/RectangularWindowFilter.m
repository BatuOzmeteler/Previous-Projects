ss = 2*pi/512;
% because window function is denoted with w
% and frequency is also denoted with w, we use
% f here for frequency
f = [-pi:ss:pi-ss];
H = zeros(512, 1);
for k = 1:512
    for n = 0:64
        H(k) = H(k) + 1*exp(-1i*f(k)*n);
    end
end
Hrec = H;
plot(f, abs(Hrec)); %%% rectangular WINDOW
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ss = 2*pi/512;
w = [0:ss:2*pi-ss];
% length(h) = 51
H = zeros(512, 1);
h_lp = zeros(6001,1);
wc = pi/4; %%cutoff frequency
for n = -3000:3000
    h_lp(n+3001) = sin(wc*n) / (pi*n);
end
h_lp(3001) = 1;

for k = 0:511 %% SAMPLE SIZE
    for n = -3000:3000 %% BOYUTUN YARISI
        H(k+1) = H(k+1) + h_lp(n+3001)*exp(-1i*w(k+1)*n);
    end
end
plot(abs(H))
%%% RECTANGULAR FILTER
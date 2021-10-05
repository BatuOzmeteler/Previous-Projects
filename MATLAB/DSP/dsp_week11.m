
% triangular window
M = 64;
w = zeros(M+1, 1);
for n = 0:M

    if n <= M/2
        w(n+1) = 2*n/M;
    else
        w(n+1) = 2 - 2*n/M;
    end
end

%w = triang(M); % do not use in the exam
%plot([0:64], w);

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

Htri = H;

% Hanning window
M = 64;
w = zeros(M+1, 1);
for n = 0:M
    w(n+1) = 0.5 - 0.5*cos(2*pi*n/M);
end

plot(w)

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

% frequency response of the rectangular window
f = [-pi:ss:pi-ss];

H = zeros(512, 1);

for k = 1:512
    
    for n = 0:64
        H(k) = H(k) + 1*exp(-1i*f(k)*n);
    end
end

Hrec = H;


% plot(f, 2*abs(Hhan));
% hold on; plot(f, abs(Hrec), 'r');
% legend('Hanning', 'rectangular')

plot(f, abs(Hhan));
hold on; plot(f, abs(Htri), 'r');
legend('Hanning', 'Triangular')




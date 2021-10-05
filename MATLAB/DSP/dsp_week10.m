
ss = 2*pi/512;

w = [0:ss:2*pi-ss];

% length(h) = 51

H = zeros(512, 1);


h_lp = zeros(6001,1);
wc = pi/4;
for n = -3000:3000
    h_lp(n+3001) = sin(wc*n) / (pi*n);
end
h_lp(3001) = 1;

for k = 0:511
    for n = -3000:3000
        H(k+1) = H(k+1) + h_lp(n+3001)*exp(-1i*w(k+1)*n);
    end
end
% % 
% % H = zeros(512, 1);
% % 
% % 
% % h_lp = zeros(51,1);
% % wc = pi/4;
% % for n = -25:25
% %     h_lp(n+26) = sin(wc*n) / (pi*n);
% % end
% % h_lp(26) = 1;
% % 
% % for k = 0:511
% %     for n = -25:25
% %         H(k+1) = H(k+1) + h_lp(n+26)*exp(-1i*w(k+1)*n);
% %     end
% % end
% % 
% % 
% % plot(w, abs(H))

H = zeros(512, 1);


h_lp = zeros(201,1);
wc = pi/4;
for n = -100:100
    h_lp(n+101) = sin(wc*n) / (pi*n);
end
h_lp(101) = 1;

for k = 0:511
    for n = -100:100
        H(k+1) = H(k+1) + h_lp(n+101)*exp(-1i*w(k+1)*n);
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

fp = fopen('TIMIT_ver110train_0001.raw', 'r');
x = fread(fp, 'short');
fclose(fp);

y = conv(h_lp, x);

specgram(x)
specgram(y)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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

f = [-pi:ss:pi-ss];

H = zeros(512, 1);

for k = 1:512
    
    for n = 0:64
        H(k) = H(k) + 1*exp(-1i*f(k)*n);
    end
end

Hrec = H;


plot(f, abs(Htri));
hold on; plot(f, abs(Hrec), 'r');
legend('triangular', 'rectangular')




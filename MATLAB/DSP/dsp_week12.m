% 
% n = [-50:50];
% 
% wc = 0.2*pi;
% 
% % sinc function
% h = sin(wc*n) ./ (pi*n);
% h(51) = 1;
% 
% plot(n, h)
% 
% w = hanning(101); % in the exam, don't use the hanning function
% 
% h_han = h.*w';
% 
% plot( abs( fft(h_han) ) );
% 
% n = [0:16000];
% x = sin(0.1*pi*n) + sin(0.3*pi*n);
% 
% 
% y = conv(x, h); % do not use the conv command in the exam.
% 
% subplot(2,1,1); plot( abs(fft(x)) );
% subplot(2,1,2); plot( abs(fft(y)) );


% implementation of the Kaiser window
M = 37;
w = zeros(37, 1);
alpha = 18.5;
beta = 5.6533;
for n = 0:37

    x = beta*sqrt(1 - ((n-alpha)/alpha)^2);
    
    w(n+1) = besseli(0, x) / besseli(0, beta); 
end

plot(w)

dw = 0.5*pi - 0.1*pi;

delta = min(0.01, 0.1);
A = -20*log10(delta);

beta = 0.5842*(A-21)^0.4 + 0.07866*(A-21);

M = ceil((A-8) / (2.285*dw))

alpha = M/2;


w = zeros(M, 1);
for n = 0:M

    x = beta*sqrt(1 - ((n-alpha)/alpha)^2);
    
    w(n+1) = besseli(0, x) / besseli(0, beta); 
end

plot(w)

n = [-6:6];

wc = 0.3*pi;

% sinc function
h = sin(wc*n) ./ (pi*n);
h(7) = 1;

hlp = h .* w';

n = [0:16000];

x = sin(0.05*pi*n) + sin(0.6*pi*n);
y = conv(hlp, x);

subplot(2,1,1); plot( abs( fft(x) ) );
subplot(2,1,2); plot( abs( fft(y) ) );





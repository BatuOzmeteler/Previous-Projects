
x = [1 0 1 -1];
%fft(x); % do not use this in the exam.

Xdft = zeros(4,1);
N = 4; %% length of x
for k = 0:3
    for n = 0:3
        Xdft(k+1) = Xdft(k+1) + x(n+1)*exp(-1i*2*pi*k*n/N); 
    end
end

subplot(2,1,1); stem([0 pi/2 pi 3*pi/2], abs(Xdft));
subplot(2,1,2); stem([0 pi/2 pi 3*pi/2], angle(Xdft));

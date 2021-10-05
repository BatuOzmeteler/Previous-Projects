% % 
% % x = [1 0 1 -1];
% % %fft(x); % do not use this in the exam.
% % 
% % Xdft = zeros(4,1);
% % N = 4;
% % for k = 0:3
% %     for n = 0:3
% %         Xdft(k+1) = Xdft(k+1) + x(n+1)*exp(-1i*2*pi*k*n/N); 
% %     end
% % end
% % 
% % subplot(2,1,1); stem([0 pi/2 pi 3*pi/2], abs(Xdft));
% % subplot(2,1,2); stem([0 pi/2 pi 3*pi/2], angle(Xdft));
% 
% % take the DTFT and then sample it
% 
% step_size = 2*pi/4
% 
% for w = 0:step_size:2*pi - step_size
%     1 + exp(-2i*w) - exp(-1i*3*w)
% end


x = [1 0 1 0];
h = [1 0 -1 1];

xdft = fft(x);
hdft = fft(h);

ydft = xdft .* hdft;

y = ifft(ydft)
conv(x, h)

h = [h zeros(1,3)];
x = [x zeros(1,3)];

xdft = fft(x);
hdft = fft(h);

ydft = xdft .* hdft;
y = ifft(ydft);
conv(x, h);


y = zeros(7, 1);
for n = 0:6
   for k = 0:6
       y(n+1) = y(n+1) + x(k+1) * h(mod(n-k, 7)+1);
   end
end
y
% % 
% % x = ones(101,1);
% % y = zeros(101,1);
% % 
% % y(1)=x(1);
% % y(2)=x(2)+5*y(1)/6;
% % for n = 3:101
% %     y(n) = x(n)+5*y(n-1)/6-y(n-2)/6;
% % end
% % 
% % stem( [0:100], y )

% n = [0:500];
% x = cos(0.04*pi*n) + cos(0.06*pi*n);
% y = 2*cos(0.04*pi*n) + 2*cos(0.06*pi*n);
% 
% subplot(2,1,1);plot(x);
% subplot(2,1,2);plot(y);

% n = [0:500];
% x = cos(0.04*pi*n) + cos(0.06*pi*n);
% y = cos(0.04*pi*n+pi/3) + ...
%     cos(0.06*pi*n+pi/8);
% 
% subplot(2,1,1);plot(x);
% subplot(2,1,2);plot(y);


w = [-0.06*pi -0.04*pi 0.04*pi 0.06*pi]
abs(ones(size(w)) ./ (1-0.5*exp(1i*w)))
angle(ones(size(w)) ./ (1-0.5*exp(1i*w)))





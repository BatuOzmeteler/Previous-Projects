
% z = 3.3 - 0.9*1i;
% abs(z)
% phase(z)
% 
% z=1 + exp(1i*0.2*pi)+1 / (1-0.3*exp(1i*0.2*pi))
% 
% abs(z)
% phase(z)

% ss = 2*pi/1024;
% 
% w = [-pi:ss:pi];
% 
% H = 1 + exp(-1i*w) + 1 ./ (1 - 0.3*exp(-1i*w));
% 
% subplot(2,1,1);
% plot(w, abs(H));
% subplot(2,1,2);
% plot(w, angle(H))

% t = [0:100];
% 
% x = sin(pi*t/8);
% y = zeros(101, 1);
% 
% y(1) = 2*x(1);
% y(2) = 2*x(2) + 0.7*x(1) + 0.3*y(1);
% for n = 3:101
%     y(n) = 2*x(n) + 0.7*x(n-1)-0.3*x(n-2)+0.3*y(n-1);
% end
% 
% subplot(2,1,1); plot(t, x);
% subplot(2,1,2); plot(t, y);

% w = 0.4*pi
% H1 = 1 + exp(-1i*w) + 1 ./ (1 - 0.3*exp(-1i*w));
% 
% abs(H1)
% phase(H1)
% w = 0.6*pi;
% H2 = 1 + exp(-1i*w) + 1 ./ (1 - 0.3*exp(-1i*w));
% abs(H2)
% phase(H2)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% ss = 2*pi/1024;
% w = -pi:ss:pi;
% 
% r = 0.5;
% theta = pi/4;
% H = 1 ./ (1-r*exp(1i*theta)*exp(-1i*w));
% 
% subplot(2,1,1); plot(w, abs(H));
% subplot(2,1,2); plot(w, angle(H));
% 
% freqz(1, [1 -r*exp(1i*theta)])

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ss = 2*pi/1024;
w = -pi:ss:pi;

r = 0.5;
theta = pi/4;
H = 1 ./ ((1-r*exp(1i*theta)*exp(-1i*w)) .* (1-r*exp(-1i*theta)*exp(-1i*w)));

subplot(2,1,1); plot(w, abs(H));
subplot(2,1,2); plot(w, angle(H));

%freqz(1, [1 -r*exp(1i*theta)])




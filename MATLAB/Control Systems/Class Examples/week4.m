% 
% H = tf(1, [1 0 2]);
% subplot(3,1,1); impulse(H);
% subplot(3,1,2); step(H);
% subplot(3,1,3); step( H*tf(1, [1 0]) ); % ramp response
% 
% 
% H = tf(1, [1 4 3]);
% subplot(3,1,1); impulse(H);
% subplot(3,1,2); step(H);
% subplot(3,1,3); step( H*tf(1, [1 0]) ); % ramp response
% 
% 
% H = tf(1, [1 1 2]);
% subplot(3,1,1); impulse(H);
% subplot(3,1,2); step(H);
% subplot(3,1,3); step( H*tf(1, [1 0]) ); % ramp response


% H = tf(1, [1 1 1]);
% 
% [y,t] = step(H);
% 
% y_ss = y(end);
% y1 = 0.1*y_ss;
% y2 = 0.9*y_ss;
% 
% t1 = 0;
% t2 = 0;
% for i = 1:length(y)
% 
%     if ((y(i) > y1) && (t1 == 0))
%         t1 = t(i);
%     end
%     
%     if ((y(i) > y2) && (t2 == 0))
%         t2 = t(i);
%     end
% end
% 
% 
% [ymax, ind] = max(y);
% Tp = t(ind)
% 
% Tr = t2 - t1
% 
% os = 100*((ymax - y_ss) / y_ss)
% 
% 
% for i = length(y):-1:1
%     i
%     if abs(y(i) - y_ss) > 0.02*y_ss
%         T_s = t(i)
%         break
%     end
% end
% 
% 
% H = tf(361, [1 16 361]);
% 
% wn = sqrt(361)
% zeta = 16 / (2*wn)
% 
% [y,t] = step(H);
% 
% y_ss = y(end);
% y1 = 0.1*y_ss;
% y2 = 0.9*y_ss;
% 
% t1 = 0;
% t2 = 0;
% for i = 1:length(y)
% 
%     if ((y(i) > y1) && (t1 == 0))
%         t1 = t(i);
%     end
%     
%     if ((y(i) > y2) && (t2 == 0))
%         t2 = t(i);
%     end
% end
% 
% 
% [ymax, ind] = max(y);
% Tp = t(ind)
% 
% Tr = t2 - t1
% 
% os = 100*((ymax - y_ss) / y_ss)
% 
% 
% for i = length(y):-1:1
%     if abs(y(i) - y_ss) > 0.02*y_ss
%         T_s = t(i)
%         break
%     end
% end
% 
% Tp
% Tp_math = pi / (wn*sqrt(1 - zeta^2))
% os
% os_math = exp(-zeta*pi/sqrt(1 - zeta^2))*100
% T_s
% Ts_math = 4 / (zeta*wn)


T1 = tf(24.542, [1 4 24.542]);
T2 = T1 * tf(1, [1 10]) * (245.42/24.542);
T3 = T1 * tf(1, [1 3]) * (73.626/24.542);


step(T1); hold on;
step(T2); hold on;
step(T3)
legend('T1', 'T2', 'T3')

figure;
pzmap(T2)


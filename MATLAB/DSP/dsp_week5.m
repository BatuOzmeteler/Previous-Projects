
% step_size = 2*pi / 512;
% w = [-pi:step_size:pi - step_size];
% 
% % x = ones(1, length(w)) ./ (1 - 0.5*exp(-1i*w));
% 
% plot( w, abs(x) )
% plot( w, angle(x) )

step_size = 2*pi / 512;
w = [-pi:step_size:pi - step_size];


x = (1/8)*exp(-1i*3*w).*...
    ones(1, length(w)) ./ (1 - 0.5*exp(-1i*w));

%plot( w, abs(x) )
plot( w, angle(x) )


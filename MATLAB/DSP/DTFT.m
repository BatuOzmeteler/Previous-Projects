step_size = 2*pi / 512;
w = [-pi:step_size:pi - step_size];

%a degeri x(n) = a^n * u(n) de ki a nin kendisi
x = ones(1, length(w)) ./ (1 - a*exp(-1i*w));

plot( w, abs(x) )
plot( w, angle(x) )
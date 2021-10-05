
a = (1/0.8913)^2 - 1;
b = (1/0.17783)^2 - 1;


N = ceil( log(a/b) / (2*log(0.2/0.3)) );

Wc1 = exp((12*log(0.2*pi) - log(a))/12)

Wc2 = exp((12*log(0.3*pi) - log(b))/12)

Wc = min(Wc1, Wc2);


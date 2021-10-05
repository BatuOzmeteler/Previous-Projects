
x = 10^(-1/20); %% replace db (-1) passband
y = 10^(-40/20); %% replace db(-15) stopband


a = (1/x)^2 - 1;
b = (1/y)^2 - 1;


N = ceil( log(a/b) / (2*log(0.2/0.6)) ); %% 2*log(passband frequency / stopband frequency)

Wc1 = exp((2*N*log(0.2*pi) - log(a))/12); %% passband frequency

Wc2 = exp((2*N*log(0.6*pi) - log(b))/12); %% stopband frequency

Wc = min(Wc1, Wc2);

sk = ones(2*-N,1);
%%poles of the filter
for k = 1:2*-N
 sk(k) = Wc * exp(1i*(pi/(2*N))*(2*k + (N-1))); %% k is 0,1,2,..to N-1
end

%%p1,p2... kac tane pole varsa yerine sk'de ki degerleri yaz(sadece
%%axisin sol tarafindaki degerler)
% p1 = -0.1820 + 0.6792*1i ; 
% p2 = -0.4972 + 0.4972*1i ; 
% p3 = -0.6792 + 0.1820*1i ; 
% p4 = -0.6792 - 0.1820*1i ; 
% p5 = -0.4972 - 0.4972*1i ; 
% p6 = -0.1820 - 0.6792*1i ; 
% r1 = conv([1, -p1], [1, -p6]);
% r2 = conv([1, -p2], [1, -p5]);
% r3 = conv([1, -p3], [1, -p4]);
% 
% rTotal = conv(r1 , conv(r2,r3)); 
% 
% A = Wc^N;
% B = conv(r1 , conv(r2,r3)); 
% 
% [r, p, k] = residue(A,B);
% 

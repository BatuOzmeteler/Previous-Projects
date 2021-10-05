
% B = conv(conv([1 0.3640 0.4945], [1 0.9945 0.4945]), [1 1.3585 0.4945]);
% A = 0.12093;
% 
% [r,p,~] = residue(A, B)
% 
% % when we discretize, sk terms should become exp(sk)
% p = exp(p)

fp = fopen('TIMIT_ver110train_0001.raw', 'r');
x = fread(fp, 'short');
fclose(fp);

%soundsc(a, 16000);


ss = 2*pi/512;
w = [0:ss:2*pi];

H1 = (0.2871-0.4466*exp(-1i*w)) ./ (1-1.2971*exp(-1i*w)+0.6949*exp(-2*1i*w));
H2 = (-2.1428+1.1455*exp(-1i*w)) ./ (1-1.0691*exp(-1i*w)+0.3699*exp(-2*1i*w));
H3 = (1.8557-0.6303*exp(-1i*w)) ./ (1-0.9972*exp(-1i*w)+0.2570*exp(-2*1i*w));

%plot(w, abs(H1+H2+H3));

% implement the first filter
y = zeros(length(x), 1);
y(1) = 0.2871*x(1);
y(2) = 1.2971*y(1) + 0.2871*x(2) - 0.4466*x(1);
for n = 3:length(x)
    y(n) = 1.2971*y(n-1) - 0.6949*y(n-2) + 0.2871*x(n) - 0.4466*x(n-1);
end

y1 = y;


% implement the second filter
y = zeros(length(x), 1);
y(1) = -2.1428*x(1);
y(2) = 1.0691*y(1) - 2.1428*x(2) + 1.1455*x(1);
for n = 3:length(x)
    y(n) = 1.0691*y(n-1) - 0.3699*y(n-2) - 2.1428*x(n) + 1.1455*x(n-1);
end

y2 = y;


% implement the third filter
y = zeros(length(x), 1);
y(1) = 1.8557*x(1);
y(2) = 0.9972*y(1) + 1.8557*x(2) - 0.6303*x(1);
for n = 3:length(x)
    y(n) = 0.9972*y(n-1) - 0.2570*y(n-2) + 1.8557*x(n) - 0.6303*x(n-1);
end

y3 = y;


y = y1 + y2 + y3;

subplot(2,1,1);specgram(x);
subplot(2,1,2);specgram(y);




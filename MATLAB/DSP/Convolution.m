x = zeros(31,1);
x(1)=1;
x(2)=-1;
x(3)=1;

h = zeros(31,1);
h(1)=1;
h(2)=0;
h(3)=1;
h(4)=-1;

y = zeros(31, 1);
for n = 1:31 % fonsiyonlarin boyutu
    y(n) = 0;
    for k = 1:3 % x nin uzunlugu
        if (n-k+1) > 0
            y(n) = y(n) + x(k)*h(n-k+1);
        end
    end
end

stem([0:30], y);
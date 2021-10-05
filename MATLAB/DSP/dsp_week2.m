
% x = [1 -2 1 -1 0 0]; % we pad zeroes at the end of x
%                      % so that we don't have issue in the for loop
% 
% 
% y(1) = (1/5)*(x(1)+x(2)+x(3));
% y(2) = (1/5)*(x(1)+x(2)+x(3)+x(4));
% for n = 3:length(x)-2
%     
%     y(n) = (1/5)*(x(n-2)+x(n-1)+x(n)+x(n+1)+x(n+2));
% end
% 
% stem(y)

x = [1 1 2 0 0 0];
y = zeros(6, 1);

for n = 0:5
    y(n+1) = sum( x(1:n+1) );
end

y = [0 ; 0 ; y];

stem([-2:5], y)

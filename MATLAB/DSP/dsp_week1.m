
% z=2+1i;
% 
% z^13
% 
% mag_z = abs(z);
% pha_z = angle(z);
% 
% 
% mag_z^13 * exp(1i*13*pha_z)



x = [0 0 1 -2 1 -1 0 0];


% be careful about the beginning and end indices
% y(n) should start from n=1 in matlab
% you cannot use n>8 for x(n) because the x vector
% has a length of 8
%
y(1) = (1/3)*(x(1)+x(2)) % assume x(0)=0
for n = 2:length(x)-1
    
    y(n) = (1/3)*(x(n-1)+x(n)+x(n+1));
end

stem(y)

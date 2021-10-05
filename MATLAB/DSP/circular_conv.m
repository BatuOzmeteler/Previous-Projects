x = [1 0 1 0 0 0 0 ];
h = [1 0 -1 1 0 0 0];

y = zeros(7, 1);
for n = 0:6
   for k = 0:6
       y(n+1) = y(n+1) + x(k+1) * h(mod(n-k, 7)+1);
   end
end
y
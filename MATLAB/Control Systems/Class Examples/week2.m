% example-1
%
% H(s) = 1 / ((s+1)(s+2))

% H(s) = A(s) / B(s)
%
% where A(s) and B(s) are polynomials of s

% A = 1;
% B = [1 3 2];
% 
% [r,p,k] = residue(A, B)


% % H(s) = 2 / (s(s+1)(s+2))
% 
% A = 2;
% B = [1 3 2 0];
% 
% [r,p,k] = residue(A, B)


% H(s) = 2 / ((s+1)(s+2)^2)

% A = 2;
% B = conv( [1 1], [1 4 4] );
% 
% [r,p,k] = residue(A, B)


% A = [1 2];
% B = conv( conv( [1 1], [1 6 9] ), [1 3]);
% 
% [r,p,k] = residue(A, B)


A = 1;
B = [1 2 2];

[r,p,k] = residue(A, B)


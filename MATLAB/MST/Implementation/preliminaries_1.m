%-------------------------------------------------------------------------%
% Case Study A - PDE Based Motion Planning for Robots
% Author: Mehmet Batu Özmeteler, 230306

% Mathematical Preliminaries for the Benchmark Example
%-------------------------------------------------------------------------%

clear all
clc

syms x y theta1 theta2 k 

u = [x; y; theta1; theta2];

% Compute Ff from Fc
Fc = [1 0; 0 1; sin(theta1) -cos(theta1); sin(theta2) -cos(theta2)];
%Ff = null(Fc');
Ff = [-sin(theta1) -sin(theta2); cos(theta1) cos(theta2); 1 0; 0 1];
F = [Fc Ff];

% Compute G
K = [k 0 0 0; 0 k 0 0; 0 0 1 0; 0 0 0 1];
G_computed = F*K*F';

% Write G explicitly
G = [       (sin(u(3))^2 + sin(u(4))^2 + k), ((-sin(2*u(3))/2) + (-sin(2*u(4))/2)), ((k-1)*sin(u(3))) ,  ((k-1)*sin(u(4)));
      ((-sin(2*u(3))/2) + (-sin(2*u(4))/2)),      ( cos(u(3))^2 + cos(u(4))^2 + k), -((k-1)*cos(u(3))), -((k-1)*cos(u(4)));
                          ((k-1)*sin(u(3))),                    -((k-1)*cos(u(3))),              (k+1), (k*cos(u(3)-u(4)));
                          ((k-1)*sin(u(4))),                    -((k-1)*cos(u(4))), (k*cos(u(3)-u(4))),             (k+1)];

% Validate explicit G
if sum(double(subs(G - G_computed, {theta1, theta2}, {pi/3,pi/3})) == zeros(4), 'all') == 16
    disp('Explicit G is VALID')
else
    disp('Explicit G is INVALID')
end

% Compute partial derivatives of G
G1 = G(:,1);
G2 = G(:,2);
G3 = G(:,3);
G4 = G(:,4);

Gx      = [jacobian(G1, u(1)), jacobian(G2, u(1)), jacobian(G3, u(1)), jacobian(G4, u(1))];
Gy      = [jacobian(G1, u(2)), jacobian(G2, u(2)), jacobian(G3, u(2)), jacobian(G4, u(2))];
Gtheta1 = [jacobian(G1, u(3)), jacobian(G2, u(3)), jacobian(G3, u(3)), jacobian(G4, u(3))];
Gtheta2 = [jacobian(G1, u(4)), jacobian(G2, u(4)), jacobian(G3, u(4)), jacobian(G4, u(4))];

% Write partial derivatives of G explicitly
pG(:,:,1) = sym(zeros(4));
pG(:,:,2) = sym(zeros(4));

pG(:,:,3) = [ 2*cos(u(3))*sin(u(3)),           -cos(2*u(3)),   cos(u(3))*(k - 1),                   0;
                       -cos(2*u(3)), -2*cos(u(3))*sin(u(3)),   sin(u(3))*(k - 1),                   0;
                  cos(u(3))*(k - 1),      sin(u(3))*(k - 1),                   0, -k*sin(u(3) - u(4));
                                  0,                      0, -k*sin(u(3) - u(4)),                  0];

pG(:,:,4) = [ 2*cos(u(4))*sin(u(4)),           -cos(2*u(4)),                  0,  cos(u(4))*(k - 1);
                       -cos(2*u(4)), -2*cos(u(4))*sin(u(4)),                  0,  sin(u(4))*(k - 1);
                                  0,                      0,                  0, k*sin(u(3) - u(4));
                  cos(u(4))*(k - 1),      sin(u(4))*(k - 1), k*sin(u(3) - u(4)),                 0];

% Validate explicit partial derivatives of G
if (sum(double(subs(pG(:,:,1) - Gx, {theta1, theta2}, {pi/3,pi/3})) == zeros(4), 'all') == 16) &&...
   (sum(double(subs(pG(:,:,2) - Gy, {theta1, theta2}, {pi/3,pi/3})) == zeros(4), 'all') == 16) &&...
   (sum(double(subs(pG(:,:,3) - Gtheta1, {theta1, theta2}, {pi/3,pi/3})) == zeros(4), 'all') == 16) &&...
   (sum(double(subs(pG(:,:,4) - Gtheta2, {theta1, theta2}, {pi/3,pi/3})) == zeros(4), 'all') == 16)

    disp('Explicit partial derivatives of G are VALID')
else
    disp('Explicit partial derivatives of G are INVALID')
end

% Double check partial derivatives of G with the provided derivative formula

F1 = F(:,1);
F2 = F(:,2);
F3 = F(:,3);
F4 = F(:,4);

Fx      = [jacobian(F1, u(1)), jacobian(F2, u(1)), jacobian(F3, u(1)), jacobian(F4, u(1))];
Fy      = [jacobian(F1, u(2)), jacobian(F2, u(2)), jacobian(F3, u(2)), jacobian(F4, u(2))];
Ftheta1 = [jacobian(F1, u(3)), jacobian(F2, u(3)), jacobian(F3, u(3)), jacobian(F4, u(3))];
Ftheta2 = [jacobian(F1, u(4)), jacobian(F2, u(4)), jacobian(F3, u(4)), jacobian(F4, u(4))];
                
Hx = Fx*K*F' + F*K*Fx';
Hy = Fy*K*F' + F*K*Fy';
Htheta1 = Ftheta1*K*F' + F*K*Ftheta1';
Htheta2 = Ftheta2*K*F' + F*K*Ftheta2';

if (sum(double(subs(pG(:,:,1) - Hx, {x, y, theta1, theta2}, {1,1,pi/3,pi/3})) == zeros(4), 'all') == 16) &&...
   (sum(double(subs(pG(:,:,2) - Hy, {x, y, theta1, theta2}, {1,1,pi/3,pi/3})) == zeros(4), 'all') == 16) &&...
   (sum(double(subs(pG(:,:,3) - Htheta1, {x, y, theta1, theta2}, {1,1,pi/3,pi/3})) == zeros(4), 'all') == 16) &&...
   (sum(double(subs(pG(:,:,4) - Htheta2, {x, y, theta1, theta2}, {1,1,pi/3,pi/3})) == zeros(4), 'all') == 16)

    disp('Explicit partial derivatives of H are VALID')
else
    disp('Explicit partial derivatives of H are INVALID')
end  

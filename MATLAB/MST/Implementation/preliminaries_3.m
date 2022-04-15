%-------------------------------------------------------------------------%
% Case Study A - PDE Based Motion Planning for Robots
% Author: Mehmet Batu Özmeteler, 230306

% Mathematical Preliminaries for the Benchmark Example with the Additional
% Holonomic Constraint q4 = (x - x_c)^2 + (y - y_c)^2 - r^2 = 0 Resulting 
% in the Circular Motion of the End-Effector
%-------------------------------------------------------------------------%

clear all
clc

syms x y theta1 theta2 k 

u = [x; y; theta1; theta2];

% Circular motion path constraint where (x_c, y_c, r) = (0, 1, 1)
q4 = x^2 + (y - 1)^2 - 1;
derivatives = jacobian(q4, u);

% Compute Ff from Fc
Fc = [1 0 -2*u(1); 0 1 -(2*u(2) - 2); sin(u(3)) -cos(u(3)) 0; sin(u(4)) -cos(u(4)) 0];
%Ff = null(Fc');

% Rescaled Ff to avoid numerical problems
Ff = [                            -(sin(conj(u(3)) - conj(u(4)))*(conj(u(2)) - 1));
                                         (conj(u(1))*sin(conj(u(3)) - conj(u(4))));
      -(cos(conj(u(4))) - cos(conj(u(4)))*conj(u(2)) + sin(conj(u(4)))*conj(u(1)));
              cos(conj(u(3))) - cos(u(3))*conj(u(2)) + sin(conj(u(3)))*conj(u(1))];

F = [Fc Ff];

% Compute G
K = [k 0 0 0; 0 k 0 0; 0 0 k 0; 0 0 0 1];
G_computed = F*K*F';

% Write G explicitly

G = [                  k + 4*k*u(1)*conj(u(1)) + sin(u(3) - u(4))*sin(conj(u(3)) - conj(u(4)))*(conj(u(2)) - 1)*(u(2) - 1),                 2*k*u(1)*(2*conj(u(2)) - 2) - u(1)*sin(u(3) - u(4))*sin(conj(u(3)) - conj(u(4)))*(conj(u(2)) - 1),                                                                       k*sin(conj(u(3))) + sin(conj(u(3)) - conj(u(4)))*(conj(u(2)) - 1)*(cos(u(4)) - u(2)*cos(u(4)) + u(1)*sin(u(4))),                                                                       k*sin(conj(u(4))) - sin(conj(u(3)) - conj(u(4)))*(conj(u(2)) - 1)*(cos(u(3)) - u(2)*cos(conj(u(3))) + u(1)*sin(u(3)));
                         2*k*conj(u(1))*(2*u(2) - 2) - sin(u(3) - u(4))*conj(u(1))*sin(conj(u(3)) - conj(u(4)))*(u(2) - 1),             k + k*(2*u(2) - 2)*(2*conj(u(2)) - 2) + u(1)*sin(u(3) - u(4))*conj(u(1))*sin(conj(u(3)) - conj(u(4))),                                                                           - k*cos(conj(u(3))) - conj(u(1))*sin(conj(u(3)) - conj(u(4)))*(cos(u(4)) - u(2)*cos(u(4)) + u(1)*sin(u(4))),                                                                             conj(u(1))*sin(conj(u(3)) - conj(u(4)))*(cos(u(3)) - u(2)*cos(conj(u(3))) + u(1)*sin(u(3))) - k*cos(conj(u(4)));
     k*sin(u(3)) + sin(u(3) - u(4))*(u(2) - 1)*(cos(conj(u(4))) - cos(conj(u(4)))*conj(u(2)) + sin(conj(u(4)))*conj(u(1))), - k*cos(u(3)) - u(1)*sin(u(3) - u(4))*(cos(conj(u(4))) - cos(conj(u(4)))*conj(u(2)) + sin(conj(u(4)))*conj(u(1))), (cos(conj(u(4))) - cos(conj(u(4)))*conj(u(2)) + sin(conj(u(4)))*conj(u(1)))*(cos(u(4)) - u(2)*cos(u(4)) + u(1)*sin(u(4))) + k*cos(conj(u(3)))*cos(u(3)) + k*sin(conj(u(3)))*sin(u(3)), k*cos(conj(u(4)))*cos(u(3)) - (cos(u(3)) - u(2)*cos(conj(u(3))) + u(1)*sin(u(3)))*(cos(conj(u(4))) - cos(conj(u(4)))*conj(u(2)) + sin(conj(u(4)))*conj(u(1))) + k*sin(conj(u(4)))*sin(u(3));
           k*sin(u(4)) - sin(u(3) - u(4))*(u(2) - 1)*(cos(conj(u(3))) - cos(u(3))*conj(u(2)) + sin(conj(u(3)))*conj(u(1))),         u(1)*sin(u(3) - u(4))*(cos(conj(u(3))) - cos(u(3))*conj(u(2)) + sin(conj(u(3)))*conj(u(1))) - k*cos(u(4)),       k*cos(conj(u(3)))*cos(u(4)) - (cos(conj(u(3))) - cos(u(3))*conj(u(2)) + sin(conj(u(3)))*conj(u(1)))*(cos(u(4)) - u(2)*cos(u(4)) + u(1)*sin(u(4))) + k*sin(conj(u(3)))*sin(u(4)),      (cos(u(3)) - u(2)*cos(conj(u(3))) + u(1)*sin(u(3)))*(cos(conj(u(3))) - cos(u(3))*conj(u(2)) + sin(conj(u(3)))*conj(u(1))) + k*cos(conj(u(4)))*cos(u(4)) + k*sin(conj(u(4)))*sin(u(4))];
 
% Validate explicit G
if sum(double(subs(G - G_computed, {x, y, theta1, theta2}, {1, 1, pi/3, pi/3})) == zeros(4), 'all') == 16
    disp('Explicit G is VALID')
else
    disp('Explicit G is INVALID')
end

% Compute partial derivatives of G using the provided derivative formula
F1 = F(:,1);
F2 = F(:,2);
F3 = F(:,3);
F4 = F(:,4);

F_x      = [jacobian(F1, u(1)), jacobian(F2, u(1)), jacobian(F3, u(1)), jacobian(F4, u(1))];
F_y      = [jacobian(F1, u(2)), jacobian(F2, u(2)), jacobian(F3, u(2)), jacobian(F4, u(2))];
F_theta1 = [jacobian(F1, u(3)), jacobian(F2, u(3)), jacobian(F3, u(3)), jacobian(F4, u(3))];
F_theta2 = [jacobian(F1, u(4)), jacobian(F2, u(4)), jacobian(F3, u(4)), jacobian(F4, u(4))];

Fx   = [ 0, 0, -2,                            0;
         0, 0,  0, sin(conj(u(3)) - conj(u(4)));
         0, 0,  0,             -sin(conj(u(4)));
         0, 0,  0,             sin(conj(u(3)))];


Fy   = [ 0, 0,  0, -sin(conj(u(3)) - conj(u(4)));
         0, 0, -2,                             0;
         0, 0,  0,               cos(conj(u(4)));
         0, 0,  0,                   -cos(u(3))];

Fth1 = [          0,         0, 0,                       -cos(conj(u(3)) - conj(u(4)))*(conj(u(2)) - 1);
                  0,         0, 0,                              cos(conj(u(3)) - conj(u(4)))*conj(u(1));
          cos(u(3)), sin(u(3)), 0,                                                                    0;
                  0,         0, 0, conj(u(2))*sin(u(3)) - sin(conj(u(3))) + cos(conj(u(3)))*conj(u(1))];


Fth2 = [         0,         0, 0,                             cos(conj(u(3)) - conj(u(4)))*(conj(u(2)) - 1);
                 0,         0, 0,                                  -cos(conj(u(3)) - conj(u(4)))*conj(u(1));
                 0,         0, 0, sin(conj(u(4))) - cos(conj(u(4)))*conj(u(1)) - sin(conj(u(4)))*conj(u(2));
         cos(u(4)), sin(u(4)), 0,                                                                        0];

pG(:,:,1) = Fx*K*F' + F*K*Fx';
pG(:,:,2) = Fy*K*F' + F*K*Fy';
pG(:,:,3) = Fth1*K*F' + F*K*Fth1';
pG(:,:,4) = Fth2*K*F' + F*K*Fth2';

G1 = G(:,1);
G2 = G(:,2);
G3 = G(:,3);
G4 = G(:,4);

% Validate explicit partial derivatives of G
Gx      = [jacobian(G1, u(1)), jacobian(G2, u(1)), jacobian(G3, u(1)), jacobian(G4, u(1))];
Gy      = [jacobian(G1, u(2)), jacobian(G2, u(2)), jacobian(G3, u(2)), jacobian(G4, u(2))];
Gtheta1 = [jacobian(G1, u(3)), jacobian(G2, u(3)), jacobian(G3, u(3)), jacobian(G4, u(3))];
Gtheta2 = [jacobian(G1, u(4)), jacobian(G2, u(4)), jacobian(G3, u(4)), jacobian(G4, u(4))];

if (sum(double(subs(Gx - pG(:,:,1), {x, y, theta1, theta2}, {1,1,pi/3,pi/3})) == zeros(4), 'all') == 16) &&...
   (sum(double(subs(Gy - pG(:,:,2), {x, y, theta1, theta2}, {1,1,pi/3,pi/3})) == zeros(4), 'all') == 16) &&...
   (sum(double(subs(Gtheta1 - pG(:,:,3), {x, y, theta1, theta2}, {1,1,pi/3,pi/3})) == zeros(4), 'all') == 16) &&...
   (sum(double(subs(Gtheta2 - pG(:,:,4), {x, y, theta1, theta2}, {1,1,pi/3,pi/3})) == zeros(4), 'all') == 16)

    disp('Explicit partial derivatives of G are VALID')
else
    disp('Explicit partial derivatives of G are INVALID')
end                                                                                                                                                                                                                                                    

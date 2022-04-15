%% Two Link Planar Arm - Incorrect Model
% Author: Mehmet Batu Özmeteler
%%
function xf = two_link_planar_arm_incorrect_model(x, u)

% Parameters
b1    = 180;           % [kg*m2 / rad] 
b2    = 45;            % [kg*m2 / rad]
b3    = 23.5;          % [kg*m2 / rad]
b4    = 25;            % [kg*m2 / rad]
b5    = 122.5;         % [kg*m2 / rad]
c1    = -25;           % [N*m / s2]
g1    = 784.8;         % [N*m]
g2    = 245.3;         % [N*m]

% States
q1    = x(1);          % [rad]
q2    = x(2);          % [rad]
q1_d  = x(3);          % [rad / s]
q2_d  = x(4);          % [rad / s]

% Inputs
tao_1 = u(1);          % [N*m]
tao_2 = u(2);          % [N*m]

% System Matrices
q = [q1; q2];
q_d = [q1_d; q2_d];
tao = [tao_1; tao_2];

B = [ b1 + b2*cos(q2) , b3 + b4*cos(q2);
      b3 + b4*cos(q2) ,      b5        ];  

C = -c1*sin(q2)*[ q2_d , (q1_d + q2_d);
                  -q1_d  ,       0      ];
G = [ g1*cos(q1) + g2*cos(q1 + q2); g2*cos(q1 + q2) ];

out = inv(B) * ( tao - C*q_d - G);

% Output
x1_d = q1_d;
x2_d = q2_d;
x3_d = out(1);
x4_d = out(2);

xf = [x1_d; x2_d; x3_d; x4_d];

end



clear all
clc

syms q1 q2 q1_d q2_d q1_dd q2_dd 
syms b1 b2 b3 b4 b5 c1 g1 g2 l1 l2

p1  = 200;           % [kg*m2 / rad] 
p2  = 50;            % [kg*m2 / rad]
p3  = 23.5;          % [kg*m2 / rad]
p4  = 25;            % [kg*m2 / rad]
p5  = 122.5;         % [kg*m2 / rad]
p6  = -25;           % [N*m / s2]
p7  = 784.8;         % [N*m]
p8  = 245.3;         % [N*m]
p9  = 0.5;           % [m]
p10 = 0.5;           % [m]

B = [ b1 + b2*cos(q2) , b3 + b4*cos(q2);
      b3 + b4*cos(q2) ,      b5        ];
C = -c1*sin(q2)*[ q2_d , (q1_d + q2_d);
                  -q1  ,       0      ];
G = [ g1*cos(q1) + g2*cos(q1+q2); g2*cos(q1+q2) ];
B_inv = inv(B);

B_num = subs(B, {b1,b2,b3,b4,b5} ,{p1,p2,p3,p4,p5});
C_num = subs(C, {c1}, {p6});
G_num = subs(G, {g1,g2}, {p7,p8});
B_inv_num = subs(B_inv, {b1,b2,b3,b4,b5}, {p1,p2,p3,p4,p5});

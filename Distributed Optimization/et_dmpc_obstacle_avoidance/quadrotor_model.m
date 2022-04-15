%% 3D Non-linear Quadrotor Model
% Authors: 
% - Mehmet Batu Özmeteler
% - Shehabeldine Housein
%% States
% n_x = 8, x: [x, x_dot, y, y_dot, z, z_dot, theta, phi]
% where; 
% x is the position in x-axis
% x_dot is the velocity on x-axis
% y is the position in y-axis
% y_dot is the velocity on y-axis
% z is the position in z-axis
% z_dot is the velocity on z-axis
% theta is the pitch angle (to move forward or backward)
% phi is the roll angle (to move right or left)
%% Inputs
% n_u = 3, u: [f w_theta w_phi]
% where;
% m is the mass of the robot
% f is the thrust force
% w_theta is the angular velocity in pitch
% w_phi is the angular velocity in roll
%% 
function xf = quadrotor_model(x, u, m)

    theta = x(7);
    phi = x(8);

    x_dot = x(2);
    x_ddot = (u(1)/m)*cos(phi)*sin(theta);

    y_dot = x(4);
    y_ddot = -(u(1)/m)*sin(phi);

    z_dot = x(6);
    z_ddot = (u(1)/m)*cos(phi)*cos(theta) - 9.81;

    theta_dot = u(2);
    phi_dot = u(3);

    xf = [x_dot; x_ddot; y_dot; y_ddot; z_dot; z_ddot; theta_dot; phi_dot];

end
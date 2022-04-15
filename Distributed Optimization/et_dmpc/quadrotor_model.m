
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
% function xf = dynamic_obstacle(x,u)
% x_dot = u(1);
% y_dot = u(2);
% z_dot = u(3);
% % theta = u(4);
% % phi   = u(5);
% % xi    = u(6);
% x_pose = x(1) + x_dot;
% y_pose = x(2) + y_dot;
% z_pose = x(3) + z_dot;
% xf = [x_pose;y_pose;z_pose];
% 
% end

function xf = dynamic_obstacle(x,u)

x_dot =  u(1);
y_dot =  u(2);
z_dot =  u(3);
xf = [x_dot;y_dot;z_dot];
end
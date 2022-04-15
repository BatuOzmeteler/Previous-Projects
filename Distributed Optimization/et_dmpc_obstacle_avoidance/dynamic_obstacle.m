%% Constant Velocity Dynamic Obstacle Model
% Authors: 
% - Shehabeldine Housein
%% Summary
% This function creates the constant velocity dynamic obstacle model. 
%%
function xf = dynamic_obstacle(x, u)

    x_dot =  u(1);
    y_dot =  u(2);
    z_dot =  u(3);
    xf = [x_dot; y_dot; z_dot];
    
end

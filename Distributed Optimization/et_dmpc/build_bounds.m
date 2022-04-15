%% Build State and Input Bounds
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function builds the state and input bounds for the problem creation. 
%%
function bounds = build_bounds()

    x_min = -50; x_max = 50;
    xd_min = -5; xd_max = 5;

    y_min = -50; y_max = 50;
    yd_min = -5; yd_max = 5;

    z_min = 0; z_max = 100;
    zd_min = -5; zd_max = 5;

    theta_min = -pi/4; theta_max = pi/4;
    phi_min = -pi/4; phi_max = pi/4;

    x_min = [x_min; xd_min; y_min; yd_min; z_min; zd_min; theta_min; phi_min];
    x_max = [x_max; xd_max; y_max; yd_max; z_max; zd_max; theta_max; phi_max];
    
    u1_min = 0; u1_max = 15;            
    u2_min = -pi/8; u2_max = pi/8;      
    u3_min = -pi/8; u3_max = pi/8;      

    u_min = [u1_min; u2_min; u3_min]; 
    u_max = [u1_max; u2_max; u3_max];
    
    bounds = {x_min, x_max, u_min, u_max};
    
end

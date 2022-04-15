%% Create Dynamic Obstacles
% Authors: 
% - Shehabeldine Housein
%% Summary
% This function runs a closed-loop DMPC simulation 
% with an event-triggering mechanism.
%%
function obstacle_pts = create_dynamic_obstacle(x_0, u, h, t0, T, scale)

    N = (T - t0) / h;
    t = t0:h:T;
    %traj = zeros(4,N);
    obstacle_pts = create_cube(x_0, t(1), scale);
    for i = 1:N
        x = rk4(@(x,u)dynamic_obstacle(x,u), h, x_0, u);
        x_0 = x;
        %traj(:,i) = [x; t(i)];
        obstacle_pts = [obstacle_pts, create_cube(x, t(i+1), scale)];
    end
    
end

function pts = create_cube(centers, t, scale)

    offsets = scale * [1, 1, -1, -1, 1, 1, -1, -1;
                      -1, 1, 1, -1, -1, 1, 1, -1;
                      -1,-1,-1,-1, 1, 1, 1, 1];
    pts = bsxfun(@plus, centers, repmat(offsets ./ (1^(1/3)), [1,1,1]));
    pts = [pts; ones(1,size(pts,2))*t];
    
end
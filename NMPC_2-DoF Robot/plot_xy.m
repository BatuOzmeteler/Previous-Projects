% Plot in x-y-plane
function plot_xy(Xsol, Xsol_pred, x_init, x_goal)
    
    figure();
    sgtitle('x-y Plane') 
    
    xy_init = forwardKinematics(x_init(1:2));
    xy_goal = forwardKinematics(x_goal(1:2));
    
    xy_traj = zeros(2, size(Xsol,2));
    xy_traj_pred = zeros(2, size(Xsol_pred,2));
    
    for i = 1:size(Xsol,2)
        xy_traj(:,i) = forwardKinematics(Xsol(:,i));
    end
    
    for i = 1:size(Xsol_pred,2)
        xy_traj_pred(:,i) = forwardKinematics(Xsol_pred(:,i));
    end
    
    hold on;
    plot(xy_traj_pred(1,:), xy_traj_pred(2,:), 'Color', '#D95319', 'LineWidth', 1.5)
    plot(xy_traj(1,:), xy_traj(2,:), 'k', 'LineWidth', 1.5)
    plot(xy_init(1), xy_init(2), 'xg', 'LineWidth', 2)
    plot(xy_goal(1), xy_goal(2), 'xr', 'LineWidth', 2)
    hold off;
    
    grid ON;
    legend('Predicted', 'Actual')
    xlabel('x (m)')
    ylabel('y (m)')
    
end

% Plot in q1-q2-plane
function plot_q1q2(Xsol, Xsol_pred, x_init, x_goal)
    
    figure();
    sgtitle('q_{1}-q_{2} Plane') 
    
    hold on;
    plot(Xsol_pred(1,:), Xsol_pred(2,:), 'Color', '#D95319', 'LineWidth', 1.5)
    plot(Xsol(1,:), Xsol(2,:), 'k', 'LineWidth', 1.5)
    plot(x_init(1), x_init(2), 'xg', 'LineWidth', 2)
    plot(x_goal(1), x_goal(2), 'xr', 'LineWidth', 2)
    hold off;
    
    grid ON;
    legend('Predicted', 'Actual')
    xlabel('q_{1} (rad)')
    ylabel('q_{2} (rad)')
    
end

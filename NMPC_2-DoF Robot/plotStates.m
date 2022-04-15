% Plot States
function plotStates(t, X, x_init, x_goal, nx)

    figure();
    sgtitle('States') 
    
    for i = 1:nx
        subplot(2,2,i)
        hold on;
        plot(t, X(i,:), 'k', 'LineWidth', 1.5)
        plot(t(1), x_init(i), 'xg', 'LineWidth', 2)
        plot(t(end), x_goal(i), 'xr', 'LineWidth', 2)
        xlabel('time (s)')
        switch i
            case 1
                ylabel('q_{1} (rad)')
            case 2
                ylabel('q_{2} (rad)')
            case 3
                ylabel('$\dot{q_{1}}$ (rad/s)', 'Interpreter', 'latex')
            case 4
                ylabel('$\dot{q_{2}}$ (rad/s)', 'Interpreter', 'latex')
        end
        grid ON;
        hold off;
    end
    
end

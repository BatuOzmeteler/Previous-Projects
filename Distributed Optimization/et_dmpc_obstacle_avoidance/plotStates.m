%% Plot States
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function plots the states for a group of quadrotors.
%%
function plotStates(t, XX, x_inits, x_goals, N_agents)
    line_width = 1.5;
    
    figure();
    sgtitle('States') 
    
    subplot(4,2,1)
    hold on
    for i = 1:N_agents
        plot(t, XX{i}(1,:),'Linewidth', line_width)
        plot(t(1), x_inits{i}(1), 'xg')
        plot(t(end), x_goals{i}(1), 'xr')
    end
    xlabel('time (s)')
    ylabel('x-position (m)')
    hold off
    
    subplot(4,2,2)
    hold on
    for i = 1:N_agents
        plot(t, XX{i}(2,:),'Linewidth', line_width)
        plot(t(1), x_inits{i}(2), 'xg')
        plot(t(end), x_goals{i}(2), 'xr')
    end
    xlabel('time (s)')
    ylabel('x-velocity (m/s)')
    hold off
    
    subplot(4,2,3)
    hold on
    for i = 1:N_agents
        plot(t, XX{i}(3,:),'Linewidth', line_width)
        plot(t(1), x_inits{i}(3), 'xg')
        plot(t(end), x_goals{i}(3), 'xr')
    end
    xlabel('time (s)')
    ylabel('y-position (m)')
    hold off
    
    subplot(4,2,4)
    hold on
    for i = 1:N_agents
        plot(t, XX{i}(4,:),'Linewidth', line_width)
        plot(t(1), x_inits{i}(4), 'xg')
        plot(t(end), x_goals{i}(4), 'xr')
    end
    xlabel('time (s)')
    ylabel('y-velocity (m)')
    hold off
    
    subplot(4,2,5)
    hold on
    for i = 1:N_agents
        plot(t, XX{i}(5,:),'Linewidth', line_width)
        plot(t(1), x_inits{i}(5), 'xg')
        plot(t(end), x_goals{i}(5), 'xr')
    end
    xlabel('time (s)')
    ylabel('z-position (m)')
    hold off
    
    subplot(4,2,6)
    hold on
    for i = 1:N_agents
        plot(t, XX{i}(6,:),'Linewidth', line_width)
        plot(t(1), x_inits{i}(6), 'xg')
        plot(t(end), x_goals{i}(6), 'xr')
    end
    xlabel('time (s)')
    ylabel('z-velocity (m)')
    hold off
    
    subplot(4,2,7)
    hold on
    for i = 1:N_agents
        plot(t, XX{i}(7,:),'Linewidth', line_width)
        plot(t(1), x_inits{i}(7), 'xg')
        plot(t(end), x_goals{i}(7), 'xr')
    end
    xlabel('time (s)')
    ylabel('theta (rad)')
    hold off
    
    subplot(4,2,8)
    hold on
    for i = 1:N_agents
        plot(t, XX{i}(8,:),'Linewidth', line_width)
        plot(t(1), x_inits{i}(8), 'xg')
        plot(t(end), x_goals{i}(8), 'xr')
    end
    xlabel('time (s)')
    ylabel('phi (rad)')
    hold off
    
end

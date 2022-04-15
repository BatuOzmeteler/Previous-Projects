%% Plot Inputs
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function plots the inputs for a group of quadrotors.
%%
function plotInputs(t, UU, N_agents)
    line_width = 1.5;

    figure();
    sgtitle('Inputs') 
    
    subplot(3,1,1)
    hold on
    for i = 1:N_agents
        plot(t(1:end-1), UU{i}(1,:),'Linewidth', line_width)
    end
    xlabel('time (s)')
    ylabel('thrust acceleration')
    hold off
    
    subplot(3,1,2)
    hold on
    for i = 1:N_agents
        plot(t(1:end-1), UU{i}(2,:),'Linewidth', line_width)
    end
    xlabel('time (s)')
    ylabel('w_{theta} (rad/s)')
    hold off
    
    subplot(3,1,3)
    hold on
    for i = 1:N_agents
        plot(t(1:end-1), UU{i}(3,:),'Linewidth', line_width)
    end
    xlabel('time (s)')
    ylabel('w_{phi} (rad/s)')
    hold off
    
end

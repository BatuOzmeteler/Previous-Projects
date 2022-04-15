%% Step-by-Step Plotting for the Event-Triggering DMPC Loop
% Authors: 
% - Ricardo França
%% Summary
% This function plots the closed-loop event-triggering DMPC simulation. 
%%
function plot_multi_robot_ADMM(list_Xsol, list_Usol, Xsol_pred, Usol_pred, list_time, Tsim, param, trigger_times)

% Deal parameters
[N_agents, ~, dt, nx, nu, ~, ~, ~, ~, ~, ~, x_goals, ~]...
    = deal_param(param);

% Time list of the predicted states
Tpred_s = trigger_times(end) + dt : dt : trigger_times(end) + dt*(length(Xsol_pred{1}(1,:)));

figure(1)
line_width = 1.0;
sgtitle('States')

% Plotting states
for j = 1:nx
    subplot(4,2,j)
    hold off
    for i = 1:N_agents
        plot(Tpred_s, Xsol_pred{i}(j,:),'--','Linewidth', line_width)
        hold on
        plot(list_time, list_Xsol{i}(j,:),'Linewidth', line_width)
        plot(Tsim, x_goals{i}(j), 'xr')
    end
    xline(trigger_times)
    xlabel('time (s)')
    xlim([list_time(1), Tsim])
    switch j
        case 1
            ylabel('x-position (m)')
        case 2
            ylabel('x-velocity (m/s)')
        case 3
            ylabel('y-position (m)')
        case 4
            ylabel('y-velocity (m/s)')
        case 5
            ylabel('z-position (m)')
        case 6
            ylabel('z-velocity (m/s)')
        case 7
            ylabel('theta (rad)')
        otherwise
            ylabel('phi (rad)')
    end
end

% Time list of the predicted inputs
Tpred_u = trigger_times(end) : dt : trigger_times(end) + dt*(length(Usol_pred{1}(1,:)));

% Plotting Inputs
figure (2)
line_width = 1.0;
sgtitle('Inputs')
for j = 1:nu
    subplot(3,1,j)
    hold off
    for i = 1:N_agents
        plot(Tpred_u(1:end-1), Usol_pred{i}(j,:),'--','Linewidth', line_width)
        hold on
        if length(list_time) >  1
            plot(list_time(1:end-1), list_Usol{i}(j,:),'Linewidth', line_width)
        end
    end
    xlabel('time (s)')
    xlim([list_time(1), Tsim])
    switch j
        case 1
            ylabel('thrust acceleration')
        case 2
            ylabel('w_{theta} (rad/s)')
        otherwise
            ylabel('w_{phi} (rad/s)')
    end
end
end
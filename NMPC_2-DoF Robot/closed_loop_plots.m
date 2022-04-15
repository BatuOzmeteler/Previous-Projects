clear all
clc
%% Extract Data
load('results_3.mat')
         
dt = 0.1;

t_none = 0:dt:8;
t_pu = 0:dt:8.1;
t_mn = 0:dt:15;
t_pu_mn = 0:dt:15;

nx = 4;
nu = 2;

%% Initial Conditions
q1_0 = -5;   q2_0 = -4;
q1_d_0 = 0;  q2_d_0 = 0; 

x_init = [q1_0; q2_0; q1_d_0; q2_d_0];

%% State Goal Vector
q1_g = pi/2; q2_g = 0;
q1_d_g = 0;  q2_d_g = 0;

x_goal = [q1_g; q2_g; q1_d_g; q2_d_g];

%% Plot
figure();
sgtitle('States') 

for i = 1:nx
    subplot(2,2,i)
    hold on;
    plot(t_none, XX_none(i,:), 'Color', '#D95319', 'LineWidth', 1.5)
    plot(t_pu, XX_pu(i,:), 'k', 'LineWidth', 1.5)
    plot(t_mn, XX_mn(i,:), 'b', 'LineWidth', 1.5)
    plot(t_pu_mn, XX_pu_mn(i,:), 'r', 'LineWidth', 1.5)
    plot(t_mn(1), x_init(i), 'xg', 'LineWidth', 2)
    plot(t_mn(end), x_goal(i), 'xr', 'LineWidth', 2)
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
    legend('Nominal', 'Parametric Uncertainty', 'Measurement Noise', 'Both')
    hold off;
end

figure();
sgtitle('Inputs') 

for i = 1:nu
    subplot(2,1,i)
    hold on;
    stairs(t_none(1:end-1), UU_none(i,:), 'Color', '#D95319', 'LineWidth', 1.5)
    stairs(t_pu(1:end-1), UU_pu(i,:), 'k', 'LineWidth', 1.5)
    stairs(t_mn(1:end-1), UU_mn(i,:), 'b', 'LineWidth', 1.5)
    stairs(t_pu_mn(1:end-1), UU_pu_mn(i,:), 'r', 'LineWidth', 1.5)
    xlabel('time (s)')
    switch i
        case 1
            ylabel('$\tau_{1}$ (rad/s)', 'Interpreter', 'latex')
        case 2
            ylabel('$\tau_{2}$ (rad/s)', 'Interpreter', 'latex')
    end
    grid ON;
    legend('Nominal', 'Parametric Uncertainty', 'Measurement Noise', 'Both')
    hold off;
end

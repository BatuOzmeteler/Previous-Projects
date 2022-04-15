clear all
clc
%% Extract Data
load('results_2.mat')

N = 100;            
t = 0:1/N:1;        

nx = 4;
nu = 2;

XX_fx = Xsol_fx;
UU_fx = Usol_fx;
XX_fr = Xsol_fr;
UU_fr = Usol_fr;
XX_fr_mce = Xsol_fr_mce;
UU_fr_mce = Usol_fr_mce;

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
    plot(t, XX_fx(i,:), 'Color', '#D95319', 'LineWidth', 1.5)
    plot(t, XX_fr(i,:), 'k', 'LineWidth', 1.5)
    plot(t, XX_fr_mce(i,:), 'b', 'LineWidth', 1.5)
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
    legend('Fixed-End-Time', 'Free-End-Time', 'Free-End-Time w/ MCE')
    hold off;
end

figure();
sgtitle('Inputs') 

for i = 1:nu
    subplot(2,1,i)
    hold on;
    stairs(t(1:end-1), UU_fx(i,:), 'Color', '#D95319', 'LineWidth', 1.5)
    stairs(t(1:end-1), UU_fr(i,:), 'k', 'LineWidth', 1.5)
    stairs(t(1:end-1), UU_fr_mce(i,:), 'b', 'LineWidth', 1.5)
    xlabel('time (s)')
    switch i
        case 1
            ylabel('$\tau_{1}$ (rad/s)', 'Interpreter', 'latex')
        case 2
            ylabel('$\tau_{2}$ (rad/s)', 'Interpreter', 'latex')
    end
    grid ON;
    legend('Fixed-End-Time', 'Free-End-Time', 'Free-End-Time w/ MCE')
    hold off;
end

figure();
sgtitle('q_{1}-q_{2} Plane') 

hold on;
plot(XX_fx(1,:), XX_fx(2,:), 'Color', '#D95319', 'LineWidth', 1.5)
plot(XX_fr(1,:), XX_fr(2,:), 'k', 'LineWidth', 1.5)
plot(XX_fr_mce(1,:), XX_fr_mce(2,:), 'b', 'LineWidth', 1.5)
plot(x_init(1), x_init(2), 'xg', 'LineWidth', 2)
plot(x_goal(1), x_goal(2), 'xr', 'LineWidth', 2)
hold off;

grid ON;
legend('Fixed-End-Time', 'Free-End-Time', 'Free-End-Time w/ MCE')
xlabel('q_{1} (rad)')
ylabel('q_{2} (rad)')

figure();
sgtitle('x-y Plane') 

xy_init = forwardKinematics(x_init(1:2));
xy_goal = forwardKinematics(x_goal(1:2));

xy1 = zeros(2, size(XX_fx,2));
xy2 = zeros(2, size(XX_fr,2));
xy3 = zeros(2, size(XX_fr_mce,2));

for i = 1:size(XX_fr,2)
    xy1(:,i) = forwardKinematics(XX_fx(1:2,i));
    xy2(:,i) = forwardKinematics(XX_fr(1:2,i));
    xy3(:,i) = forwardKinematics(XX_fr_mce(1:2,i));
end

hold on;
plot(xy1(1,:), xy1(2,:), 'Color', '#D95319', 'LineWidth', 1.5)
plot(xy2(1,:), xy2(2,:), 'k', 'LineWidth', 1.5)
plot(xy3(1,:), xy3(2,:), 'b', 'LineWidth', 1.5)

plot(xy_init(1), xy_init(2), 'xg', 'LineWidth', 2)
plot(xy_goal(1), xy_goal(2), 'xr', 'LineWidth', 2)
hold off;

grid ON;
legend('Fixed-End-Time', 'Free-End-Time', 'Free-End-Time w/ MCE')
xlabel('x (m)')
ylabel('y (m)')

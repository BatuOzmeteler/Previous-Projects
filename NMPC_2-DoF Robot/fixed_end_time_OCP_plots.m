clear all
clc
%% Extract Data
load('results_1.mat')

XX_1 = Xsol_tp_heun_1;
XX_2 = Xsol_tp_heun_2;
XX_3 = Xsol_tp_rk4_1;
XX_4 = Xsol_tp_rk4_2;
XX_5 = Xsol_tc_heun_1;
XX_6 = Xsol_tc_heun_2;
XX_7 = Xsol_tc_rk4_1;
XX_8 = Xsol_tc_rk4_2;

XXX = {XX_1, XX_2, XX_3, XX_4, XX_5, XX_6, XX_7, XX_8};

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
sgtitle('q_{1}-q_{2} Plane') 

hold on;
plot(XXX{1}(1,:), XXX{1}(2,:), 'c', 'LineWidth', 1.5)
plot(XXX{2}(1,:), XXX{2}(2,:), 'b', 'LineWidth', 1.5)
plot(XXX{3}(1,:), XXX{3}(2,:), 'y', 'LineWidth', 1.5)
plot(XXX{4}(1,:), XXX{4}(2,:), 'r', 'LineWidth', 1.5)
plot(x_init(1), x_init(2), 'xg', 'LineWidth', 2)
plot(x_goal(1), x_goal(2), 'xr', 'LineWidth', 2)
hold off;

grid ON;
legend('tp, heun, 0.05', 'tp, heun, 0.10', 'tp, rk4, 0.05', 'tp, rk4, 0.10')
xlabel('q_{1} (rad)')
ylabel('q_{2} (rad)')

figure();
sgtitle('q_{1}-q_{2} Plane') 

hold on;
plot(XXX{5}(1,:), XXX{5}(2,:), 'c', 'LineWidth', 1.5)
plot(XXX{6}(1,:), XXX{6}(2,:), 'b', 'LineWidth', 1.5)
plot(XXX{7}(1,:), XXX{7}(2,:), 'y', 'LineWidth', 1.5)
plot(XXX{8}(1,:), XXX{8}(2,:), 'r', 'LineWidth', 1.5)
plot(x_init(1), x_init(2), 'xg', 'LineWidth', 2)
plot(x_goal(1), x_goal(2), 'xr', 'LineWidth', 2)
hold off;

grid ON;
legend('tc, heun, 0.05', 'tc, heun, 0.10', 'tc, rk4, 0.05', 'tc, rk4, 0.10')
xlabel('q_{1} (rad)')
ylabel('q_{2} (rad)')

xy_init = forwardKinematics(x_init(1:2));
xy_goal = forwardKinematics(x_goal(1:2));

XYY = cell(1, 8);

for i = 1:size(XXX,2)
    XYY{i} = zeros(2, size(XXX{i},2));
    for j = 1:size(XXX{i},2)
        XYY{i}(:,j) = forwardKinematics(XXX{i}(1:2,j));
    end
end

figure();
sgtitle('x-y Plane') 

hold on;
plot(XYY{1}(1,:), XYY{1}(2,:), 'c', 'LineWidth', 1.5)
plot(XYY{2}(1,:), XYY{2}(2,:), 'b', 'LineWidth', 1.5)
plot(XYY{3}(1,:), XYY{3}(2,:), 'y', 'LineWidth', 1.5)
plot(XYY{4}(1,:), XYY{4}(2,:), 'r', 'LineWidth', 1.5)
plot(xy_init(1), xy_init(2), 'xg', 'LineWidth', 2)
plot(xy_goal(1), xy_goal(2), 'xr', 'LineWidth', 2)
hold off;

grid ON;
legend('tp, heun, 0.05', 'tp, heun, 0.10', 'tp, rk4, 0.05', 'tp, rk4, 0.10')
xlabel('x (m)')
ylabel('y (m)')

figure();
sgtitle('x-y Plane') 

hold on;
plot(XYY{5}(1,:), XYY{5}(2,:), 'c', 'LineWidth', 1.5)
plot(XYY{6}(1,:), XYY{6}(2,:), 'b', 'LineWidth', 1.5)
plot(XYY{7}(1,:), XYY{7}(2,:), 'y', 'LineWidth', 1.5)
plot(XYY{8}(1,:), XYY{8}(2,:), 'r', 'LineWidth', 1.5)
plot(xy_init(1), xy_init(2), 'xg', 'LineWidth', 2)
plot(xy_goal(1), xy_goal(2), 'xr', 'LineWidth', 2)
hold off;

grid ON;
legend('tc, heun, 0.05', 'tc, heun, 0.10', 'tc, rk4, 0.05', 'tc, rk4, 0.10')
xlabel('x (m)')
ylabel('y (m)')


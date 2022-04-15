function plot_rpm_power(xx, u_cl, t, T, plot_rpm, plot_mechPower)

set(0,'DefaultAxesFontName', 'Times New Roman')
set(0,'DefaultAxesFontSize', 12)

line_width = 1.5;
fontsize_labels = 14;
u_cl(length(t)+1) = u_cl(end);
t(length(t)+1) = t(end) + T;

mechPower_arr = plot_mechPower(xx',u_cl);
rpm_arr = plot_rpm(xx);

mech_lim = full(max(mechPower_arr)) + 100;
rpm_lim  = full(max(rpm_arr)) + 1000;

subplot(2,1,1); plot(t, full(mechPower_arr), '-b','linewidth',line_width);

p1_title = title('Mechanical Power in Time');
set(p1_title,'fontsize',14,'fontweight','bold');
ylabel('horsepower (hP)', 'FontSize',fontsize_labels);
xlabel('time (s)', 'FontSize',fontsize_labels);
axis([0 t(end) 0 mech_lim]);
grid on

subplot(2,1,2); plot(t, full(rpm_arr), '-r','linewidth',line_width);

p2_title = title('RPM in Time');
set(p2_title,'fontsize',14,'fontweight','bold');
ylabel('rpm (rev/min)', 'FontSize',fontsize_labels);
xlabel('time (s)', 'FontSize',fontsize_labels);
axis([0 t(end) 0 rpm_lim]);
grid on

end
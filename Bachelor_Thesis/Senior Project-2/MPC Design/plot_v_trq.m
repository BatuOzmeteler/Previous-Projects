function plot_v_trq(xx, u_cl, t, trq_max, xs, T, N)

set(0,'DefaultAxesFontName', 'Times New Roman')
set(0,'DefaultAxesFontSize', 12)

line_width = 1.5;
fontsize_labels = 14;
t_plot = t;
u_plot = u_cl;
t_plot(length(t)+1) = t(end) + T;
u_plot(length(t)+1) = u_cl(end);

subplot(2,1,1); plot(t_plot, xx, '-b','linewidth',line_width);

p_title = title(sprintf('Reference Speed: %u m/s - Sample Time: %.2f s - Prediction Horizon: %u \n',xs, T, N));
set(p_title,'fontsize',14,'fontweight','bold');
set(gcf, 'Position',  [850, 300, 600, 475])
ylabel('velocity (m/s)', 'FontSize',fontsize_labels);
xlabel('time (s)', 'FontSize',fontsize_labels);
axis([0 t_plot(end) 0 xs+10]);
grid on

subplot(2,1,2); plot(t_plot, u_plot, '-r','linewidth',line_width);
ylabel('torque (N.m)', 'FontSize',fontsize_labels);
xlabel('time (s)', 'FontSize',fontsize_labels);
axis([0 t_plot(end) 0 trq_max+50]);
grid on

end
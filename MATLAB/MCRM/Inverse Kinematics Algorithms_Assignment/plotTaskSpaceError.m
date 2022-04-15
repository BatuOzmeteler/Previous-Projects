function [ ] = plotTaskSpaceError( t, tformx, tformxd)
% t : time index
% tformx : end effector motion
% tformxd : desired end effector motion
% e : generalized pose and orientation error 
subplot(321);
plot(t,tform2trvec(tformxd));
xlabel('t');
ylabel('x_d');
legend('x_{d_x}','x_{d_y}','x_{d_z}','Location','northeast');
title('reference position');
subplot(322);
plot(t,tform2quat(tformxd));
xlabel('t')
ylabel('\eta_d \epsilon_{x_d} \epsilon_{y_d}  \epsilon_{z_d}')
legend('\eta', '\epsilon_x', '\epsilon_y', '\epsilon_z','Location','southeast');
title('reference orientation');

subplot(323);
plot(t,tform2trvec(tformx));
xlabel('t')
ylabel('x');
legend('x_x','x_y','x_z','Location','northeast');
title('robot position');
subplot(324);
plot(t,tform2quat(tformx));
xlabel('t')
ylabel('\eta \epsilon_x \epsilon_y  \epsilon_z')
legend('\eta', '\epsilon_x', '\epsilon_y', '\epsilon_z','Location','southeast');
title('robot orientation');


% calculate tform of error
for k=1:length(t)
    tforme(:,:,k)=tformx(:,:,k)\tformxd(:,:,k);
end

subplot(325);
plot(t,tform2trvec(tforme));

xlabel('t');
ylabel('e_p');
legend('e_x','e_y','e_z','Location','northeast');
title('position error');
subplot(326);

plot(t,tform2quat(tforme));
xlabel('t')
ylabel('\eta \epsilon_x \epsilon_y  \epsilon_z')
legend('\eta', '\epsilon_x', '\epsilon_y', '\epsilon_z','Location','southeast');

title('orientation error')

end


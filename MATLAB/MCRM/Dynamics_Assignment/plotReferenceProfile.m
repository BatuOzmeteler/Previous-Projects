function plotReferenceProfile(tsteps,qref,dqref,ddqref)
njoints=size(qref,2);
subplot(311);
plot(tsteps,qref(:,:));
xlabel('t');
ylabel('\vartheta_i');
for i=1:njoints
    legarray{i}=['$$\vartheta_', num2str(i), '$$'];
end
leg=legend('show');
leg.Interpreter='latex';
leg.String=legarray;
title('joint states');
subplot(312);
plot(tsteps,dqref(:,:));
xlabel('t');
ylabel('$$\dot \vartheta_i$$','Interpreter','latex');
leg=legend('show');
leg.Interpreter='latex';
for i=1:njoints
    legarray{i}=['$$\dot \vartheta_', num2str(i), '$$'];
end
leg.String=legarray;
title('joint velocities');
subplot(313);
plot(tsteps,ddqref(:,:));
xlabel('t');
ylabel('$$\ddot \vartheta_i$$','Interpreter','latex');
for i=1:njoints
    legarray{i}=['$$\ddot \vartheta_', num2str(i), '$$'];
end
leg=legend('show');
leg.Interpreter='latex';
leg.String=legarray;
title('joint accelerations');
end


function [] = plotJointMotionTracking( t, q, qdot )
% t : time index
% q : joint states
% qdot : commanded joint velocities

qdotmotion=(q(:,2:end)-q(:,1:end-1))/(t(end)/length(t));
clf;
subplot(3,1,1);
plot(t(1:end-1),qdotmotion);
title('actual joint velocity');
ylabel('q(t)');
xlabel('t');

if size(q,1)==7 
    legend('q_1','q_2','q_3','q_4','q_5','q_6','q_7');
else
    legend('q_1','q_2','q_3','q_4','q_5','q_6');
end

subplot(312);

plot(t,qdot);
title('commanded joint velocities');
ylabel('dq(t)/dt');
xlabel('t');

if size(q,1)==7 
    legend('q_1','q_2','q_3','q_4','q_5','q_6','q_7');
else
    legend('q_1','q_2','q_3','q_4','q_5','q_6');
end

subplot(313);

plot(t(1:end-1),qdot(:,1:end-1)-qdotmotion);
title('error joint velocities');
ylabel('dq(t)/dt');
xlabel('t');

if size(q,1)==7 
    legend('q_1','q_2','q_3','q_4','q_5','q_6','q_7');
else
    legend('q_1','q_2','q_3','q_4','q_5','q_6');
end
end


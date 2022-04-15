function [] = plotJointMotion( t, q, qdot, mode, w, c, qdotnull )
% t : time index
% q : joint states
% qdot : joint velocities
% qdotnull : null space joint velocities
% w : volume manipulation ellipsoid
% c : condition number Jacobian

if (nargin<6)
    qdotnull=zeros(size(qdot));
end

clf;
subplot(3,2,[1,2]);
plot(t,q);
title('joint motion');
ylabel('q(t)');
xlabel('t');

if size(q,1)==7 
    legend('q_1','q_2','q_3','q_4','q_5','q_6','q_7');
else
    legend('q_1','q_2','q_3','q_4','q_5','q_6');
end

if isequal(mode, 'invKinRedundant')
  subplot(323);
else
  subplot(3,2,[3,4]);
end
plot(t,qdot);
title('joint velocities');
ylabel('dq(t)/dt');
xlabel('t');

if size(q,1)==7 
    legend('q_1','q_2','q_3','q_4','q_5','q_6','q_7');
else
    legend('q_1','q_2','q_3','q_4','q_5','q_6');
end

if isequal(mode, 'invKinRedundant')
    subplot(324);
    plot(t,qdotnull);
    title('null space joint velocities');
    ylabel('dq0(t)/dt');
    xlabel('t');
    if size(q,1)==7
        legend('q_1','q_2','q_3','q_4','q_5','q_6','q_7');
    else
        legend('q_1','q_2','q_3','q_4','q_5','q_6');
    end
end
subplot(325);
plot(t(1:end-1),w(1:end-1));
title('volume manipulability ellipsoid');
ylabel('w(q(t))');
xlabel('t');
subplot(326);
plot(t(1:end-1),c(1:end-1));
title('condition number');
ylabel('cond(J(q(t))');
xlabel('t');
end


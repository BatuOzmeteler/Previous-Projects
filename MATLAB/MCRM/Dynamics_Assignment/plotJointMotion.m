function [] = plotJointMotion( t, q, titletext, tsteps, qref, dqref)
% t : time index
% q : joint states [q1 q2 dotq1 dotq2]

if (nargin  <3)
    titletext='Motion of two link planar arm';
end

clf;
subplot(211)
plot(t,q(:,1:2));
if (nargin == 6)
    hold on;
    plot(tsteps,qref);
    hold off;
end
xlabel('t');
ylabel('$\vartheta_i$','Interpreter','latex');
leg=legend('show');
leg.Interpreter='latex';
if (nargin == 6)
    leg.String={'$$\vartheta_1$$', '$$\vartheta_2$$','$$\vartheta_{1r}$$', '$$\vartheta_{2r}$$'};
else
    leg.String={'$$\vartheta_1$$', '$$\vartheta_2$$'};
end
title(titletext);
subplot(212)
plot(t,q(:,3:4));
if (nargin == 6)
    hold on;
    plot(tsteps,dqref);
    hold off;
end
xlabel('t');
ylabel('$$\dot \vartheta_i$$','Interpreter','latex');
leg=legend('show');
leg.Interpreter='latex';
if (nargin == 6)
    leg.String={'$$\dot \vartheta_1$$', '$$\dot \vartheta_2$$','$$\dot \vartheta_{1r}$$', '$$\dot \vartheta_{2r}$$',};
else
    leg.String={'$$\dot \vartheta_1$$', '$$\dot \vartheta_2$$'};
end


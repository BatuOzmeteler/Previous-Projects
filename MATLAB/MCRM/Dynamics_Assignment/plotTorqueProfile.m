function plotTorqueProfile(tsteps, tau, taui, taug, tauc, imposetau)

if nargin<6
    imposetau=true;
end

if (imposetau)
    subplot(411);
    plot(tsteps,[tau]);
    xlabel('t');
    ylabel('$\tau_i$','Interpreter','latex');
    leg=legend('show');
    leg.Interpreter='latex';
    leg.String={'$$\tau_1$$', '$$\tau_2$$'};
    title('total torque profile');
    
    subplot(412);
    plot(tsteps,[tau taug]);
    xlabel('t');
    ylabel('$\tau_i$','Interpreter','latex');
    leg=legend('show');
    leg.Interpreter='latex';
    leg.String={'$$\tau_1$$', '$$\tau_2$$','$$\tau_{g1}$$', '$$\tau_{g2}$$'};
    title('total torque vs gravity torque profile');
    subplot(413);
    plot(tsteps,[tau taui]);
    xlabel('t');
    ylabel('$\tau_i$','Interpreter','latex');
    leg=legend('show');
    leg.Interpreter='latex';
    leg.String={'$$\tau_1$$', '$$\tau_2$$','$$\tau_{i1}$$', '$$\tau_{i2}$$'};
    title('total torque vs. inertia torque profile');
    subplot(414);
    % plot(tsteps,[tau tauc]);
    plot(tsteps,[tauc]);
    
    xlabel('t');
    ylabel('$\tau_i$','Interpreter','latex');
    leg=legend('show');
    leg.Interpreter='latex';
    % leg.String={'$$\tau_1$$', '$$\tau_2$$','$$\tau_{c1}$$', '$$\tau_{c2}$$'};
    % title('total torque vs. coriolis/centrifugal torque profile');
    leg.String={'$$\tau_{c1}$$', '$$\tau_{c2}$$'};
    title('coriolis/centrifugal torque profile');
else
    subplot(411);
    plot(tsteps,[tau]);
    xlabel('t');
    ylabel('$\tau_i$','Interpreter','latex');
    leg=legend('show');
    leg.Interpreter='latex';
    leg.String={'$$\tau_1$$', '$$\tau_2$$'};
    title('total torque profile');
    
    subplot(412);
    plot(tsteps,[taug]);
    xlabel('t');
    ylabel('$\tau_i$','Interpreter','latex');
    leg=legend('show');
    leg.Interpreter='latex';
    leg.String={'$$\tau_{g1}$$', '$$\tau_{g2}$$'};
    title('gravity torque profile');
    subplot(413);
    plot(tsteps,[taui]);
    xlabel('t');
    ylabel('$\tau_i$','Interpreter','latex');
    leg=legend('show');
    leg.Interpreter='latex';
    leg.String={'$$\tau_{i1}$$', '$$\tau_{i2}$$'};
    title('inertia torque profile');
    subplot(414);
    % plot(tsteps,[tau tauc]);
    plot(tsteps,[tauc]);
    
    xlabel('t');
    ylabel('$\tau_i$','Interpreter','latex');
    leg=legend('show');
    leg.Interpreter='latex';
    % leg.String={'$$\tau_1$$', '$$\tau_2$$','$$\tau_{c1}$$', '$$\tau_{c2}$$'};
    % title('total torque vs. coriolis/centrifugal torque profile');
    leg.String={'$$\tau_{c1}$$', '$$\tau_{c2}$$'};
    title('coriolis/centrifugal torque profile');
end

end


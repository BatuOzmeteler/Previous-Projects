%-------------------------------------------------------------------------%
% Case Study A - PDE Based Motion Planning for Robots
% Author: Mehmet Batu Özmeteler, 230306

% The Benchmark Example
%-------------------------------------------------------------------------%

function two_links_manipulator_1

tmax    = 4;                                  % integration time for PDE
tpoints = 250;
xpoints = 250;

m = 0;
x = linspace(0, 1, xpoints);                  % discretization of the curve
t = [0 logspace(-4, log10(tmax), tpoints-1)]; % discretization of time interval

% Solve GHF
sol = pdepe(m, @mypdexpde, @mypdexic, @mypdexbc, x, t);  

% The solution is of the form sol(t,x,i)
state_x   = sol(:,:,1);    % x-position
state_y   = sol(:,:,2);    % y-position
state_th1 = sol(:,:,3);    % theta1-angle
state_th2 = sol(:,:,4);    % theta2-angle

% Homotopy plot in x_y-plane (no obstacles included)
figure_xy = figure;       
X   = state_x(1,:);
Y   = state_y(1,:);

init_xy = plot(X, Y, 'k', 'LineWidth', 2);
axis([0.7,0.8,0.2,1.8]);
grid ON;
title('Configuration Space Curve - (xy-plane)');
xlabel('x-position');
ylabel('y-position');
hold on;
pause;

for i = 1:tpoints 
    X = state_x(i,:); init_xy.XDataSource = 'X';
    Y = state_y(i,:); init_xy.YDataSource = 'Y';
    
    refreshdata(init_xy,'caller');
    drawnow;
end

% Homotopy plot in th1_th2-plane (no obstacles included)
figure_th1th2 = figure;    
TH1   = state_th1(1,:);
TH2   = state_th2(1,:);

init_th1th2 = plot(TH1, TH2, 'k', 'LineWidth', 2);
axis([1.55,1.85,-0.8,0.8]);
grid ON;
title('Configuration Space Curve - (\theta_{1}\theta_{2}-plane)');
xlabel('\theta_{1}-angle');
ylabel('\theta_{2}-angle');
hold on;
pause;

for i = 1:tpoints 
    TH1 = state_th1(i,:); init_th1th2.XDataSource = 'TH1';
    TH2 = state_th2(i,:); init_th1th2.YDataSource = 'TH2';
    
    refreshdata(init_th1th2,'caller');
    drawnow;
end

% Extract controls
u = zeros(2, xpoints);
[p1, dp1] = pdeval(m, x, sol(end, :, 1), x);
[p2, dp2] = pdeval(m, x, sol(end, :, 2), x);
[p3, dp3] = pdeval(m, x, sol(end, :, 3), x);
[p4, dp4] = pdeval(m, x, sol(end, :, 4), x);

for i = 1:xpoints
    Ff = [-sin(p3(i)), -sin(p4(i));
           cos(p3(i)),  cos(p4(i));
                    1,           0;
                    0,           1];

    Ff_pinv = pinv(Ff);
    u(:,i)= Ff_pinv*[dp1(i);dp2(i);dp3(i);dp4(i)];
end

% Find x^* by integrating the ODE
xstar       =   zeros(4, xpoints);
xstar(1,1)  =   p1(1); 
xstar(2,1)  =   p2(1);
xstar(3,1)  =   p3(1);
xstar(4,1)  =   p4(1);

for i = 1:(xpoints-1)
    Ff = [-sin(p3(i)), -sin(p4(i));
           cos(p3(i)),  cos(p4(i));
                    1,           0;
                    0,           1];
    
    xstar(:,i+1) = xstar(:,i) + Ff*u(:,i)*(x(i+1)-x(i));
end

% Plot target path vs actual path

figure_star1 = figure;

x_cl = xstar(1,:); y_cl = xstar(2,:); 
hold on;
plot(state_x(end,:), state_y(end,:), 'k', 'LineWidth', 1.5);
plot(x_cl(end,:), y_cl(end,:), 'LineWidth', 1.5);
hold off;

axis([0.7,0.8,0.2,1.8]);
grid ON;
title('Target Path vs Actual Path - (xy-plane)');
xlabel('x-position');
ylabel('y-position');
legend('Target Path','Actual Path')

figure_star2 = figure;

th1_cl = xstar(3,:); th2_cl = xstar(4,:);
hold on;
plot(state_th1(end,:), state_th2(end,:), 'k', 'LineWidth', 1.5); 
plot(th1_cl(end,:), th2_cl(end,:), 'LineWidth', 1.5);
hold off;

axis([1.55,1.85,-0.8,0.8]);
grid ON;
title('Target Path vs Actual Path - (\theta_{1}\theta_{2}-plane)');
xlabel('\theta_{1}-angle');
ylabel('\theta_{2}-angle');
legend('Target Path','Actual Path')

% Average error calculation
sol_x_end = [state_x(end,:); state_y(end,:); state_th1(end,:); state_th2(end,:)];

for xp = 1:xpoints
errors = abs(xstar(xp) - sol_x_end(:, xp));
end
xe = sum(errors(1,:))/xpoints;
ye = sum(errors(2,:))/xpoints;
th1e = sum(errors(3,:))/xpoints;
th2e = sum(errors(4,:))/xpoints;

fprintf('Average Error \nx: %i \ny: %i \nth1: %i \nth2: %i \n', xe, ye, th1e, th2e)

end

% ------------------------------------------------------------------------%

% The following functions are for the formulation of the PDE, the initial 
% condition and the boundary condition for the MATLAB command pdepe:

function [c, f, s] = mypdexpde(x, t, u, DuDx)          % define PDE, evaluate the RHS of GHF

k = 300;                                               % penalty in the constrained directions

% Metric
G = [       (sin(u(3))^2 + sin(u(4))^2 + k), ((-sin(2*u(3))/2) + (-sin(2*u(4))/2)), ((k-1)*sin(u(3))) ,  ((k-1)*sin(u(4)));
      ((-sin(2*u(3))/2) + (-sin(2*u(4))/2)),      ( cos(u(3))^2 + cos(u(4))^2 + k), -((k-1)*cos(u(3))), -((k-1)*cos(u(4)));
                          ((k-1)*sin(u(3))),                    -((k-1)*cos(u(3))),              (k+1), (k*cos(u(3)-u(4)));
                          ((k-1)*sin(u(4))),                    -((k-1)*cos(u(4))), (k*cos(u(3)-u(4))),             (k+1)];

  
% Partial derivatives of G
pG(:,:,1) = zeros(4);
pG(:,:,2) = zeros(4);

pG(:,:,3) = [ 2*cos(u(3))*sin(u(3)),           -cos(2*u(3)),   cos(u(3))*(k - 1),                   0;
                       -cos(2*u(3)), -2*cos(u(3))*sin(u(3)),   sin(u(3))*(k - 1),                   0;
                  cos(u(3))*(k - 1),      sin(u(3))*(k - 1),                   0, -k*sin(u(3) - u(4));
                                  0,                      0, -k*sin(u(3) - u(4)),                  0];

pG(:,:,4) = [ 2*cos(u(4))*sin(u(4)),           -cos(2*u(4)),                  0,  cos(u(4))*(k - 1);
                       -cos(2*u(4)), -2*cos(u(4))*sin(u(4)),                  0,  sin(u(4))*(k - 1);
                                  0,                      0,                  0, k*sin(u(3) - u(4));
                  cos(u(4))*(k - 1),      sin(u(4))*(k - 1), k*sin(u(3) - u(4)),                 0];

% Inverse of G
invG = inv(G);

% Evaluate Christoffels symbols
christoffel_sym = zeros(size(pG));
for i = 1:4
    for j = 1:4
        for k = 1:4
            for l = 1:4
                christoffel_sym(i,j,k) = christoffel_sym(i,j,k) + 0.5*invG(i,l)*(pG(l,j,k) + pG(l,k,j) - pG(j,k,l));
            end
        end
    end
end
c = [1; 1; 1; 1];
f = DuDx;
s = DuDx'*squeeze(christoffel_sym(1,:,:))*DuDx*[1;0;0;0] + ...
    DuDx'*squeeze(christoffel_sym(2,:,:))*DuDx*[0;1;0;0] + ...
    DuDx'*squeeze(christoffel_sym(3,:,:))*DuDx*[0;0;1;0] + ...
    DuDx'*squeeze(christoffel_sym(4,:,:))*DuDx*[0;0;0;1];

end

% ------------------------------------------------------------------------%
 
function u0 = mypdexic(x)                              % initial condition for GHF

% Setting an initial curve which doesn't respect the constraints
% q1 and q2 but respects the boundary conditions

u0 = [ sqrt(2)/2; ( (1 - (sqrt(2)/2)) + (2*x*(sqrt(2)/2)) ); pi/2; ( (-pi/4) + (2*x*pi/4) ) ];             

end

% ------------------------------------------------------------------------%

function [pl, ql, pr, qr] = mypdexbc(xl, ul, xr, ur, t) % boundary condition for GHF

% L1 and L2 are assumed to be 1

pl = ul - [ sqrt(2)/2 ; (1 - (sqrt(2)/2)); pi/2; -pi/4 ];
ql = [0; 0; 0; 0];
pr = ur - [ sqrt(2)/2 ; (1 + (sqrt(2)/2)); pi/2;  pi/4 ];
qr = [0; 0; 0; 0];

end

% ------------------------------------------------------------------------%

% A Homotopy method for Motion Planning
% A unicycle example wtih two point obstacles
% M.-A. Belabbas and S. Liu
%
% Passing initial condition and boundary condition to the PDE
% Solve PDE
% The solution at tmax approximates the steady state solution
% Extract the controls from sol(tpoints,:,:)
% Simulate the motion of unicycle using extracted controls
%

function unicycle_with_obstacle

tmax=4;                                     % Integration time for PDE
tpoints=250;
xpoints=250;

m = 0;
x = linspace(0,1,xpoints);                  % discretization of the curve
t = [0 logspace(-4,log10(tmax),tpoints-1)]; % discretization of time interval
                                            % logscale to see what take
                                            % place for t small

global z1 z2 rr                             % position of the obstacles in R2, rr= radius of obstacles

z1=[0.5;0];z2=[-0.5;0];
rr=.03;

sol = pdepe(m,@mypdexpde,@mypdexic,@mypdexbc,x,t);  % Solve GHF
                                                    % The solution is of form sol(t,x,i)
u1 = sol(:,:,1);    %x position
u2 = sol(:,:,2);    %y position
u3 = sol(:,:,3);    % angle theta

 figure                                     % 3d homotopy plot without plotting obstacles

 X=u1(1,:);
 Y=u2(1,:);
 Z=u3(1,:);
    h1=plot3(X,Y,Z,'k','LineWidth',2);
    axis([-1.5,1.5,-1.5,1.5,-1.5,1.5]);
    grid ON;
    title('3D configuration space curve');
    xlabel('x');
    ylabel('y');
    zlabel('\theta');
    hold on;
    pause;
    
for i=1:tpoints 
    X=u1(i,:);h1.XDataSource='X';
    Y=u2(i,:);h1.YDataSource='Y';
    Z=u3(i,:);h1.ZDataSource='Z';
    
    refreshdata(h1,'caller');
    drawnow;
end

figure                                      % Show homotopy in 2D with obstacles

X=u1(1,:);
Y=u2(1,:);
    h1=plot(X,Y,'k','LineWidth',2);
    hold on;
    j=0:0.1:2*pi;
    plot(z1(1)+rr*cos(j),z1(2)+rr*sin(j),'r:');
    plot(z2(1)+rr*cos(j),z2(2)+rr*sin(j),'r:');
    scatter(z1(1),z1(2),'r','filled');
    scatter(z2(1),z2(2),'r','filled');
    axis([-1.5,1.5,-1.0,1.0]);
    grid ON;
    title('xy-plane trajectory and unicycle simulation');
    xlabel('x');
    ylabel('y');
    pause;
   
for i=1:tpoints
    X=u1(i,:);h1.XDataSource='X';
    Y=u2(i,:);h1.YDataSource='Y';
    
    refreshdata(h1,'caller');
    drawnow;
      
end


% Extract Controls


bu = zeros(2,xpoints);
[p1 dp1]=pdeval(m,x,sol(end,:,1),x);
[p2 dp2]=pdeval(m,x,sol(end,:,2),x);
[p3 dp3]=pdeval(m,x,sol(end,:,3),x);

for i = 1 : xpoints
    projB = [cos(p3(i)) sin(p3(i)) 0;
        0 0 1];
    bu(:,i)= projB*[dp1(i);dp2(i);dp3(i)];
end


%Find x^* by integrating ODE

xstar   =   zeros(3,xpoints);
xstar(1,1)  =   p1(1); %Initial state
xstar(2,1)  =   p2(1);
xstar(3,2)  =   p3(1);

for i =1: xpoints-1
    cB = [cos(xstar(3,i)) 0;
        sin(xstar(3,i)) 0;
        0 1];
%      cB = [cos(p3(i)) 0;
%         sin(p3(i)) 0;
%         0 1]; %Sanity check: this cB should yield xstar equal to p up to
%         numerical error
    xstar(:,i+1) = xstar(:,i) + cB*bu(:,i)*(x(i+1)-x(i));
end


% Plot the trajectory x^*
% Negligible error so omitted

% Drawing of unicycle

w=0.3; %width
l=0.1; %height
R=[-w/2 w/2 w/2 -w/2 -w/2; -l/2 -l/2 l/2 l/2 -l/2];


xc=xstar(1,1);yc=xstar(2,1);theta=xstar(3,1);
XY=[cos(theta) -sin(theta);sin(theta) cos(theta)]*R;    %rotate angle alpha
h=patch(XY(1,:)+xc,XY(2,:)+yc,[0.4,0.4,1]);

 plot(z1(1)+rr*cos(j),z1(2)+rr*sin(j),'r:');
    plot(z2(1)+rr*cos(j),z2(2)+rr*sin(j),'r:');
    scatter(z1(1),z1(2),'r','filled');
    scatter(z2(1),z2(2),'r','filled');
    
hold on;
% axis([-0.5,1.5,-0.5,1.5]);
h2=quiver(xc,yc,cos(theta),sin(theta),'r');
set(h2,'AutoScaleFactor',0.15,'MaxHeadSize',2)
grid on
pause();

% Show unicycle simulation
for i=1:xpoints;
    xc=sol(tpoints,i,1);yc=sol(tpoints,i,2);
    xc=xstar(1,i);yc=xstar(2,i);theta=xstar(3,i);
    R=[-w/2 w/2 w/2 -w/2 -w/2; -l/2 -l/2 l/2 l/2 -l/2];
    XY=[cos(theta) -sin(theta);sin(theta) cos(theta)]*R;
    h.XData=XY(1,:)+xc;h.YData=XY(2,:)+yc;
    h2.XData=xc;h2.YData=yc;h2.UData=cos(theta);h2.VData=sin(theta);
    refreshdata(h);
    refreshdata(h2);
    drawnow
    
end

end
% --------------------------------------------------------------------------

% The followings are the PDE, initial condition and boundary condition for
% the Matlab commend pdepe:


function [c,f,s] = mypdexpde(x,t,u,DuDx)    % Define PDE; Evaluate right-hand-side of GHF


k=180;                                      % Penalty in the constrained directions


global z1 z2 rr
R = rr+.15;                                 % Safety zone around obstacle
l1=norm(u(1:2)-z1);l2=norm(u(1:2)-z2);      % Barrier function lambda


if l1>=R
    lambda1=0;
    plambda1=[0;0;0];
else
    lambda1=((l1^2-R^2)/l1^2)^2+1;
    plambda1=4*R^2*(l1^2-R^2)/l1^6*[u(1)-z1(1);u(2)-z1(2);0];
end

if l2>=R
    lambda2=0;
    plambda2=[0;0;0];
else
    lambda2=((l2^2-R^2)/l2^2)^2+1;
    plambda2=4*R^2*(l2^2-R^2)/l2^6*[u(1)-z2(1);u(2)-z2(2);0];
end

lambda=lambda1+lambda2+1;
plambda=plambda1+plambda2;

% Metric
G=[cos(u(3))^2+k*sin(u(3))^2,(1-k)*sin(u(3))*cos(u(3)),0;
   (1-k)*sin(u(3))*cos(u(3)),sin(u(3))^2+k*cos(u(3))^2,0;
   0, 0, 1];
% Partial derivatives of G
pG=zeros(3);
pG(:,:,2)=zeros(3);
pG(:,:,3)=[(k-1)*sin(2*u(3)),(1-k)*cos(2*u(3)),0;
           (1-k)*cos(2*u(3)),(1-k)*sin(2*u(3)),0;
           0,0,0];
       
pG(:,:,1)=plambda(1)*G;
pG(:,:,2)=plambda(2)*G;
pG(:,:,3)=lambda*pG(:,:,3);
       
invG=inv(lambda*G);
% Evaluate christoffels symboles
Chris=zeros(size(pG));
for i=1:3
    for j=1:3
        for k=1:3
            for l=1:3
                Chris(i,j,k)=Chris(i,j,k)+0.5*invG(i,l)*(pG(l,j,k)+pG(l,k,j)-pG(j,k,l));
            end
        end
    end
end
c = [1;1;1];
f = DuDx;
s = DuDx'*squeeze(Chris(1,:,:))*DuDx*[1;0;0]+DuDx'*squeeze(Chris(2,:,:))*DuDx*[0;1;0]+DuDx'*squeeze(Chris(3,:,:))*DuDx*[0;0;1];
end
% --------------------------------------------------------------------------
 
function u0 = mypdexic(x)                           % Initial condition for GHF

u0=[2*x-1;1*sin(2*pi*x);0];

end
% --------------------------------------------------------------------------

function [pl,ql,pr,qr] = mypdexbc(xl,ul,xr,ur,t)    % Boundary condition for GHF

pl = ul-[-1;0;0];
ql = [0;0;0];
pr = ur-[1;0;0];
qr = [0;0;0];
end
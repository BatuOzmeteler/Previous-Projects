function [tsteps, qref, dqref, ddqref] = jointReferenceProfile(qinitial,qfinal,tfinal,dt,type)

if nargin<3
    tfinal=10.0;
end

if nargin<4
    dt=0.1;
end

if nargin<5
    type='cubic'
end

njoints=size(qfinal,2);
nsteps=tfinal/dt;
tsteps=linspace(0,tfinal,nsteps);
ddqref=zeros(nsteps,njoints);

% triangular acceleration profile
if strcmp(type,'cubic')
     % total distance x = a_max t^2/8
        % a_max = 8*x/t^2
    ddqmax=8*(qfinal-qinitial)/tfinal^2;
    for i=1:njoints % linspace is not vectorized
        ddqref(1:nsteps/4,i)=linspace(0,ddqmax(i),nsteps/4);
        ddqref(nsteps/4+1:nsteps/2,i)=linspace(ddqmax(i),0,nsteps/4);
        ddqref(nsteps/2+1:3*nsteps/4,i)=linspace(0,-ddqmax(i),nsteps/4);
        ddqref(3*nsteps/4+1:nsteps,i)=linspace(-ddqmax(i),0,nsteps/4);
    end
end

if strcmp(type,'quadratic')
    % total distance x = a_max t^2/2
    % a_max = 2*x/t^2
    ddqmax=4*(qfinal-qinitial)/tfinal^2;
    ddqref(1:nsteps/2,:)=ones(nsteps/2,1)*ddqmax;
    ddqref(nsteps/2+1:nsteps,:)=-ones(nsteps/2,1)*ddqmax;
end

dqref=cumsum(ddqref,1)*dt;
qref=qinitial+cumsum(dqref,1)*dt;

end
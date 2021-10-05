classdef Path
    properties
        numClothoids {mustBeInteger}
        initialPose {mustBeNumeric}
        s {mustBeNumeric}  % total path length of i-th clothoid
        cums {mustBeNumeric} % cumulative path length
        poses {mustBeNumeric}   % final pose of i-th clothoid
        kappa0 {mustBeNumeric} % initial curvature of i-th clothoid
        dkappa0 {mustBeNumeric} % initial rate of change of curvature of i-th clothoid  kappa=kappa0+dkappa0*s;
    end
    
    methods
        function obj=Path(s,kappa0,dkappa0,initialPose)
            
            obj.numClothoids=length(s);
            obj.s=s;
            obj.cums=cumsum(s);
            obj.kappa0=kappa0;
            
            if nargin < 3
                obj.dkappa0=zeros(1,obj.numClothoids); % inital pose of path
            else
                obj.dkappa0=dkappa0;
            end
            
            if nargin < 4
                obj.initialPose=[0, 0, 0]; % inital pose of path
            else
                obj.initialPose=initialPose;
            end
            
            theta=obj.initialPose(3);
            z = matlabshared.tracking.internal.scenario.fresnelg(obj.s(1),obj.dkappa0(1),obj.kappa0(1),theta);
            % dtheta/ds = kappa0 + dkappa0*s  -> theta(s)= kappa0*s +dkappa0/2 * s^2
            dtheta=obj.kappa0(1)*obj.s(1)+obj.dkappa0(1)/2*obj.s(1)^2;
            obj.poses(1,:) = [real(z), imag(z), dtheta]+obj.initialPose;
            
            for i=2:obj.numClothoids
                z = matlabshared.tracking.internal.scenario.fresnelg(obj.s(i),obj.dkappa0(i),obj.kappa0(i),obj.poses(i-1,3));
                % dtheta/ds = kappa0 + dkappa0*s  -> theta(s)= kappa0*s +dkappa0/2 * s^2
                dtheta=obj.kappa0(i)*obj.s(i)+obj.dkappa0(i)/2*obj.s(i)^2;
                obj.poses(i,:) = [real(z), imag(z), dtheta]+obj.poses(i-1,:);
            end
                       
        end
        
        function path=shift(obj,delta)  % shift path
            % path.numClothoids=obj.numClothoids;
             if length(delta) < obj.numClothoids
                delta=[delta delta(end)*ones(1,obj.numClothoids-length(delta))];
             end
             delta=[delta delta(1)];
             delta(end-1)=delta(1);
             
             for i=1:obj.numClothoids
                % path.s(i)=obj.s(i);
                % path.cums(i)=obj.cums(i);
                [obj.kappa0(i), obj.dkappa0(i), obj.s(i)] = shiftClothoid(obj.kappa0(i), obj.dkappa0(i), obj.s(i), delta(i), delta(i+1));
             end
             obj.cums=cumsum(obj.s);
             
            obj.initialPose(1)=obj.initialPose(1)+delta(1)*sin(obj.initialPose(3));
            obj.initialPose(2)=obj.initialPose(2)+delta(1)*cos(obj.initialPose(3));
            theta=obj.initialPose(3);
            z = matlabshared.tracking.internal.scenario.fresnelg(obj.s(1),obj.dkappa0(1),obj.kappa0(1),theta);
            % dtheta/ds = kappa0 + dkappa0*s  -> theta(s)= kappa0*s +dkappa0/2 * s^2
            dtheta=obj.kappa0(1)*obj.s(1)+obj.dkappa0(1)/2*obj.s(1)^2;
            obj.poses(1,:) = [real(z), imag(z), dtheta]+obj.initialPose;
            
            for i=2:obj.numClothoids
                z = matlabshared.tracking.internal.scenario.fresnelg(obj.s(i),obj.dkappa0(i),obj.kappa0(i),obj.poses(i-1,3));
                % dtheta/ds = kappa0 + dkappa0*s  -> theta(s)= kappa0*s +dkappa0/2 * s^2
                dtheta=obj.kappa0(i)*obj.s(i)+obj.dkappa0(i)/2*obj.s(i)^2;
                obj.poses(i,:) = [real(z), imag(z), dtheta]+obj.poses(i-1,:);
            end
                        
            path=obj;
        end
        
        function obj=closingClothoid(obj)
            dtheta=wrapToPi(obj.initialPose(3)-obj.poses(end,3));
            dx=obj.initialPose(1)-obj.poses(end,1);
            dy=obj.initialPose(2)-obj.poses(end,2);
            
            % initialize clothoid with circle of constant radius
            % sqrt(dx^2+dy^2) = secant along circle
            % d = 2 R sin(theta/2), kappa =2 sin(theta/2) / d
            kappa0=2*sin(dtheta/2)/sqrt(dx^2+dy^2); 
            s0=dtheta/kappa0; % path length of circular segment
            param=[kappa0 0 s0];
            objfun=@(param) ([dx, dy, dtheta]-clothoid2Pose(param(1),param(2),param(3),obj.poses(end,3)));  % objective function total least squares
            options=optimoptions(@lsqnonlin,'display','off');
            sol=lsqnonlin(objfun,param,[],[],options);
            obj.numClothoids=obj.numClothoids+1;
            obj.kappa0=[obj.kappa0 sol(1)];
            obj.dkappa0=[obj.dkappa0 sol(2)];
            obj.s=[obj.s sol(3)];
            obj.cums=[obj.cums obj.cums(end)+sol(3)];
        end
        
               
        function s=pathlength(obj) % total path length
            s=obj.cums(end);
        end
        
        
        function kappas=kappa(obj,s) % curvature at path length s
            % modulo path length
            s = mod(s,[obj.cums(end)]);
            ind = find(s<[obj.cums],1,'first');
            kappas=[obj.kappa0(ind)]+[obj.dkappa0(ind)]*(s-[obj.cums(ind)]);
        end
        
        function [ey epsi]=poseError(obj,pose,s)
            % pose error between pose and path at pathlength s
            poseRef=obj.sigma(s);
            assert(isequal(size(pose),size(poseRef)));
            ey=cos(poseRef(:,3)).*(pose(:,2)-poseRef(:,2))-sin(poseRef(:,3)).*(pose(:,1)-poseRef(:,1));
            epsi=wrapToPi(pose(:,3)-poseRef(:,3));
        end
        
        function pose=sigma(obj,s)
            s= mod(s,[obj.cums(end)]);
            for i=1:length(s)
                ind = find(s(i)<[obj.cums],1,'first');
                if (ind==1)  % first segment
                    theta=obj.initialPose(3);
                    z = matlabshared.tracking.internal.scenario.fresnelg(s(i),obj.dkappa0(1),obj.kappa0(1),theta);
                    dtheta=obj.kappa0(1)*s(i)+obj.dkappa0(1)/2*s(i)^2;
                    pose(i,:) = [real(z), imag(z), dtheta]+obj.initialPose;
                else
                    s(i)=s(i)-obj.cums(ind-1); % subtract path length until start of clothoid
                    theta=obj.poses(ind-1,3);
                    z = matlabshared.tracking.internal.scenario.fresnelg(s(i),obj.dkappa0(ind),obj.kappa0(ind),theta);
                    dtheta=obj.kappa0(ind)*s(i)+obj.dkappa0(ind)/2*s(i)^2;
                    pose(i,:) = [real(z), imag(z), dtheta]+obj.poses(ind-1,:);
                end
            end
        end    
            
        function plot(obj,deltas,color)
            if nargin < 3
                color='b';
            end
            if nargin < 2
                deltas=0.25;
            end
            s=0:deltas:obj.cums(end);
            poses=obj.sigma(s);
            endposes=obj.sigma(obj.cums);
            plot(poses(:,1),poses(:,2),color,endposes(:,1),endposes(:,2),'o','MarkerEdgeColor','r','MarkerFaceColor','r');
            
            axis([min(poses(:,1))-2.0,max(poses(:,1))+2.0,min(poses(:,2))-2.0,max(poses(:,2))+2.0]),
            axis equal;
            
            xlabel('x');
            ylabel('y');
        end
        
        function shat = pose2PathLength(obj,pose,s0)
            % determine the equivalent path length of the current pose
            options=optimoptions(@lsqnonlin,'MaxIterations',5,...
                                 'FunctionTolerance',0.2,...
                                 'OptimalityTolerance',0.1,...
                                 'display','off');
            shat=lsqnonlin(@(s)obj.pose2Path(s,pose),s0,[],[],options);
        end
        
        function displacement=pose2Path(obj,s,pose)
            poseref=obj.sigma(s);
            displacement=pose(:,1:2)-poseref(:,1:2);
        end
        
    end
    end

    
    
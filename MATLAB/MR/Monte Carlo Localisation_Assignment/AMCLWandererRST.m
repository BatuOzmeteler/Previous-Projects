classdef AMCLWandererRST < handle
    %AMCLWandererRST Robot driver using Vector Field Histogram method.
    % w = AMCLWandererRST(tf, velPub, velMsg) creates a
    % helper that drives robot using Vector Field Histogram (VFH) method.
    % VFH needs a laser scan message, a transformation from laser sensor
    % frame to robot base frame, velocity publisher and the corresponding 
    % message type to control a robot. This modification allows to register
    % the function scan_callback() as callback function of a laser scan
    % subscriber. Thus, everytime a scan message arrives, a new control
    % input is generated.
    %
    % Example:
    %
    %   % Create subscriber to laser scan topic: '/scan'.
    %   laserSub = rossubscriber('scan');
    %
    %   % Create velocity publisher to issue commands to robot.
    %   [velPub,velMsg] = rospublisher('/vel_cmd','geometry_msgs/Twist');
    %
    %   % Obtain the transform from the robot's base frame to its sensor
    %   % frame.
    %   tftree = rostf;
    %   waitForTransform(tftree,'/base','/sensor');
    %   sensorTransform = getTransform(tftree,'/base','/sensor');
    %
    %   % Construct AMCLWandererRST class
    %   wanderer = AMCLWandererRST(sensorTransform, velPub, velMsg);
    %   % Register callback
    %   laserSub.NewMessageFcn = @wanderer.scan_callback;
    %
    %   % Issue velocity commands to robot to make it move around without
    %   % hitting obstacles.
    %   for i = 1:100
    %       wander(wanderHelper);
    %       pause(0.01);
    %   end
    %
    %   % Stop the robot
    %   stop(wanderHelper);
    
    % Copyright 2015 The MathWorks, Inc.
    
    properties (Access = private)
        % TF - Transformation Tree data
        TF
        % VelocityPublisher - Velocity publisher
        VelocityPublisher
        % VelocityMessage - ROS velocity message type
        VelocityMessage
        % Vector Field Histogram object
        VFH
        % Desired moving direction
        TargetDirection = 0
    end
    
    methods
        function obj = AMCLWandererRST(tf, velPub, velMsg)
            
            % Initialize sensor interface
            obj.TF = tf;
            
            % Initialize control interface
            obj.VelocityPublisher = velPub;
            obj.VelocityMessage = velMsg;
            
            % Initialize VectorFieldHistogram object. The parameters here
            % are tuned to prevent TurtleBot pass doors in the office
            % environment used in AdaptiveMonteCarloLocalizationExample.
            obj.VFH = robotics.VectorFieldHistogram;
            obj.VFH.DistanceLimits = [0.45 3];
            obj.VFH.RobotRadius = 0.3;
            obj.VFH.MinTurningRadius = 0.3;
            obj.VFH.SafetyDistance = 0.3;
            obj.VFH.HistogramThresholds= [3 10];
        end
        
        function scan_callback(obj, ~, scan)
            %WANDER Move robot around while avoiding obstacles.
            
            % Get laser scan data
            [ranges, angles] = transformLaserToRobot(obj, scan, obj.TF);
            
            % Call VFH step method
            steerDir = step(obj.VFH, ranges, angles, obj.TargetDirection);
            
            if ~isnan(steerDir)
                desiredV = 0.5;
                w = ComputeAngularVelocity(steerDir, 0.5);
            else
                desiredV = 0.0;
                w = 0.3;
            end
            
            % Send velocity commands
            drive(obj, desiredV, w)
        end
        
        function stop(obj)
            %STOP Stop the robot
            drive(obj, 0, 0);
        end     
    end
    
    methods (Access = private)
        function drive(obj, v, w)
            %DRIVE Send out linear and angular velocities to robot through
            %ros publisher
            obj.VelocityMessage.Linear.X = v;
            obj.VelocityMessage.Angular.Z = w;
            send(obj.VelocityPublisher, obj.VelocityMessage);
        end
    end
    
    methods (Access = private)
        function [ranges, angles] = transformLaserToRobot(~, scan,tf)
            %transformLaserToRobot Transform laser data to robot base frame
            %  This transform function is only suitable for 2D laser scan
            %  sensor mounted on surface parallel to ground plane.
                        
            % Convert to Cartesian
            cart = scan.readCartesian;
            cart(:,3) = 0;
            
            % Get the rotation matrix
            quat = [tf.Transform.Rotation.W tf.Transform.Rotation.X ...
                tf.Transform.Rotation.Y tf.Transform.Rotation.Z];
            rotm = quat2tform(quat);
            
            % Get translation matrix
            trvec = [tf.Transform.Translation.X tf.Transform.Translation.Y ...
                tf.Transform.Translation.Z];
            transln = trvec2tform(trvec);
            
            % Create homogeneous transform
            tform = rotm*transln;
            
            % Create homogeneous points for laser scan
            hom = cart2hom(cart);
            
            % Apply homogeneous transform
            trPts = tform*hom';
            
            % Convert back to Cartesian points
            cart = hom2cart(trPts');
            
            % Return transformed polar angles and ranges
            [angles, ranges] = cart2pol(cart(:,1), cart(:,2));
        end
    end
    
end


function w = ComputeAngularVelocity(steeringDir, wMax)
    %ComputeAngularVelocity Compute angular velocity
    %   W = ComputeAngularVelocity(STEERINGDIR)
    %   returns the angular velocity W for a differential drive robot in radians
    %   per second for a given steering direction STEERINGDIR in the robot's
    %   coordinate frame in radians.
    %
    %   W = ComputeAngularVelocity(STEERINGDIR, WMAX)
    %   returns the angular velocity W for a differential drive robot in radians
    %   per second for a given steering direction STEERINGDIR in the robot's
    %   coordinate frame in radians and maximum angular velocity WMAX in
    %   radians per second. The output angular velocity is saturated based on
    %   the WMAX.
    %
    %   Example:
    %       
    %       w = ComputeAngularVelocity(0.2, 1);

    % Copyright 2015 The MathWorks, Inc.

    %#codegen
    %#ok<*EMCA>

    % Allow variable input arguments
    if nargin == 1   
        wMax = inf;
    end

    validateattributes(steeringDir, {'double'},{'real'},...
        'ComputeAngularVelocity', 'STEERINGDIR', 1);
    validateattributes(wMax, {'double'},{'real', 'positive'},...
        'ComputeAngularVelocity', 'WMAX', 2);

    % Computing in robot's coordinate frame
    curPose = [0 0 0];

    % The following computation is similar to robotics.PurePursuit
    lookaheadPoint = [cos(steeringDir), sin(steeringDir)];
    slope = atan2((lookaheadPoint(2) - curPose(2)), ...
        (lookaheadPoint(1) - curPose(1)));
    alpha = angdiff(curPose(3), slope);

    % Angular velocity command for a differential drive robot is
    % equal to the desired curvature to be followed by the robot.

    w = (2*sin(alpha));

    % Pick a constant rotation when robot is facing in the opposite
    % direction of the path
    if abs(abs(alpha) - pi) < 1e-12
        w = sign(w)*1;
    end

    if abs(w) > wMax
        w = sign(w)*wMax;
    end

end



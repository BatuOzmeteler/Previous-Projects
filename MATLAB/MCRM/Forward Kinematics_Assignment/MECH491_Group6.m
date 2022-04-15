
% Listen to the lectures of Spatial Transformations and Direct(Forward) Kinematics
% Code Repository for UR10 Robot: https://github.com/ros-industrial/robot_movement_interface
% UR10 - 6DOF (6 Degrees of Freedom)

%import UR10 robot from URDF file
rosrobot = importrobot('ur10.urdf');
rosrobot.showdetails()

%get the home configuration of the UR10 robot
home = homeConfiguration(rosrobot);
%ransform between world frame and end effector
transform_home2ee = getTransform(rosrobot, home,'ee_link');

%open VM
%open Mozilla Firefox to check internet connection

%catkin_make
%source devel/setup.bash

%To launch ROS server, run roscore

ROS_MASTER_URI = 'http://ubuntu:11311/';
setenv('ROS_MASTER_URI', 'http://ubuntu:11311/')
rosinit('http://ubuntu:11311/')

%To check MATLAB-ROS connection and communication:
%rosnode list 
%rostopic list
%rosnode ping /matlab_global_node_79866
%disable Firewall if a problem occurs

chatpub = rospublisher('/chatter', 'std_msgs/String');
msg = rosmessage(chatpub);
msg.Data = 'test phrase';

%rostopic echo /chatter
send(chatpub,msg);

%check if the 'test phrase' is received on ROS side 

jointstatepublisher = rospublisher('/ur10/joint_states','sensor_msgs/JointState');
jointstatemsg = rosmessage('sensor_msgs/JointState');

%catkin_make
%roslaunch ur_launch ur10_sim_visualization.launch use_joint_publisher:=false

rosrandq = randomConfiguration(rosrobot);
for i=1:6
    jointstatemsg.Name(i) = cellstr(rosrandq(i).JointName);
    jointstatemsg.Position(i) = rosrandq(i).JointPosition;
end
jointstatemsg.Header.Stamp = rostime('now');

%rostopic echo /ur10/joint_states

jointstatepublisher.send(jointstatemsg);

%SIMULATION LOOP 

goIntoLoop = true;

displayMatlab = false;
displayRVIZ = true;

if goIntoLoop
    rate = 20; % Rate of 20 Hz
    rateObj=robotics.Rate(rate);
    maxTime = 4;
    
    % prepare loop
    qi=[0 0 0 0 0 0];
    qt=[pi/2 -pi/2 pi/2 0 pi/3 -pi/3];
    figure(6);
    hold off;
    rateObj.reset;  % reset time at the beginning of the loop
    while rateObj.TotalElapsedTime < maxTime
        
        qinterp=qi+(qt-qi)*min(1,rateObj.TotalElapsedTime/maxTime);
        rosqinterp=JointVec2JointConf(rosrobot,qinterp);
        if (displayMatlab)
            rosrobot.show(rosqinterp);
        end
        if (displayRVIZ)
            for i=1:6
                jointstatemsg.Name(i)=cellstr(rosqinterp(i).JointName);
                jointstatemsg.Position(i)=rosqinterp(i).JointPosition;
            end
            jointstatemsg.Header.Stamp=rostime('now');
            send(jointstatepublisher,jointstatemsg);
        end
        waitfor(rateObj);
    end
    
    disp('Loop ended');
end

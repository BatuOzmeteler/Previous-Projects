

chatpub = rospublisher('/chatter', 'std_msgs/String');
msg = rosmessage(chatpub);
msg.Data = 'test phrase';
send(chatpub,msg);

chatsub = rossubscriber('chatter');
chatsub.LatestMessage

%-------------------------------WORKFLOW----------------------------------%
%open VM 
%open Mozilla Firefox to check internet connection
%disable Firewall if a problem occurs

%roscore
%catkin_make
%source /devel/setup.bash
%rosinit('http://ubuntu:11311/')
%rosnode list
%rostopic list
%rostopic echo /chatter
%run the example script to check if the communication is working

%roslaunch ur_launch ur10_sim_visualization.launch use_joint_publisher:=false
%rostopic echo /ur10/joint_states
%run the simulation loop
%-------------------------------------------------------------------------%

% If you want to visualizesome configurations on the robot run:
% roslaunch ur_launchur10_sim_visualization.launch

% If you want to simulate the robot run:
% roslaunch ur_launchur10_sim_gazebo.launch


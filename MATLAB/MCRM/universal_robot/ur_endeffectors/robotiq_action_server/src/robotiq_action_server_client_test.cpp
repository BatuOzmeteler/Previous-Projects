#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <control_msgs/GripperCommandAction.h>
#include <ros/ros.h>
#include <ur_utilities/ur_misc/ur_utility.h>

int main(int argc, char** argv)
{
    // roslaunch robotiq_action_server robotiq_c_model_action_server.launch
    // rosrun robotiq_action_server robotiq_action_server_client_test

    ros::init(argc, argv, "test_robotiq_action_server");

    ros::NodeHandle pnh("~");

    std::string gripper_name;
    pnh.param<std::string>("gripper_name", gripper_name, "gripper");

    // create the action client
    // true causes the client to spin its own thread
    actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> ac(gripper_name, true);

    ROS_INFO("Waiting for action server to start...");

    // wait for the action server to start
    ac.waitForServer();  // will wait for infinite time

    ROS_INFO("Action server started, sending goal(s).");

    // create a FollowJointTrajectoryGoal

    // close with full speed
    double position = 0;
    double speed    = 100;
    double effort   = 100;
    bool real       = true;
    // std::string ur_type = "unused_for_real_robot...";

    control_msgs::FollowJointTrajectoryGoal goal = robot_utilities::robot_misc::URUtility::getGripperCommandGoal(position, speed, effort, real);

    ac.sendGoal(goal);
    ROS_INFO("Goal sent to Action Server");
    ac.waitForResult(ros::Duration(5.0));

    // opening with half speed
    goal = robot_utilities::robot_misc::URUtility::getGripperCommandGoal(100, 50, 100, true);
    ac.sendGoal(goal);
    ROS_INFO("Goal sent to Action Server");
    ac.waitForResult(ros::Duration(5.0));

    // exit
    return 0;
}

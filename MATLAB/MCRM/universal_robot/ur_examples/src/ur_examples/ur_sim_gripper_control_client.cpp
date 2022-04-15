#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <ros/ros.h>
#include <ur_utilities/ur_misc/ur_utility.h>

int main(int argc, char** argv)
{
    // usage:
    // roslaunch ur_launch ur3_sim_gazebo.launch
    // rosrun ur_examples ur_sim_gripper_control_client

    ros::init(argc, argv, "test_gripper_command_sim");
    ros::NodeHandle n("~");

    // std::string ur_type      = "ur10";
    // std::string action_topic = "/ur10/gripper/follow_joint_trajectory";
    // std::string ur_type      = "ur3";
    std::string action_topic = "/ur3/gripper/follow_joint_trajectory";

    // create the action client
    // true causes the client to spin its own thread
    actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> ac(n, action_topic, true);

    ROS_INFO("Waiting for action server to start...");
    // wait for the action server to start
    ac.waitForServer();  // will wait for infinite time

    ROS_INFO("Action server started, sending goal(s).");

    // closing
    double position = 0;

    control_msgs::FollowJointTrajectoryGoal goal = robot_utilities::robot_misc::URUtility::getGripperCommandGoal(position, 0, 0, false);

    ac.sendGoal(goal);
    ROS_INFO("Goal sent to Action Server");
    ac.waitForResult(ros::Duration(1.5));

    // opening
    goal = robot_utilities::robot_misc::URUtility::getGripperCommandGoal(100, 0, 0, false);

    ac.sendGoal(goal);
    ROS_INFO("Goal sent to Action Server");
    ac.waitForResult(ros::Duration(1.5));

    // exit
    return 0;
}

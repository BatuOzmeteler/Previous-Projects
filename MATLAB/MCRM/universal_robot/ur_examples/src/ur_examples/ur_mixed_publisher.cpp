#include <actionlib/client/simple_action_client.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <control_msgs/FollowJointTrajectoryGoal.h>
#include <controller_manager_msgs/SwitchController.h>
#include <ros/ros.h>
#include <string.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <ur_utilities/ur_manipulator/ur_manipulator.h>
#include <ur_utilities/ur_misc/ur_utility.h>

using URUtility     = robot_utilities::robot_misc::URUtility;
using URManipulator = robot_utilities::robot_manipulator::URManipulator;

bool _real_robot = false;

void setVelocityModeManually()
{
    // ROS service client (choose correct topic or remap in launch file)
    ros::NodeHandle n("~");
    ros::ServiceClient service_client = n.serviceClient<controller_manager_msgs::SwitchController>("/controller_manager/switch_controller");

    /*
     Real robots topic (UR3/UR10):  /ur_driver/controller_manager/switch_controller
     Simulated UR3 topic:           /ur3/controller_manager/switch_controller
     Simulated UR10 topic:          /ur10/controller_manager/switch_controller
    */

    controller_manager_msgs::SwitchController switch_to_speed;
    switch_to_speed.request.strictness = 1;

    if (_real_robot)
    {
        switch_to_speed.request.stop_controllers = URUtility::getPositionControllerNames();
    }
    else
    {
        switch_to_speed.request.start_controllers = URUtility::getVelocityControllerNames();
        switch_to_speed.request.stop_controllers  = URUtility::getPositionControllerNames();
    }
    service_client.call(switch_to_speed);
}

void setPositionModeManually()
{
    // ROS service client (choose correct topic or remap in launch file)
    ros::NodeHandle n("~");
    ros::ServiceClient service_client = n.serviceClient<controller_manager_msgs::SwitchController>("/controller_manager/switch_controller");

    /*
     Real robots topic (UR3/UR10):  /ur_driver/controller_manager/switch_controller
     Simulated UR3 topic:           /ur3/controller_manager/switch_controller
     Simulated UR10 topic:          /ur10/controller_manager/switch_controller
    */

    controller_manager_msgs::SwitchController switch_to_position;
    switch_to_position.request.strictness = 1;

    if (_real_robot)
    {
        switch_to_position.request.start_controllers = URUtility::getPositionControllerNames();
    }
    else
    {
        switch_to_position.request.stop_controllers  = URUtility::getVelocityControllerNames();
        switch_to_position.request.start_controllers = URUtility::getPositionControllerNames();
    }
    service_client.call(switch_to_position);
}

void sendVelocityAutomatically()
{
    // Helper classes
    URUtility::Ptr ur_utility         = std::make_shared<URUtility>();
    URManipulator::Ptr ur_manipulator = std::make_shared<URManipulator>();

    // Initialize class
    ur_manipulator->initialize(_real_robot, false, ur_utility);

    // Speed msg is an NxM Eigen::MatrixXd, with N = DOF of the robot and M the number of speed commands
    Eigen::MatrixXd speed = Eigen::MatrixXd(6, 1);
    speed.col(0) << 0.1, 0.0, 0.0, 0.0, 0.0, 0.0;

    // Send single speed command
    ur_manipulator->sendJointVelocityCommands(speed, {0.0});

    // Stop after 5 seconds
    ros::Duration(5).sleep();
    ur_manipulator->stop();
}

void sendPositionAutomatically()
{
    // Helper classes
    URUtility::Ptr ur_utility         = std::make_shared<URUtility>();
    URManipulator::Ptr ur_manipulator = std::make_shared<URManipulator>();

    // Initialize class
    ur_manipulator->initialize(_real_robot, false, ur_utility);

    // Position msg is an NxM Eigen::MatrixXd, with N = DOF of the robot and M the number of speed commands
    Eigen::MatrixXd position = Eigen::MatrixXd(6, 1);
    position.col(0) << 1.0, -1.0, 1.0, 0.0, 0.0, 0.0;

    // Send single speed command (time defines the timestamp of the target)
    // !!Give the robot some time to safely reach the target!!
    ur_manipulator->sendJointPositionCommands(position, {2.0});
}

void sendVelocityManually()
{
    setVelocityModeManually();

    // ROS publisher (choose correct topic or remap in launch file)
    ros::NodeHandle n("~");
    ros::Publisher joint_speed_publisher = n.advertise<trajectory_msgs::JointTrajectory>("joint_speed_controls", 1);

    /*
     Real robots topic (UR3/UR10):  /ur_driver/joint_speed
     Simulated UR3 topic:           /ur3/joint_speed
     Simulated UR10 topic:          /ur10/joint_speed
    */

    // Helper class to get joint names
    URUtility::Ptr ur_utility = std::make_shared<URUtility>();

    // Create msg
    trajectory_msgs::JointTrajectory speed_msg;

    speed_msg.points.resize(1);
    speed_msg.points[0].velocities = {0.1, 0.0, 0.0, 0.0, 0.0, 0.0};  // Desired velocities

    speed_msg.joint_names  = ur_utility->getJointNames();
    speed_msg.header.stamp = ros::Time::now();

    // Let subscriber connect (only needed once)
    ros::Duration(1).sleep();

    // Send msg
    joint_speed_publisher.publish(speed_msg);

    // Stop after 5 seconds
    ros::Duration(5).sleep();
    speed_msg.points[0].velocities = std::vector<double>(6, 0.0);
    speed_msg.header.stamp         = ros::Time::now();
    joint_speed_publisher.publish(speed_msg);
}

void sendActionGoalManually()
{
    setPositionModeManually();

    // ROS action client (choose correct topic or remap in launch file)
    ros::NodeHandle n("~");
    actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> action_client(n, "follow_joint_trajectory", true);

    /*
     Real robots topic (UR3/UR10):  /ur_driver/vel_based_pos_traj_controller/follow_joint_trajectory
     Simulated UR3 topic:           /ur3/vel_based_pos_traj_controller/follow_joint_trajectory
     Simulated UR10 topic:          /ur10/vel_based_pos_traj_controller/follow_joint_trajectory
    */

    // Helper class to get joint names
    URUtility::Ptr ur_utility = std::make_shared<URUtility>();

    // Create goal msg and trajectory msg
    control_msgs::FollowJointTrajectoryGoal trajectory_msg;
    trajectory_msgs::JointTrajectory trajectory;

    trajectory.points.resize(1);
    trajectory.points[0].positions       = {0.0, -1.0, 1.0, 0.0, 0.0, 0.0};  // Desired joint position
    trajectory.points[0].time_from_start = ros::Duration(2.0);               // !!Give the robot some time to safely reach the target!!

    trajectory.joint_names  = ur_utility->getJointNames();
    trajectory.header.stamp = ros::Time::now();

    // Store trajectory msg in goal msg
    trajectory_msg.trajectory = trajectory;

    // Let server connect
    action_client.waitForServer();

    // Send goal msg and wait for result
    action_client.sendGoal(trajectory_msg);
    action_client.waitForResult();
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ur_mixed_publisher");
    ros::NodeHandle n("~");
    n.getParam("real_robot", _real_robot);

    // Examples for switching between position and velocity mode manually
    // setPositionModeManually();
    // setVelocityModeManually();

    // Examples for sending position or speed commands manually
    // sendActionGoalManually();
    // sendVelocityManually();

    // Examples for sending position and speed commands with automatic switching (recommended)
    // sendPositionAutomatically();
    // sendVelocityAutomatically();

    ros::spin();

    return 0;
}

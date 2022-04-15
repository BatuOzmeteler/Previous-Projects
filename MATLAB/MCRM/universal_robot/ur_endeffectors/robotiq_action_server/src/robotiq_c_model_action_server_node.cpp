#include "robotiq_action_server/robotiq_c_model_action_server.h"

namespace {
// Defines a default for the c2 model 140 gripper
robotiq_action_server::CModelGripperParams c2_140_defaults()
{
    robotiq_action_server::CModelGripperParams params;
    params._min_gap    = -.017;  // because fingers can push forward before the mechanical stops are reached
    params._max_gap    = 0.140;
    params._min_effort = 40.0;  // This is a guess. Could not find data with quick search.
    params._max_effort = 100.0;
    params._min_speed  = 0.0;  // until now: value does not correspond to a physical value TODO: messure aktual min/max speed of gripper
    params._max_speed  = 100.0;

    return params;
}

// Defines a default for the a universal c2-model
// use percentage values
robotiq_action_server::CModelGripperParams c2_univ_defaults()
{
    robotiq_action_server::CModelGripperParams params;
    params._min_gap    = 0.0;
    params._max_gap    = 100.0;
    params._min_effort = 0.0;
    params._max_effort = 100.0;
    params._min_speed  = 0.0;
    params._max_speed  = 100.0;

    return params;
}
}  // namespace

int main(int argc, char** argv)
{
    // Can be renamed with standard ROS-node launch interface
    ros::init(argc, argv, "gripper_action_server");

    // Private Note Handle for retrieving parameter arguments to the server
    ros::NodeHandle private_nh("~");

    std::string gripper_name;
    private_nh.param<std::string>("gripper_name", gripper_name, "gripper");

    // Fill out C-Model Params
    robotiq_action_server::CModelGripperParams cparams = c2_univ_defaults();

    // Min because fingers can push forward before the mechanical stops are reached
    private_nh.param<double>("min_gap", cparams._min_gap, cparams._min_gap);
    private_nh.param<double>("max_gap", cparams._max_gap, cparams._max_gap);
    private_nh.param<double>("min_effort", cparams._min_effort, cparams._min_effort);
    private_nh.param<double>("max_effort", cparams._max_effort, cparams._max_effort);
    private_nh.param<double>("min_speed", cparams._min_speed, cparams._min_speed);
    private_nh.param<double>("max_speed", cparams._max_speed, cparams._max_speed);

    ROS_INFO("Initializing Robotiq action server for gripper: %s", (gripper_name + "/follow_joint_trajectory").c_str());

    // The name of the gripper -> this server communicates over name/inputs and name/outputs
    robotiq_action_server::CModelGripperActionServer gripper(gripper_name, cparams);

    ROS_INFO("Robotiq action-server spinning for gripper: %s", (gripper_name + "/follow_joint_trajectory").c_str());
    ros::spin();
}

/**
 * ActionServer interface to the control_msgs/GripperCommand action
 * for a Robotiq C-Model device
 */

#include <robotiq_action_server/robotiq_c_model_action_server.h>
#include <thread>

namespace robotiq_action_server {

CModelGripperActionServer::CModelGripperActionServer(const std::string& name, const CModelGripperParams& params)
    : _nh(),
      _as(_nh, (name + "/follow_joint_trajectory"), false),
      _action_name(name + "/follow_joint_trajectory"),
      _gripper_name(name),
      _gripper_params(params)
{
    // Register Callbacks
    _as.registerGoalCallback(boost::bind(&CModelGripperActionServer::goalCB, this));
    _as.registerPreemptCallback(boost::bind(&CModelGripperActionServer::preemptCB, this));

    // Subscribe to gripper topic
    _state_sub = _nh.subscribe("/CModelRobotInput", 1, &CModelGripperActionServer::analysisCB, this);

    // Publisher
    _goal_pub                = _nh.advertise<GripperOutput>("/CModelRobotOutput", 1);
    _gripper_joint_state_pub = _nh.advertise<sensor_msgs::JointState>(_gripper_name + "/joint_states", 1);
    _gripper_state_pub       = _nh.advertise<control_msgs::JointTrajectoryControllerState>(_gripper_name + "/state", 1);

    _as.start();
    ROS_INFO("Gripper Action server started");

    // Gripper Activation
    issueActivation();

    if (_current_register_state.gSTA == 0x3)
    {
        ROS_INFO("Gripper activated");
        std::thread threadObj(&CModelGripperActionServer::gripperStatePublisher, this);
        threadObj.detach();
    }
    else
        ROS_ERROR("Could not activate gripper");
}

void CModelGripperActionServer::goalCB()
{
    // Check to see if the gripper is in an active state where it can take goals
    if (_current_register_state.gSTA != 0x3)
    {
        ROS_WARN("%s could not accept goal because the gripper is not yet active", _action_name.c_str());
        return;
    }

    GripperGoal current_goal(*(_as.acceptNewGoal()));

    if (_as.isPreemptRequested())
    {
        _as.setPreempted();
    }

    try
    {
        _goal_register_state = goalToRegisterState(current_goal, _gripper_params);
        _goal_pub.publish(_goal_register_state);
    }
    catch (BadArgumentsError& e)
    {
        ROS_INFO("%s No goal issued to gripper", _action_name.c_str());
    }
}

void CModelGripperActionServer::preemptCB()
{
    // ROS_INFO("%s: Preempted", _action_name.c_str());

    _as.setPreempted();
}

void CModelGripperActionServer::analysisCB(const GripperInput::ConstPtr& msg)
{
    _current_register_state = *msg;

    if (!_as.isActive()) return;

    // Check to see if the gripper is in its activated state
    if (_current_register_state.gSTA != 0x3)
    {
        // Check to see if the gripper is active or if it has been asked to be active
        if (_current_register_state.gSTA == 0x0 && _goal_register_state.rACT != 0x1)
        {
            // If it hasn't been asked, active it
            issueActivation();
        }
        return;
    }

    // Check for errors
    if (_current_register_state.gFLT)
    {
        ROS_WARN("%s faulted with code: %x", _action_name.c_str(), _current_register_state.gFLT);
        _as.setAborted(registerStateToResult(_current_register_state, _gripper_params, _goal_register_state.rPR));
    }
    else if (_current_register_state.gGTO && _current_register_state.gOBJ && _current_register_state.gPR == _goal_register_state.rPR)
    {
        // If commanded to move and if at a goal state and if the position request matches the echoed PR, we're
        // done with a move

        // ROS_INFO("%s succeeded", _action_name.c_str());
        // TODO: gripper shows succeeded if values are out of range
        _as.setSucceeded(registerStateToResult(_current_register_state, _gripper_params, _goal_register_state.rPR));
    }
    else
    {
        // Publish feedback
        _as.publishFeedback(registerStateToFeedback(_current_register_state, _gripper_params, _goal_register_state));
    }
}

void CModelGripperActionServer::issueActivation()
{
    ROS_INFO("Activating gripper for gripper action server: %s", _action_name.c_str());
    for (int i = 0; i < _max_attempts; ++i)
    {
        if (_current_register_state.gSTA == 0x3 || _current_register_state.gSTA == 0x1) return;

        GripperOutput out;  // All values should be 0
        _goal_register_state = out;
        _goal_pub.publish(out);
        ros::Duration(0.5).sleep();

        out.rACT = 0x1;
        // other params should be zero
        _goal_register_state = out;
        _goal_pub.publish(out);
        ros::Duration(2.5).sleep();

        ros::spinOnce();
    }
}

void CModelGripperActionServer::gripperStatePublisher()
{
    // gripper position is assumed as:
    //  %           pos       reg.state   real_pos
    // 100% -> fully opened ->  0x00 ->   -0.402
    // 0%   -> fully closed ->  0xFF ->   0.402

    ros::Rate loop(125);

    double current_gripper_pos, desired_gripper_pos;
    std::string gripper_joint_name = "gripper_joint";

    control_msgs::JointTrajectoryControllerState gripper_state_msg;
    sensor_msgs::JointState gripper_joint_state_msg;

    while (ros::ok())
    {
        current_gripper_pos = (double)_current_register_state.gPO * 0.804 / 255 - 0.402;
        desired_gripper_pos = (double)_current_register_state.gPR * 0.804 / 255 - 0.402;

        gripper_state_msg.desired.positions = {desired_gripper_pos};
        gripper_state_msg.actual.positions  = {current_gripper_pos};
        gripper_state_msg.error.positions   = {desired_gripper_pos - current_gripper_pos};
        gripper_state_msg.joint_names       = {gripper_joint_name};
        gripper_state_msg.header.stamp      = ros::Time::now();

        gripper_joint_state_msg.position     = {current_gripper_pos};
        gripper_joint_state_msg.velocity     = {0};
        gripper_joint_state_msg.effort       = {0};
        gripper_joint_state_msg.name         = {gripper_joint_name};
        gripper_joint_state_msg.header.stamp = ros::Time::now();

        _gripper_joint_state_pub.publish(gripper_joint_state_msg);
        _gripper_state_pub.publish(gripper_state_msg);

        loop.sleep();
    }
}

GripperOutput CModelGripperActionServer::goalToRegisterState(const GripperGoal& goal, const CModelGripperParams& params)
{
    GripperOutput result;
    result.rACT = 0x1;  // active gripper
    result.rGTO = 0x1;  // go to position
    result.rATR = 0x0;  // No emergency release

    // trajectery msg only used as a container only the first point is processed until now
    double position = goal.trajectory.points[0].positions[0];
    double effort   = goal.trajectory.points[0].accelerations[0];
    double velocity = goal.trajectory.points[0].velocities[0];

    if (position > params._max_gap || position < params._min_gap)
    {
        ROS_WARN("Goal gripper gap size is out of range(%f to %f): %f m", params._min_gap, params._max_gap, position);
        throw BadArgumentsError();
    }

    if (effort < params._min_effort || effort > params._max_effort)
    {
        ROS_WARN("Goal gripper effort out of range (%f to %f N): %f N", params._min_effort, params._max_effort, effort);
        throw BadArgumentsError();
    }

    if (velocity < params._min_speed || velocity > params._max_speed)
    {
        ROS_WARN("Goal gripper velocity out of range (%f to %f N): %f", params._min_speed, params._max_speed, velocity);
        throw BadArgumentsError();
    }

    double dist_per_tick = (params._max_gap - params._min_gap) / 255;
    double eff_per_tick  = (params._max_effort - params._min_effort) / 255;
    double vel_per_tick  = (params._max_speed - params._min_speed) / 255;

    result.rPR = static_cast<uint8_t>((params._max_gap - position) / dist_per_tick);
    result.rFR = static_cast<uint8_t>((effort - params._min_effort) / eff_per_tick);
    result.rSP = static_cast<uint8_t>((velocity - params._min_speed) / vel_per_tick);

    // ROS_INFO("Setting goal position register to %hhu", result.rPR);

    return result;
}

GripperResult CModelGripperActionServer::registerStateToResult(const GripperInput& input, const CModelGripperParams& params, uint8_t goal_pos)
{
    GripperResult result;
    if (input.gPO == goal_pos)
        result.error_code = 0;
    else
        result.error_code = -1;  // TODO: insert actual error codes

    return result;
}

GripperFeedback CModelGripperActionServer::registerStateToFeedback(const GripperInput& input, const CModelGripperParams& params,
                                                                   const GripperOutput& goal_reg_state_)
{
    GripperFeedback feedback;
    double dist_per_tick = (params._max_gap - params._min_gap) / 255;
    double eff_per_tick  = (params._max_effort - params._min_effort) / 255;

    feedback.desired.positions.resize(1);
    feedback.desired.effort.resize(1);
    feedback.actual.positions.resize(1);
    feedback.actual.effort.resize(1);
    feedback.error.positions.resize(1);
    feedback.error.effort.resize(1);

    feedback.actual.positions[0] = input.gPO * dist_per_tick + params._min_gap;
    feedback.actual.effort[0]    = input.gCU * eff_per_tick + params._min_effort;

    feedback.desired.positions[0] = goal_reg_state_.rPR * dist_per_tick + params._min_gap;
    feedback.desired.effort[0]    = goal_reg_state_.rFR * eff_per_tick + params._min_effort;

    feedback.error.positions[0] = feedback.desired.positions[0] - feedback.actual.positions[0];
    feedback.error.effort[0]    = feedback.desired.effort[0] - feedback.actual.positions[0];

    return feedback;
}

}  // namespace robotiq_action_server

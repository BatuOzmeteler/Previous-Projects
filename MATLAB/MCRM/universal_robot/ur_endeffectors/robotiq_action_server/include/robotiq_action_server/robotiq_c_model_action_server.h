/**
 * ActionServer interface to the control_msgs/GripperCommand action
 * for a Robotiq C-Model (2 finger) device
 */

#ifndef ROBOTIQ_C_MODEL_ACTION_SERVER_H
#define ROBOTIQ_C_MODEL_ACTION_SERVER_H

#include <actionlib/server/simple_action_server.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <control_msgs/GripperCommandAction.h>
#include <control_msgs/JointTrajectoryControllerState.h>
#include <robotiq_c_model_control/CModel_robot_input.h>
#include <robotiq_c_model_control/CModel_robot_output.h>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <string>

namespace robotiq_action_server {

using GripperInput  = robotiq_c_model_control::CModel_robot_input;
using GripperOutput = robotiq_c_model_control::CModel_robot_output;

using GripperCommandGoal     = control_msgs::FollowJointTrajectoryActionGoal;
using GripperCommandFeedback = control_msgs::FollowJointTrajectoryActionFeedback;
using GripperCommandResult   = control_msgs::FollowJointTrajectoryActionResult;

using GripperGoal     = control_msgs::FollowJointTrajectoryGoal;
using GripperFeedback = control_msgs::FollowJointTrajectoryFeedback;
using GripperResult   = control_msgs::FollowJointTrajectoryResult;

/**
 * @brief Structure containing the parameters necessary to translate
 *        GripperCommand actions to register-based commands to a
 *        particular gripper (and vice versa).
 *
 *        The min gap can be less than zero. This represents the case where the
 *        gripper fingers close and then push forward.
 *
 *        EDIT: for now (on RST) all these values are 0-100 (percentage values)
 */
struct CModelGripperParams
{
    double _min_gap;  // meters
    double _max_gap;
    double _min_effort;  // N / (Nm)
    double _max_effort;
    double _min_speed;  // 0
    double _max_speed;  // 100
};

/*  This struct is declared for the sole purpose of being used as an exception internally
    to keep the code clean (i.e. no output params). It is caught by the action_server and
    should not propogate outwards. If you use these functions yourself, beware.
*/
struct BadArgumentsError
{
};

/**
 * @brief The CModelGripperActionServer class. Takes as arguments the name of the gripper it is to command,
 *        and a set of parameters that define the physical characteristics of the particular gripper.
 *
 *        Listens for messages on input and publishes on output. Remap these.
 */
class CModelGripperActionServer
{
 public:
    CModelGripperActionServer(const std::string& name, const CModelGripperParams& params);

    // These functions are meant to be called by simple action server
    void goalCB();
    void preemptCB();
    void analysisCB(const GripperInput::ConstPtr& msg);

 private:
    void issueActivation();
    void gripperStatePublisher();

    GripperFeedback registerStateToFeedback(const GripperInput& input, const CModelGripperParams& params, const GripperOutput& goal_reg_state_);
    GripperResult registerStateToResult(const GripperInput& input, const CModelGripperParams& params, uint8_t goal_pos);
    GripperOutput goalToRegisterState(const GripperGoal& goal, const CModelGripperParams& params);

    ros::NodeHandle _nh;
    actionlib::SimpleActionServer<control_msgs::FollowJointTrajectoryAction> _as;

    ros::Subscriber _state_sub;               // Subscriber to grippers "input" topic
    ros::Publisher _goal_pub;                 // Publisher to grippers "output" topic
    ros::Publisher _gripper_state_pub;        // Publisher to gripper state broadcast
    ros::Publisher _gripper_joint_state_pub;  // Publisher to gripper state broadcast

    GripperOutput _goal_register_state;    // Goal information in gripper-register form
    GripperInput _current_register_state;  // State info in gripper-register form

    /* Used to translate GripperCommands in engineering units
     * to/from register states understood by gripper itself. Different
     * for different models/generations of Robotiq grippers */
    CModelGripperParams _gripper_params;

    std::string _action_name;
    std::string _gripper_name;
    const int _max_attempts = 20;
};

}  // namespace robotiq_action_server
#endif

#include <ur_utilities/ur_misc/ur_utility.h>

namespace robot_utilities {
namespace robot_misc {

URUtility::URUtility(robot_description::URDescription::Ptr robot_description) : RobotUtility(robot_description) {}

URUtility::URUtility() : RobotUtility(std::make_shared<robot_description::URDescription>("/robot_description", "world", "ee_link")) {}

std::vector<std::string> URUtility::getVelocityControllerNames() { return _velocity_controller_names; }

std::vector<std::string> URUtility::getPositionControllerNames() { return _position_controller_names; }

const std::vector<std::string> URUtility::_position_controller_names = {"vel_based_pos_traj_controller"};

const std::vector<std::string> URUtility::_velocity_controller_names = {"joint0_velocity_controller", "joint1_velocity_controller",
                                                                        "joint2_velocity_controller", "joint3_velocity_controller",
                                                                        "joint4_velocity_controller", "joint5_velocity_controller"};

const std::vector<std::string> URUtility::_gripper_joint_name = {"gripper_joint"};

control_msgs::FollowJointTrajectoryGoal URUtility::getGripperCommandGoal(double position, double speed, double effort, bool real)
{
    if (real)
    {
        // The real gripper takes the opening positon, the movement speed and the gripping effort as input arguments.
        // The robotiq_action_server/robotiq_c_model_action_server checks the validity of the inputs,
        // converts the msg to robotiq_c_model_control::CModel_robot_output and sends it to the gripper

        control_msgs::FollowJointTrajectoryGoal goal;
        goal.trajectory.points.resize(1);
        goal.trajectory.points[0].accelerations.resize(1);
        goal.trajectory.points[0].positions.resize(1);
        goal.trajectory.points[0].velocities.resize(1);

        goal.trajectory.points[0].accelerations[0] = effort;
        goal.trajectory.points[0].positions[0]     = position;
        goal.trajectory.points[0].velocities[0]    = speed;

        return goal;
    }
    else
    {  // gazebo simulation

        // The gripper in gazebo is modeled as one joint with angles in [-0.402 0.402]. In theory you could provide a trajectory with multiple points
        // (like for every other joit as well). But because you can't send a trajectory in reality, this is not implemented in simulation as well
        // -> send trajectory with only one point
        // Additionaly this node does a mapping of the position from [0 100] to [-0.402 0.402].

        // The time for the trajectory point could be aproximated from the current position, the desiered position and the desired speed...
        // But for now it is set to 1 sec for all inputs.

        control_msgs::FollowJointTrajectoryGoal goal;
        goal.trajectory.points.resize(1);
        goal.trajectory.points[0].accelerations.resize(1);
        goal.trajectory.points[0].velocities.resize(1);
        goal.trajectory.points[0].positions.resize(1);
        goal.trajectory.joint_names.resize(1);

        goal.trajectory.points[0].accelerations[0] = 0;
        goal.trajectory.points[0].velocities[0]    = 0;

        if (position < 0 || position > 100)
        {
            ROS_ERROR(
                "URUtility::getGripperCommandGoalgazebo - provided position must be in [0 100] --> percentage values! Goal with empty position will "
                "be "
                "returned...");
        }
        else
        {
            goal.trajectory.points[0].positions[0] = ((-position * (0.804 / 100)) + 0.402);  // mapping of position
        }

        goal.trajectory.points[0].time_from_start = ros::Duration(1);
        goal.trajectory.joint_names               = URUtility::_gripper_joint_name;

        return goal;
    }
}

}  // namespace robot_misc
}  // namespace robot_utilities

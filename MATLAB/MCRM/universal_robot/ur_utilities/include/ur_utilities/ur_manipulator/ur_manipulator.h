/*********************************************************************
 *
 * Software License Agreement
 *
 *  Copyright (c) 2018,
 *  TU Dortmund - Institute of Control Theory and Systems Engineering.
 *  All rights reserved.
 *
 *  This software is currently not released.
 *  Redistribution and use in source and binary forms,
 *  with or without modification, are prohibited.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Maximilian Krämer
 *********************************************************************/

#ifndef UR_MANIPULATOR_H
#define UR_MANIPULATOR_H

#include <robot_utilities/robot_manipulator/robot_manipulator.h>
#include <ur_utilities/ur_misc/ur_utility.h>
#include <mutex>

namespace robot_utilities {
namespace robot_manipulator {

class URManipulator : public RobotManipulator
{
 public:
    using Ptr = std::shared_ptr<URManipulator>;

    URManipulator();

    bool initialize(bool real_robot, bool reset_on_start, robot_misc::URUtility::Ptr ur_utility);

    int getJointNumber() const override;
    void stop() override;

 private:
    using URUtility = robot_misc::URUtility;

    URUtility::Ptr _ur_utility;

    ros::ServiceClient _controller_switch_srv;
    ros::Publisher _joint_speed_pub;
    ros::Publisher _joint_position_pub;
    ros::Subscriber _joint_states_sub;

    std::vector<trajectory_msgs::JointTrajectory> _trajectory_msgs;
    std::vector<double> _time_vector;

    bool _new_sequence = false;
    bool _publish_flag = false;

    std::mutex _mutex;
    std::thread _publish_worker;

    bool setVelocityMode() override;
    bool setPositionMode() override;

    void sendSingleJointVelocityCommand(const Eigen::Ref<const Eigen::VectorXd>& command) override;
    void sendMultipleJointVelocityCommands(const Eigen::Ref<const Eigen::MatrixXd>& u, const std::vector<double>& time, int n_points,
                                           int start_index) override;

    void sendSingleJointPositionCommand(const Eigen::Ref<const Eigen::VectorXd>& command, double time) override;
    void sendMultipleJointPositionCommands(const Eigen::Ref<const Eigen::MatrixXd>& q, const std::vector<double>& time, int n_points,
                                           int start_index) override;

    void stopJointVelocity();
    void stopJointPosition();
    void setUpJointVelocityCommand(trajectory_msgs::JointTrajectory& msg, const Eigen::Ref<const Eigen::VectorXd>& joint_speed);
    void addJointPositionCommand(trajectory_msgs::JointTrajectory& msg, const Eigen::Ref<const Eigen::VectorXd>& joint_position, double time = 0.0);
    void publishJointVelocityCommands();
};

}  // namespace robot_manipulator
}  // namespace robot_utilities

#endif  // UR_MANIPULATOR_H

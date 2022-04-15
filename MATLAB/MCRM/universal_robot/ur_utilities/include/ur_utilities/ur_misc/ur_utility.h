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

#ifndef UR_UTILITY_H
#define UR_UTILITY_H

#include <control_msgs/FollowJointTrajectoryAction.h>
#include <robot_utilities/robot_misc/robot_utility.h>
#include <ur_utilities/ur_description/ur_description.h>

namespace robot_utilities {
namespace robot_misc {

class URUtility : public RobotUtility
{
 public:
    using Ptr = std::shared_ptr<URUtility>;

    URUtility();
    URUtility(robot_description::URDescription::Ptr robot_description);

    static std::vector<std::string> getVelocityControllerNames();
    static std::vector<std::string> getPositionControllerNames();
    static control_msgs::FollowJointTrajectoryGoal getGripperCommandGoal(double position, double speed, double effort, bool real);

 private:
    static const std::vector<std::string> _velocity_controller_names;
    static const std::vector<std::string> _position_controller_names;
    static const std::vector<std::string> _gripper_joint_name;
};
}  // namespace robot_misc
}  // namespace robot_utilities

#endif  // UR_UTILITY_H

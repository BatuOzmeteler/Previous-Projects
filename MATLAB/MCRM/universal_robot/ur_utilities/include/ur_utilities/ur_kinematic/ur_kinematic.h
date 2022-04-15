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
 * Authors: Rodrigo Velasco, Maximilian Krämer
 *********************************************************************/

#ifndef UR_KINEMATIC_H
#define UR_KINEMATIC_H

#include <robot_utilities/robot_kinematic/robot_kinematic.h>
#include <ur_utilities/ur_misc/ur_utility.h>

namespace robot_utilities {
namespace robot_kinematic {

/**
 * @brief The URKinematic class
 *
 * URKimenatic is meant to calculate the kinematics (forward kinematics for all links)
 * for a 6-DOF Universal Robotic Arm.
 *
 * Implementations of forward kinematics (end effector) calculations were borrowed from the ur_moveit package.
 *
 * @see /ur10/ur_moveit/ur_kin.h
 *
 * @author Rodrigo Velasco, Maximilian Krämer
 */

class URKinematic : public RobotKinematic
{
 public:
    using Ptr = std::shared_ptr<URKinematic>;

    URKinematic();

 private:
    double _d1, _a2, _a3, _d4, _d5, _d6;
    double _c0, _s0;
    double _dx, _dy, _dz;

    void calculateKinematicTransformations() override;
    void calculateEndEffectorMatrix() override;

    void forward(const double* q, double* T);
    void parseLinkLengths();
};
}  // namespace robot_kinematic
}  // namespace robot_utilities

#endif  // UR_KINEMATIC_H

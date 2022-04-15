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
 * Authors: Rodrigo Velasco
 * Notes: Inverse kinematic functions were originally authored by
 * Kelsey Hawkins (kphawkins@gatech.edu).
 * Copyright (c) 2013, Georgia Institute of Technology
 *********************************************************************/

#ifndef UR_INVERSE_KINEMATIC_H
#define UR_INVERSE_KINEMATIC_H

#include <robot_utilities/robot_kinematic/robot_inverse_kinematic.h>
#include <robot_utilities/robot_misc/common.h>
#include <ur_utilities/ur_kinematic/ur_kinematic.h>

namespace robot_utilities {
namespace robot_kinematic {

/**
 * @brief The URInverseKinematic class
 *
 * URInvKimenatic is meant to calculate multiple inverse kinematic solutions
 * for a 6-DOF Universal Robotic Arm from the /world frame to the /ee_link frame.
 *
 * Implementations of inverse kinematics calculations were borrowed from the ur_moveit package.
 *
 * @see /ur10/ur_moveit/ur_kin.h
 *
 * @author Rodrigo Velasco, Maximilian Krämer
 */

class URInverseKinematic : public RobotInverseKinematic
{
 public:
    using Ptr = std::shared_ptr<URInverseKinematic>;

    URInverseKinematic();
    URInverseKinematic(robot_kinematic::URKinematic::Ptr ur_kinematic);

    void setDesiredLastJoint(double value);

 private:
    double _q6_desired = 0;
    double _d1, _a2, _a3, _d4, _d5, _d6;

    Eigen::Matrix<double, 9, 1> _links;

    void calculateIKSolution() override;

    void defineTransformation(const Eigen::Ref<const Eigen::Vector3d>& pos, const Eigen::Ref<const Eigen::Vector3d>& rpy, double* T);
    int inverse(const double* T, double* q_sols);
    void parseLinkLengths();
};

}  // namespace robot_kinematic
}  // namespace robot_utilities

#endif  // UR_INVERSE_KINEMATIC_H

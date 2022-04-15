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

#ifndef UR_COLLISION_H
#define UR_COLLISION_H

#include <robot_utilities/robot_collision/robot_collision.h>
#include <ur_utilities/ur_kinematic/ur_kinematic.h>

namespace robot_utilities {
namespace robot_collision {

class URCollision : public RobotCollision
{
 public:
    using Ptr = std::shared_ptr<URCollision>;

    URCollision();
    URCollision(robot_kinematic::URKinematic::Ptr ur_kinematic);

    // Robot - Robot
    bool getSelfCollisionDistances(Eigen::Ref<Eigen::Matrix<double, -1, 1>> d) override;
    bool getSelfCollisionDistances(Eigen::Ref<Eigen::Matrix<double, -1, 1>> d, Eigen::Ref<Eigen::Matrix<double, 3, -1>> start_points,
                                   Eigen::Ref<Eigen::Matrix<double, 3, -1>> end_points) override;

    // Robot - Obstacle
    bool getObstacleDistances(const robot_misc::Obstacle& obstacle, Eigen::Ref<Eigen::Matrix<double, -1, 1>> d, double t = 0) override;
    bool getObstacleDistances(const robot_misc::Obstacle& obstacle, Eigen::Ref<Eigen::Matrix<double, -1, 1>> d,
                              Eigen::Ref<Eigen::Matrix<double, 3, -1>> start_points, Eigen::Ref<Eigen::Matrix<double, 3, -1>> end_points,
                              double t = 0) override;

    // Robot - Plane
    bool getPlaneCollisionDistances(const robot_misc::Plane& plane, Eigen::Ref<Eigen::Matrix<double, -1, 1>> d) override;
    bool getPlaneCollisionDistances(const robot_misc::Plane& plane, Eigen::Ref<Eigen::Matrix<double, -1, 1>> d,
                                    Eigen::Ref<Eigen::Matrix<double, 3, -1>> start_points,
                                    Eigen::Ref<Eigen::Matrix<double, 3, -1>> end_points) override;
};

}  // namespace robot_collision
}  // namespace robot_utilities

#endif  // UR_COLLISION_H

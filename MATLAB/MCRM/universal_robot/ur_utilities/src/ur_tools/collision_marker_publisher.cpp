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

#include <robot_utilities/robot_collision/robot_collision_marker_publisher.h>
#include <ros/ros.h>
#include <ur_utilities/ur_collision/ur_collision.h>

using RobotCollisionMarkerPublisher = robot_utilities::robot_collision::RobotCollisionMarkerPublisher;
using URCollision                   = robot_utilities::robot_collision::URCollision;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ur_collision_marker_publisher");

    ROS_INFO_STREAM("UR Collision Marker Publisher Started...");

    URCollision::Ptr ur_collision = std::make_shared<URCollision>();
    RobotCollisionMarkerPublisher publisher("/joint_states", "/robot_workspace_monitor/obstacles", ur_collision);

    publisher.publish();

    ros::waitForShutdown();
    return 0;
}

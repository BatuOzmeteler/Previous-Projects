/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2015,
 *  TU Dortmund - Institute of Control Theory and Systems Engineering.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the institute nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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
 * Author: Maximilian Krämer
 *********************************************************************/

#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <ur_utilities/ur_misc/ur_utility.h>
#include <sstream>
#include <string>
#include <vector>

using URUtility = robot_utilities::robot_misc::URUtility;

URUtility::Ptr _ur_utility;

std::vector<ros::Publisher>* _pub_vel_ptr;
bool _first_msg = true;

void convert_velocity_cb(const trajectory_msgs::JointTrajectory::ConstPtr& msg)
{
    if (msg->points.size() < 1)
    {
        ROS_ERROR_STREAM(
            "Error in converter node: No velocities given! Message is "
            "not converted!"
            << std::endl);
        return;
    }

    std::vector<double> velocity             = msg->points[0].velocities;
    std::vector<std::string> joint_names_msg = msg->joint_names;

    if (velocity.size() != joint_names_msg.size())
    {
        ROS_ERROR_STREAM(
            "Error in converter node: msg.velocity has hot the same "
            "size as msg.names! Message is not converted!"
            << std::endl);
        return;
    }

    if (_first_msg)
    {
        if (!_ur_utility->initJointMapping(joint_names_msg))
        {
            return;
        }
        _first_msg = false;
    }
    _ur_utility->parseJointStates(velocity, msg->points[0].velocities);

    std_msgs::Float64 send_msg;
    for (int i = 0; i < (int)velocity.size(); ++i)
    {
        send_msg.data = velocity[i];
        (*_pub_vel_ptr)[i].publish(send_msg);
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ur_msgs_translation");
    ros::NodeHandle n("~");

    ROS_INFO("ur_msgs_translation started");

    _ur_utility = std::make_shared<URUtility>();

    // create and fill vector of all publishers
    std::vector<ros::Publisher> pub_vel(_ur_utility->getJointsCount());

    for (int i = 0; i < (int)pub_vel.size(); ++i)
    {
        std::stringstream name_vel;
        name_vel << "joint" << i << "_velocity_controller/command";
        pub_vel[i] = n.advertise<std_msgs::Float64>(name_vel.str(), 1);
    }

    _pub_vel_ptr = &pub_vel;

    ros::Subscriber sub = n.subscribe("joint_speed", 1, &convert_velocity_cb);

    ros::spin();
    return 0;
}

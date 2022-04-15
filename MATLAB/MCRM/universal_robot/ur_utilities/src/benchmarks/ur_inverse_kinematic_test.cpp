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

#include <ros/ros.h>
#include <ur_utilities/ur_kinematic/ur_inverse_kinematic.h>
#include <chrono>

const int ur_ref_ee = 1780;

using URInverseKinematic    = robot_utilities::robot_kinematic::URInverseKinematic;
using RobotInverseKinematic = robot_utilities::robot_kinematic::RobotInverseKinematic;

bool performTest(int iterations, RobotInverseKinematic::Ptr kinematic, const int ref)
{
    Eigen::Vector3d position = Eigen::Vector3d::Zero();
    double roll, pitch, yaw = 0.0;

    std::chrono::steady_clock::time_point begin, end;
    int ns_ee = 0;

    for (int i = 0; i < iterations; ++i)
    {
        position(0) = 0.5;
        position(1) = 0.5;
        position(2) = 1.0;

        roll  = 90;
        pitch = 0;
        yaw   = -90;

        kinematic->setIKAParameters(Eigen::Matrix<double, 6, 1>::Zero(), 5000, 1e-3, 0.1, 0.1, 1 * Eigen::Matrix<double, 6, 1>::Ones(), 0);

        begin = std::chrono::steady_clock::now();
        kinematic->setTaskPoint(position, roll, pitch, yaw);
        end = std::chrono::steady_clock::now();
        ns_ee += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    }

    std::cout << "Endeffector: \t\t" << ns_ee / iterations << " ns. (Ref: " << ref << ")" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    return (ns_ee / iterations) <= ref * 1.05;
}

int main(int argc, char** argv)
{
    // sleep(5);

    ros::init(argc, argv, "ur_inverse_kinematic_test");
    ros::NodeHandle n("~");

    URInverseKinematic::Ptr ur_inverse_kinematic = std::make_shared<URInverseKinematic>();

    int iterations = 100000;
    bool okay      = true;

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Benchmarking URInverseKinematic using " << iterations << " samples:" << std::endl;
    okay &= performTest(iterations, ur_inverse_kinematic, ur_ref_ee);
    std::cout << okay << std::endl;

    return !okay;
}

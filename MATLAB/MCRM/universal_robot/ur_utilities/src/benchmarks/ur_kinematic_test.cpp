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
#include <ur_utilities/ur_kinematic/ur_kinematic.h>
#include <chrono>
#include <random>
//#include <ur_utilities/ur_description/ur_description.h>
//#include <ur_utilities/ur_kinematic/robot_kinematic_kdl.h>

const int ur_ref_ee  = 735;
const int ur_ref_j   = 925;
const int ur_ref_fkc = 785;
const int ur_ref_fkt = 590;

using URKinematic    = robot_utilities::robot_kinematic::URKinematic;
using RobotKinematic = robot_utilities::robot_kinematic::RobotKinematic;

bool performTest(int iterations, RobotKinematic::Ptr kinematic, const std::vector<int>& refs)
{
    Eigen::Matrix<double, 6, 1> q = Eigen::Matrix<double, 6, 1>::Zero();
    Eigen::Matrix<double, 4, 4> T;
    Eigen::Matrix<double, 6, 6> J;
    std::vector<Eigen::Matrix<double, 4, 4>> FK;

    std::chrono::steady_clock::time_point begin, end;
    int ns_ee  = 0;
    int ns_j   = 0;
    int ns_fkc = 0;
    int ns_fkt = 0;

    std::uniform_real_distribution<double> random(-180, 180);
    std::default_random_engine re;

    for (int i = 0; i < iterations; ++i)
    {
        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);

        begin = std::chrono::steady_clock::now();
        kinematic->getEndEffectorMatrix(q, T);
        end = std::chrono::steady_clock::now();
        ns_ee += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);

        begin = std::chrono::steady_clock::now();
        kinematic->getGeometricJacobian(q, J);
        end = std::chrono::steady_clock::now();
        ns_j += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);

        begin = std::chrono::steady_clock::now();
        kinematic->getForwardKinematicChain(q, FK);
        end = std::chrono::steady_clock::now();
        ns_fkc += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);

        begin = std::chrono::steady_clock::now();
        kinematic->getForwardKinematicTransformations(q, FK);
        end = std::chrono::steady_clock::now();
        ns_fkt += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    }

    std::cout << "Endeffector: \t\t" << ns_ee / iterations << " ns. (Ref: " << refs[0] << ")" << std::endl;
    std::cout << "Jacobian: \t\t" << ns_j / iterations << " ns. (Ref: " << refs[1] << ")" << std::endl;
    std::cout << "Chain: \t\t\t" << ns_fkc / iterations << " ns. (Ref: " << refs[2] << ")" << std::endl;
    std::cout << "Transformations: \t" << ns_fkt / iterations << " ns. (Ref: " << refs[3] << ")" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    return ((ns_ee / iterations) <= refs[0] * 1.05) && ((ns_j / iterations) <= refs[1] * 1.05) && ((ns_fkc / iterations) <= refs[2] * 1.05) &&
           ((ns_fkt / iterations) <= refs[3] * 1.05);
}

int main(int argc, char** argv)
{
    // sleep(5);

    ros::init(argc, argv, "ur_kinematic_test");
    ros::NodeHandle n("~");

    URKinematic::Ptr ur_kinematic = std::make_shared<URKinematic>();
    // robot_utilities::robot_description::URDescription robot_description("/robot_description", "world", "ee_link");
    // ur_utilities::ur_kinematic::RobotKinematicKDL kdl_kine(robot_description.getURDFModel());

    int iterations           = 100000;
    bool okay                = true;
    std::vector<int> ur_refs = {ur_ref_ee, ur_ref_j, ur_ref_fkc, ur_ref_fkt};

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Benchmarking URKinematic using " << iterations << " samples:" << std::endl;
    okay &= performTest(iterations, ur_kinematic, ur_refs);
    std::cout << okay << std::endl;

    /*
    KDL::JntArray kdl_q(6);
    kdl_q(0) = q(0);
    kdl_q(1) = q(1);
    kdl_q(2) = q(2);
    kdl_q(3) = q(3);
    kdl_q(4) = q(4);
    kdl_q(5) = q(5);

    ns = 0;
    KDL::Frame T;
    for (int i = 0; i < iterations; ++i)
    {
        q(0) = (rand() % 200 - 100) * M_PI / 100;
        q(1) = (rand() % 200 - 100) * M_PI / 100;
        q(2) = (rand() % 200 - 100) * M_PI / 100;
        q(3) = (rand() % 200 - 100) * M_PI / 100;
        q(4) = (rand() % 200 - 100) * M_PI / 100;
        q(5) = (rand() % 200 - 100) * M_PI / 100;

        begin = std::chrono::steady_clock::now();
        T     = kdl_kine.solve(kdl_q);
        end   = std::chrono::steady_clock::now();
        ns += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    }

    // std::cout << T.p.data[0] << "," << T.p.data[1] << "," << T.p.data[2] << std::endl;
    std::cout << "Calculation Time: " << ns / iterations << " ns" << std::endl;
    std::cout << "-----------------------" << std::endl;
    */

    // Debug robot kinematic with different frames
    /*
    using URDescription = robot_utilities::robot_description::URDescription;
    using URUtility     = robot_utilities::robot_misc::URUtility;

    URDescription::Ptr urd = std::make_shared<URDescription>("/robot_description", "world", "new_link2");
    URUtility::Ptr uru     = std::make_shared<URUtility>(urd);
    RobotKinematic::Ptr rk = std::make_shared<RobotKinematic>(uru);

    rk->setJointState(0.0 * Eigen::VectorXd::Ones(6));

    Eigen::Matrix4d tf_matrix = robot_utilities::robot_misc::Common::getTransformation("world", "new_link2");
    Eigen::Matrix4d rk_matrix = rk->getEndEffectorMatrixCopy();

    std::cout << "TF: " << std::endl;
    std::cout << tf_matrix << std::endl << std::endl;

    std::cout << "RK: " << std::endl;
    std::cout << rk_matrix << std::endl << std::endl;
    */

    return !okay;
}

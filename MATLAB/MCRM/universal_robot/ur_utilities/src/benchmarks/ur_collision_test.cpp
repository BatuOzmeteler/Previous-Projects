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
#include <ur_utilities/ur_collision/ur_collision.h>
#include <chrono>
#include <random>

const int max = 10000;

const int ur_ref_self      = max;
const int ur_ref_self_min  = 1650;
const int ur_ref_obs       = max;
const int ur_ref_obs_min   = 1400;
const int ur_ref_hum_min   = max;
const int ur_ref_head      = max;
const int ur_ref_head_min  = max;
const int ur_ref_plane     = max;
const int ur_ref_plane_min = max;

const int robot_ref_links         = 1370;
const int robot_ref_obs           = 1370;
const int robot_ref_plane         = max;
const int robot_ref_oobs          = 195;
const int robot_ref_plane_obs     = max;
const int robot_ref_human_obs     = max;
const int robot_ref_human_obs_min = max;

using Obstacle       = robot_utilities::robot_misc::Obstacle;
using URCollision    = robot_utilities::robot_collision::URCollision;
using RobotCollision = robot_utilities::robot_collision::RobotCollision;
using Human          = robot_utilities::robot_misc::Human;
using Plane          = robot_utilities::robot_misc::Plane;

void getObstacle(Obstacle& obs)
{
    obs.bounding_box.length_x = 0.5;
    obs.bounding_box.radius   = 0.1;
    obs.bounding_box.type     = robot_utilities::robot_misc::BoundingBoxType::CYLINDER;
    obs.bounding_box.T.block<3, 1>(0, 3) << (rand() % 5), 1.0, 0.9;
}

void getHuman(Human& human)
{
    Obstacle obs;
    getObstacle(obs);

    human.body_parts["Head"]  = obs;
    human.body_parts["Beck"]  = obs;
    human.body_parts["Hip"]   = obs;
    human.body_parts["RUArm"] = obs;
    human.body_parts["LUArm"] = obs;
    human.body_parts["RFArm"] = obs;
    human.body_parts["LFArm"] = obs;
}

void getPlane(Plane& plane)
{
    plane.n << 0, 0, 1;
    plane.q << 0, 0, 0;
}

bool performRobotCollisionTest(int iterations, RobotCollision::Ptr collision)
{
    Eigen::Matrix<double, 6, 1> q = Eigen::Matrix<double, 6, 1>::Zero();
    Obstacle obstacle1;
    Obstacle obstacle2;
    Human human;
    Plane plane;

    double d;
    Eigen::Matrix<double, 6, 1> dist;

    getObstacle(obstacle1);
    getObstacle(obstacle2);
    getHuman(human);
    getPlane(plane);

    std::chrono::steady_clock::time_point begin, end;
    int ns_links         = 0;
    int ns_obs           = 0;
    int ns_plane         = 0;
    int ns_oobs          = 0;
    int ns_plane_obs     = 0;
    int ns_human_obs     = 0;
    int ns_human_obs_min = 0;

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
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getDistanceBetweenLinks(d, 0, 5);
        end = std::chrono::steady_clock::now();
        ns_links += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getDistanceToObstacle(d, 5, obstacle1);
        end = std::chrono::steady_clock::now();
        ns_obs += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getDistanceToPlane(d, 5, plane);
        end = std::chrono::steady_clock::now();
        ns_plane += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getDistanceBetweenObstacles(d, obstacle1, obstacle2);
        end = std::chrono::steady_clock::now();
        ns_oobs += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getDistanceBetweenPlaneObstacle(d, plane, obstacle1);
        end = std::chrono::steady_clock::now();
        ns_plane_obs += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getDistanceBetweenHumanObstacle(dist, obstacle1, human);
        end = std::chrono::steady_clock::now();
        ns_human_obs += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getMinDistanceBetweenHumanObstacle(d, obstacle1, human);
        end = std::chrono::steady_clock::now();
        ns_human_obs_min += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    }

    std::cout << "getDistanceBetweenLinks: \t\t" << ns_links / iterations << " ns. \t(Ref: " << robot_ref_links << ")" << std::endl;
    std::cout << "getDistanceToObstacle: \t\t\t" << ns_obs / iterations << " ns. \t(Ref: " << robot_ref_obs << ")" << std::endl;
    std::cout << "getDistanceToPlane: \t\t\t" << ns_plane / iterations << " ns. \t(Ref: " << robot_ref_plane << ")" << std::endl;
    std::cout << "getDistanceBetweenObstacles: \t\t" << ns_oobs / iterations << " ns. \t\t(Ref: " << robot_ref_oobs << ")" << std::endl;
    std::cout << "getDistanceBetweenPlaneObstacle: \t" << ns_plane_obs / iterations << " ns. \t\t(Ref: " << robot_ref_plane_obs << ")" << std::endl;
    std::cout << "getDistanceBetweenHumanObstacle: \t" << ns_human_obs / iterations << " ns. \t(Ref: " << robot_ref_human_obs << ")" << std::endl;
    std::cout << "getMinDistanceBetweenHumanObstacle: \t" << ns_human_obs_min / iterations << " ns. \t(Ref: " << robot_ref_human_obs_min << ")"
              << std::endl;

    std::cout << "--------------------------------------------------------------------" << std::endl;

    return ((ns_obs / iterations) <= robot_ref_obs * 1.05) && ((ns_links / iterations) <= robot_ref_links * 1.05) &&
           ((ns_oobs / iterations) <= robot_ref_oobs * 1.05) && ((ns_plane / iterations) <= robot_ref_plane * 1.05) &&
           ((ns_plane_obs / iterations) <= robot_ref_plane_obs * 1.05) && ((ns_human_obs / iterations) <= robot_ref_human_obs * 1.05) &&
           ((ns_human_obs_min / iterations) <= robot_ref_human_obs_min * 1.05);
}

bool performURCollisionTest(int iterations, URCollision::Ptr collision)
{
    Eigen::Matrix<double, 6, 1> q = Eigen::Matrix<double, 6, 1>::Zero();
    Obstacle obstacle1;
    Obstacle obstacle2;
    Human human;
    Plane plane;

    Eigen::Matrix<double, 8, 1> dist;

    getObstacle(obstacle1);
    getObstacle(obstacle2);
    getHuman(human);
    getPlane(plane);

    std::chrono::steady_clock::time_point begin, end;
    int ns_self      = 0;
    int ns_self_min  = 0;
    int ns_obs       = 0;
    int ns_obs_min   = 0;
    int ns_hum_min   = 0;
    int ns_head      = 0;
    int ns_head_min  = 0;
    int ns_plane     = 0;
    int ns_plane_min = 0;

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
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getSelfCollisionDistances(dist);
        end = std::chrono::steady_clock::now();
        ns_self += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getMinSelfCollisionDistance();
        end = std::chrono::steady_clock::now();
        ns_self_min += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getObstacleDistances(obstacle1, dist);
        end = std::chrono::steady_clock::now();
        ns_obs += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getMinObstacleDistance(obstacle1);
        end = std::chrono::steady_clock::now();
        ns_obs_min += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getMinHumanDistance(human, dist);
        end = std::chrono::steady_clock::now();
        ns_hum_min += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getHumanHeadDistances(human, dist);
        end = std::chrono::steady_clock::now();
        ns_head += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getMinHumanHeadDistance(human);
        end = std::chrono::steady_clock::now();
        ns_head_min += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getPlaneCollisionDistances(plane, dist);
        end = std::chrono::steady_clock::now();
        ns_plane += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

        q(0) = random(re);
        q(1) = random(re);
        q(2) = random(re);
        q(3) = random(re);
        q(4) = random(re);
        q(5) = random(re);
        collision->setJointState(q);

        begin = std::chrono::steady_clock::now();
        collision->getMinPlaneCollisionDistance(plane);
        end = std::chrono::steady_clock::now();
        ns_plane_min += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    }
    std::cout << "getSelfCollisionDistances: \t\t" << ns_self / iterations << " ns. \t(Ref: " << ur_ref_self << ")" << std::endl;
    std::cout << "getMinSelfCollisionDistance: \t\t" << ns_self_min / iterations << " ns. \t(Ref: " << ur_ref_self_min << ")" << std::endl;
    std::cout << "getObstacleDistances: \t\t\t" << ns_obs / iterations << " ns. \t(Ref: " << ur_ref_obs << ")" << std::endl;
    std::cout << "getMinObstacleDistance: \t\t" << ns_obs_min / iterations << " ns. \t(Ref: " << ur_ref_obs_min << ")" << std::endl;
    std::cout << "getMinHumanDistance: \t\t\t" << ns_hum_min / iterations << " ns. \t(Ref: " << ur_ref_hum_min << ")" << std::endl;
    std::cout << "getHumanHeadDistances: \t\t\t" << ns_head / iterations << " ns. \t(Ref: " << ur_ref_head << ")" << std::endl;
    std::cout << "getMinHumanHeadDistance: \t\t" << ns_head_min / iterations << " ns. \t(Ref: " << ur_ref_head_min << ")" << std::endl;
    std::cout << "getPlaneCollisionDistances: \t\t" << ns_plane / iterations << " ns. \t(Ref: " << ur_ref_plane << ")" << std::endl;
    std::cout << "getMinPlaneCollisionDistance: \t\t" << ns_plane_min / iterations << " ns. \t(Ref: " << ur_ref_plane_min << ")" << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;

    return ((ns_obs / iterations) <= ur_ref_obs * 1.05) && ((ns_self / iterations) <= ur_ref_self * 1.05);
}

int main(int argc, char** argv)
{
    // sleep(5);

    ros::init(argc, argv, "ur_collision_test");

    URCollision::Ptr ur_collision = std::make_shared<URCollision>();

    int iterations = 100000;
    bool okay      = true;

    ros::Duration(1).sleep();

    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "Benchmarking URCollision using " << iterations << " samples:" << std::endl << std::endl;
    okay &= performURCollisionTest(iterations, ur_collision);
    std::cout << okay << std::endl;

    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "Benchmarking RobotCollision using " << iterations << " samples:" << std::endl << std::endl;
    okay &= performRobotCollisionTest(iterations, ur_collision);
    std::cout << okay << std::endl;

    ur_collision->performBenchmark();

    return !okay;
}

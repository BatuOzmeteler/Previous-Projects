#include <ros/io.h>
#include <ur_utilities/ur_collision/ur_collision.h>

namespace robot_utilities {
namespace robot_collision {

URCollision::URCollision() : RobotCollision(std::make_shared<robot_kinematic::URKinematic>(), 6, 5, 8) {}

URCollision::URCollision(robot_kinematic::URKinematic::Ptr ur_kinematic) : RobotCollision(ur_kinematic, 6, 5, 8) {}

bool URCollision::getSelfCollisionDistances(Eigen::Ref<Eigen::Matrix<double, -1, 1>> d)
{
    bool success = true;

    // using +1 on all links starting with 1, because 1 is inactive
    success &= getDistanceBetweenLinks(d(0), 4, 0);
    success &= getDistanceBetweenLinks(d(1), 4, 2);
    success &= getDistanceBetweenLinks(d(2), 5, 3);
    success &= getDistanceBetweenLinks(d(3), 6, 3);
    success &= getDistanceBetweenLinks(d(4), 7, 0);
    success &= getDistanceBetweenLinks(d(5), 7, 2);
    success &= getDistanceBetweenLinks(d(6), 7, 3);
    success &= getDistanceBetweenLinks(d(7), 7, 4);

    if (!success)
    {
        ROS_WARN("Error calculating self collision distances.");
    }

    return success;
}

bool URCollision::getSelfCollisionDistances(Eigen::Ref<Eigen::Matrix<double, -1, 1>> d, Eigen::Ref<Eigen::Matrix<double, 3, -1>> start_points,
                                            Eigen::Ref<Eigen::Matrix<double, 3, -1>> end_points)
{
    bool success = true;

    // using +1 on all links starting with 1, because 1 is inactive
    success &= getDistanceBetweenLinks(d(0), 4, 0, start_points.col(0), end_points.col(0));
    success &= getDistanceBetweenLinks(d(1), 4, 2, start_points.col(1), end_points.col(1));
    success &= getDistanceBetweenLinks(d(2), 5, 3, start_points.col(2), end_points.col(2));
    success &= getDistanceBetweenLinks(d(3), 6, 3, start_points.col(3), end_points.col(3));
    success &= getDistanceBetweenLinks(d(4), 7, 0, start_points.col(4), end_points.col(4));
    success &= getDistanceBetweenLinks(d(5), 7, 2, start_points.col(5), end_points.col(5));
    success &= getDistanceBetweenLinks(d(6), 7, 3, start_points.col(6), end_points.col(6));
    success &= getDistanceBetweenLinks(d(7), 7, 4, start_points.col(7), end_points.col(7));

    if (!success)
    {
        ROS_WARN("Error calculating self collision distances.");
    }

    return success;
}

bool URCollision::getObstacleDistances(const robot_misc::Obstacle& obstacle, Eigen::Ref<Eigen::Matrix<double, -1, 1>> d, double t)
{
    bool success = true;

    // skip 0 because it cannot move anyway and 1 because it is inactive
    success &= getDistanceToObstacle(d(0), 2, obstacle, t);
    success &= getDistanceToObstacle(d(1), 3, obstacle, t);
    success &= getDistanceToObstacle(d(2), 4, obstacle, t);
    success &= getDistanceToObstacle(d(3), 5, obstacle, t);
    success &= getDistanceToObstacle(d(4), 6, obstacle, t);
    success &= getDistanceToObstacle(d(5), 7, obstacle, t);

    if (!success)
    {
        ROS_WARN("Error calculating distances to obstacle.");
    }

    return success;
}

bool URCollision::getObstacleDistances(const robot_misc::Obstacle& obstacle, Eigen::Ref<Eigen::Matrix<double, -1, 1>> d,
                                       Eigen::Ref<Eigen::Matrix<double, 3, -1>> start_points, Eigen::Ref<Eigen::Matrix<double, 3, -1>> end_points,
                                       double t)
{
    bool success = true;

    // skip 0 because it cannot move anyway and 1 because it is inactive
    success &= getDistanceToObstacle(d(0), 2, obstacle, start_points.col(0), end_points.col(0), t);
    success &= getDistanceToObstacle(d(1), 3, obstacle, start_points.col(1), end_points.col(1), t);
    success &= getDistanceToObstacle(d(2), 4, obstacle, start_points.col(2), end_points.col(2), t);
    success &= getDistanceToObstacle(d(3), 5, obstacle, start_points.col(3), end_points.col(3), t);
    success &= getDistanceToObstacle(d(4), 6, obstacle, start_points.col(4), end_points.col(4), t);
    success &= getDistanceToObstacle(d(5), 7, obstacle, start_points.col(5), end_points.col(5), t);

    if (!success)
    {
        ROS_WARN("Error calculating distances to obstacle.");
    }

    return success;
}

bool URCollision::getPlaneCollisionDistances(const robot_misc::Plane& plane, Eigen::Ref<Eigen::Matrix<double, -1, 1>> d,
                                             Eigen::Ref<Eigen::Matrix<double, 3, -1>> start_points,
                                             Eigen::Ref<Eigen::Matrix<double, 3, -1>> end_points)
{
    bool success = true;

    // skip 0 because it can not move anyway, 1 because it is inactive and 2 because it cannot collide with a plane
    success &= getDistanceToPlane(d(0), 3, plane, start_points.col(0), end_points.col(0));
    success &= getDistanceToPlane(d(1), 4, plane, start_points.col(1), end_points.col(1));
    success &= getDistanceToPlane(d(2), 5, plane, start_points.col(2), end_points.col(2));
    success &= getDistanceToPlane(d(3), 6, plane, start_points.col(3), end_points.col(3));
    success &= getDistanceToPlane(d(4), 7, plane, start_points.col(4), end_points.col(4));

    if (!success)
    {
        ROS_WARN("Error calculating distances to plane.");
    }

    return success;
}

bool URCollision::getPlaneCollisionDistances(const robot_misc::Plane& plane, Eigen::Ref<Eigen::Matrix<double, -1, 1>> d)
{
    bool success = true;

    // skip 0 because it can not move anyway, 1 because it is inactive and 2 because it cannot collide with a plane
    success &= getDistanceToPlane(d(0), 3, plane);
    success &= getDistanceToPlane(d(1), 4, plane);
    success &= getDistanceToPlane(d(2), 5, plane);
    success &= getDistanceToPlane(d(3), 6, plane);
    success &= getDistanceToPlane(d(4), 7, plane);

    if (!success)
    {
        ROS_WARN("Error calculating distances to plane.");
    }

    return success;
}

}  // namespace robot_collision
}  // namespace robot_utilities

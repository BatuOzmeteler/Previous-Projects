#include <controller_manager_msgs/SwitchController.h>
#include <gazebo_msgs/SetModelConfiguration.h>
#include <ur_utilities/ur_manipulator/ur_manipulator.h>
#include <chrono>

namespace robot_utilities {
namespace robot_manipulator {

URManipulator::URManipulator() {}

bool URManipulator::initialize(bool real_robot, bool reset_on_start, robot_misc::URUtility::Ptr ur_utility)
{
    if (!RobotManipulator::initialize(real_robot, reset_on_start, ur_utility)) return false;

    _ur_utility = ur_utility;

    ros::NodeHandle n("~");

    std::string joint_states_topic   = "";
    std::string joint_speed_topic    = "";
    std::string joint_position_topic = "";

    if (_real_robot)
    {
        // hardcoded topic since real robot does not differ in robot ns
        joint_states_topic = "/ur_driver/joint_states";

        // hardcoded topic since real robot does not differ in robot ns
        joint_speed_topic = "/ur_driver/joint_speed";

        // hardcoded topic since real robot does not differ in robot ns
        joint_position_topic = "/ur_driver/vel_based_pos_traj_controller";
    }
    else
    {
        // will be remapped by launch file to ur3/ur10 ns
        joint_states_topic = "/joint_states";

        // will be remapped by launch file to ur3/ur10 ns
        joint_speed_topic = "joint_speed_controls";

        // will be remapped by launch file to ur3/ur10 ns
        joint_position_topic = "joint_position_controls";
    }

    _joint_states_sub = n.subscribe(joint_states_topic, 1, &RobotManipulator::jointStateCallback, dynamic_cast<RobotManipulator*>(this),
                                    ros::TransportHints().tcpNoDelay());
    ROS_INFO_STREAM("URManipulator: listening for joint states on topic " << joint_states_topic << ".");

    _joint_speed_pub = n.advertise<trajectory_msgs::JointTrajectory>(joint_speed_topic, 1);
    ROS_INFO_STREAM("URManipulator: sending joint speeds on topic " << joint_speed_topic << ".");

    _joint_position_pub = n.advertise<trajectory_msgs::JointTrajectory>(joint_position_topic, 1);
    ROS_INFO_STREAM("URManipulator: sending joint positions on topic " << joint_position_topic << ".");

    if (!ros::topic::waitForMessage<sensor_msgs::JointState>(joint_states_topic, ros::Duration(5)))
    {
        ROS_WARN_STREAM("URManipulator: " << joint_states_topic << " topic not published.");

        _initialized = false;
        return false;
    }

    std::string switch_controller_service = "";
    if (_real_robot)
    {
        // hardcoded topic since real robot does not differ in robot ns
        switch_controller_service = "/ur_driver/controller_manager/switch_controller";
    }
    else
    {
        // will be remapped by launch file to ur3/ur10 ns
        switch_controller_service = "/controller_manager/switch_controller";
    }
    _controller_switch_srv = n.serviceClient<controller_manager_msgs::SwitchController>(switch_controller_service);

    if (!_controller_switch_srv.waitForExistence(ros::Duration(5)))
    {
        ROS_WARN_STREAM("URManipulator: " << switch_controller_service << " service not avaiable.");

        _initialized = false;
        return false;
    }

    // Stop any lingering robot motion
    setVelocityMode();
    stop();

    _initialized = true;
    ROS_INFO("URManipulator initialized.");

    return true;
}

int URManipulator::getJointNumber() const { return 6; }

void URManipulator::stop()
{
    _publish_flag = false;
    if (_publish_worker.joinable()) _publish_worker.join();

    // Check mode (position/velocity) and stop accordingly
    if (_mode == Mode::VELOCITY)
    {
        stopJointVelocity();
    }
    else if (_mode == Mode::POSITION)
    {
        stopJointPosition();
    }
    else
    {
        // TODO (kraemer) Fatal
    }
}

bool URManipulator::setVelocityMode()
{
    if (_mode == Mode::VELOCITY) return true;

    if (!_controller_switch_srv.exists())
    {
        ROS_ERROR("URManipulator: Cannot switch controllers to velocity mode, since service is not avaiable");
        return false;
    }

    controller_manager_msgs::SwitchController switch_msg;
    if (_real_robot)
    {
        switch_msg.request.stop_controllers = _ur_utility->getPositionControllerNames();
    }
    else
    {
        switch_msg.request.start_controllers = _ur_utility->getVelocityControllerNames();
        switch_msg.request.stop_controllers  = _ur_utility->getPositionControllerNames();
    }

    switch_msg.request.strictness = 1;

    if (_controller_switch_srv.call(switch_msg) && switch_msg.response.ok)
    {
        _mode = Mode::VELOCITY;
        ROS_INFO("URManipulator: Switched to velocity control mode.");
        return true;
    }
    else
    {
        ROS_ERROR("URManipulator: Failed to switch to velocity control mode");
        return false;
    }
}

bool URManipulator::setPositionMode()
{
    if (_mode == Mode::POSITION) return true;

    if (!_controller_switch_srv.exists())
    {
        ROS_ERROR("URManipulator: Cannot switch controllers to position mode, since service is not avaiable");
        return false;
    }

    controller_manager_msgs::SwitchController switch_msg;
    if (_real_robot)
    {
        switch_msg.request.start_controllers = _ur_utility->getPositionControllerNames();
    }
    else
    {
        switch_msg.request.stop_controllers  = _ur_utility->getVelocityControllerNames();
        switch_msg.request.start_controllers = _ur_utility->getPositionControllerNames();
    }

    switch_msg.request.strictness = 1;

    if (_controller_switch_srv.call(switch_msg) && switch_msg.response.ok)
    {
        _mode = Mode::POSITION;
        ROS_INFO("URManipulator: Switched to position mode.");
        return true;
    }
    else
    {
        ROS_ERROR("URManipulator: Failed to switch to position mode");
        return false;
    }
}

void URManipulator::sendSingleJointVelocityCommand(const Eigen::Ref<const Eigen::VectorXd>& command)
{
    trajectory_msgs::JointTrajectory jt;
    setUpJointVelocityCommand(jt, command);
    _joint_speed_pub.publish(jt);
}

void URManipulator::sendMultipleJointVelocityCommands(const Eigen::Ref<const Eigen::MatrixXd>& u, const std::vector<double>& time, int n_points,
                                                      int start_index)
{
    std::vector<trajectory_msgs::JointTrajectory> trajectory_msgs;
    std::vector<double> time_vector;

    trajectory_msgs.resize(n_points);
    time_vector.resize(n_points);

    for (int i = 0; i < n_points; ++i)
    {
        setUpJointVelocityCommand(trajectory_msgs[i], u.col(start_index + i));

        if (i + 1 >= n_points)
        {
            // Last command copies second last command
            time_vector[i] = time_vector[i - 1];
        }
        else
        {
            time_vector[i] = 1000 * (time[start_index + 1 + i] - time[start_index + i]);
        }
    }

    _mutex.lock();
    _trajectory_msgs = trajectory_msgs;
    _time_vector     = time_vector;
    _new_sequence    = true;
    _mutex.unlock();

    // Start thread
    if (!_publish_flag)
    {
        _publish_flag   = true;
        _publish_worker = std::thread(&URManipulator::publishJointVelocityCommands, this);
    }
}

void URManipulator::sendSingleJointPositionCommand(const Eigen::Ref<const Eigen::VectorXd>& command, double time)
{
    trajectory_msgs::JointTrajectory jt;
    jt.joint_names  = _ur_utility->getJointNames();
    jt.header.stamp = ros::Time::now();

    addJointPositionCommand(jt, command, time);

    _joint_position_pub.publish(jt);
}

void URManipulator::sendMultipleJointPositionCommands(const Eigen::Ref<const Eigen::MatrixXd>& q, const std::vector<double>& time, int n_points,
                                                      int start_index)
{
    trajectory_msgs::JointTrajectory jt;
    jt.joint_names  = _ur_utility->getJointNames();
    jt.header.stamp = ros::Time::now();

    for (int i = start_index; i < n_points; ++i)
    {
        addJointPositionCommand(jt, q.col(i), time[i]);
    }

    _joint_position_pub.publish(jt);
}

void URManipulator::stopJointVelocity()
{
    trajectory_msgs::JointTrajectory jt;
    setUpJointVelocityCommand(jt, Eigen::VectorXd::Zero(getJointNumber()));
    _joint_speed_pub.publish(jt);
}

void URManipulator::stopJointPosition()
{
    // Send empty trajectory to trigger position hold mode
    trajectory_msgs::JointTrajectory jt;
    _joint_position_pub.publish(jt);
}

void URManipulator::setUpJointVelocityCommand(trajectory_msgs::JointTrajectory& msg, const Eigen::Ref<const Eigen::VectorXd>& joint_speed)
{
    msg.joint_names = _ur_utility->getJointNames();

    msg.points.resize(1);
    msg.points[0].accelerations = _default_acceleration;
    msg.points[0].velocities.resize(joint_speed.size());

    Eigen::VectorXd::Map(&msg.points[0].velocities[0], joint_speed.size()) = joint_speed;

    msg.header.stamp = ros::Time::now();
}

void URManipulator::addJointPositionCommand(trajectory_msgs::JointTrajectory& msg, const Eigen::Ref<const Eigen::VectorXd>& joint_position,
                                            double time)
{
    trajectory_msgs::JointTrajectoryPoint point;

    point.time_from_start = ros::Duration(time);
    point.positions.resize(joint_position.size());

    Eigen::VectorXd::Map(&point.positions[0], joint_position.size()) = joint_position;

    msg.points.push_back(point);
}

void URManipulator::publishJointVelocityCommands()
{
    std::chrono::steady_clock::time_point start, end;
    int idx     = 0;
    double time = 0.0;

    while (_publish_flag && ros::ok())
    {
        start = std::chrono::steady_clock::now();

        _mutex.lock();

        if (_new_sequence)
        {
            idx           = 0;
            _new_sequence = false;
        }

        if (idx < _trajectory_msgs.size())
        {
            _trajectory_msgs[idx].header.stamp = ros::Time::now();

            if (_publish_flag)
            {
                _joint_speed_pub.publish(_trajectory_msgs[idx]);
            }
            else
            {
                break;
            }
        }
        else
        {
            idx = _trajectory_msgs.size() - 1;
        }

        time = _time_vector[idx];
        _mutex.unlock();

        end = std::chrono::steady_clock::now();

        if (_publish_flag)
        {
            int delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            if (delay < time) std::this_thread::sleep_for(std::chrono::milliseconds((int)time - delay));
        }
        else
        {
            break;
        }

        ++idx;
    }
}

}  // namespace robot_manipulator
}  // namespace robot_utilities

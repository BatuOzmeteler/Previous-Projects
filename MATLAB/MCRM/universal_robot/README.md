# Universal Robot

This repository contains metapackages and files for installation/usage of the Universal Robot.

The following steps help you to set up the robots repositories and how to use the tools of this repository in particular.

## License

The UR10 package contains modifications that are currently not released.
Redistribution and use of this version of the UR10 package in source and binary forms, with or without modification, are **prohibited**.

Copyright (c) 2019, TU Dortmund - Lehrstuhl RST

## Status

* master branch  &nbsp;&nbsp;[![build status](https://git.rst.e-technik.tu-dortmund.de/robotics-rigid-arms/ur10/badges/master/pipeline.svg)](https://git.rst.e-technik.tu-dortmund.de/robotics-rigid-arms/ur10/commits/master)
* develop branch [![build status](https://git.rst.e-technik.tu-dortmund.de/robotics-rigid-arms/ur10/badges/develop/pipeline.svg)](https://git.rst.e-technik.tu-dortmund.de/robotics-rigid-arms/ur10/commits/develop)

Currently supported ROS distributions:
* ``melodic`` (recommended)
* ``kinetic`` (EoL)

## Setup

Dont forget to source the catkin workspace:

    source ~/catkin_ws/devel/setup.bash

See [here](http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment) for additional information about how to initialize a catkin workspace.

Clone this repo into the ``src`` folder of the catkin workspace:

    cd ~/catkin_ws/src

    git clone https://<username>@git.rst.e-technik.tu-dortmund.de/robotics-rigid-arms/ur10.git

Also install the ``robot_utilities`` [package](http://robotics-rigid-arms.pages.rst.e-technik.tu-dortmund.de/robot_utilities/robot_utilities-melodic.zip) **or** clone the repo into the ``src`` folder of the catkin workspace:

    sudo apt install <path_to_deb_file>

    OR

    git clone https://<username>@git.rst.e-technik.tu-dortmund.de/robotics-rigid-arms/robot_utilities.git

Please note, if you are not going to change the ``robot_utilities`` package, use the linked .deb file instead of cloning. For a username and password ask your supervisor or the maintainer of this package. Lab computers usually have this package installed already.

Install missing dependencies and check if everything compiles:

    rosdep install --from-paths ./ --ignore-src --rosdistro melodic -y
    cd ..
    catkin_make

## Universal Robot Simulation (Visualization Only)

In the following you can replace urX with ur10 or ur3 depending on what robot you want to use.
In order to just visualize the robot and publish some joint states over a joint state publisher gui use the following launch files depending on what setup you want to use:

    roslaunch ur_launch urX_sim_visualization.launch
    roslaunch ur_launch urX_sim_visualization_obstacle.launch

The launch files starts RViz with a predefined setup, joint publisher GUI, and headles publisher for robot state and marker.
The robot uses the respective gripper (140 for UR10, 85 for UR3). When using the obstacle mode, make sure to configure them obstacles in ``robot_utilities/config/obstacles/static_obstacles.yaml``.

## Universal Robot Simulation (Gazebo)

In the following you can replace urX with ur10 or ur3 depending on what robot you want to use.
In order to simulate the robot in Gazebo use the following launch files depending on what setup you want to use:

    roslaunch ur_launch urX_sim_gazebo.launch
    roslaunch ur_launch urX_sim_gazebo_obstacle.launch

    roslaunch ur_launch ur3_sim_gazebo_visual.launch

The launch files starts Gazebo, RViz with a predefined setup, rqt GUI, ROS Controllers with interfaces, and headles publisher for robot state and marker.
The robot uses the respective gripper (140 for UR10, 85 for UR3). When using the obstacle mode, make sure to configure them obstacles in ``robot_utilities/config/obstacles/static_obstacles.yaml``. Currently, only the UR3 supports the camera with ar marker tracking using ar_pose. The pose information is published to the topic ``/ar_pose_marker``.

### Position and Trajectory Control

By default a ``joint_trajectory_controller`` is started that can be used by the launched rqt_gui for approaching configurations or following joint space trajectories. This also holds for the gripper.

    /URX/vel_based_pos_traj_controller/
    /URX/gripper/

Inside your own programm you should use the action server to communicate with the ``joint_trajectory_controller``.
For the gripper trajectory points can reach from -0.4 (open) to 0.4 (close) for the UR10 and from 0.0 (open) to 0.8 (close) for the UR3.
See [here](http://wiki.ros.org/joint_trajectory_controller?distro=kinetic) for additional information about joint trajectory controller.


### Velocity Control

To control joint velocities there is a JointTrajectory topic for sending joint speeds:

    /URX/joint_speed

It takes messages of type ``trajectory_msgs/JointTrajectory`` and parses the first JointTracetoryPoint and sends the specified joint speeds and accelerations to the robot.
Remember to set values for all 6 joints. Ignores the field joint_names, so set the values in the correct order.
To switch from position controller to velocity controller send a switch service command on the following topic:

    /URX/controller_manager/switch_controller

The command contains:
- start_controllers:    ['joint0_velocity_controller','joint1_velocity_controller','joint2_velocity_controller','joint3_velocity_controller','joint4_velocity_controller','joint5_velocity_controller']
- stop_controllers:    ['vel_based_pos_traj_controller']

And vice versa to go back to position control.
See [here](http://wiki.ros.org/controller_manager?distro=kinetic) for additional information on how to switch between position/trajectory and velocity controllers.


## Using Real Universal Robot

Remember that you should always have your hands on the emergency button in case there is anything unexpected happening.
In this meta package we use modified version of the ``ur_modern_driver`` package from [here](https://github.com/ThomasTimm/ur_modern_driver). Visit for additional information.

Start the robot via the teach pendant and wait until it finished booting.
If not already done, initialize the robot by clicking ``Setup Robot`` and ``Initialize Robot``.
Refer to the manual next to the robot for more details.

Use the following launch files to start the robot driver and visualization tools:

    roslaunch ur_launch urX_real_visualization.launch
    roslaunch ur_launch urX_real_visualization_obstacle.launch

### Position and Trajectory Control

By default a ``joint_trajectory_controller`` is started that can be used by the launched rqt_gui for approaching configurations or following joint space trajectories:

    /ur_driver/vel_based_pos_traj_controller/

Inside your own programm you should use the action server to communicate with the ``joint_trajectory_controller``.
See [here](http://wiki.ros.org/joint_trajectory_controller?distro=kinetic) for additional information about joint trajectory controller.

### Velocity Control

To control joint velocities there is a JointTrajectory topic for sending joint speeds:

    /ur_driver/joint_speed

It takes messages of type ``trajectory_msgs/JointTrajectory`` and parses the first JointTracetoryPoint and sends the specified joint speeds and accelerations to the robot.
Remember to set values for all 6 joints. Ignores the field joint_names, so set the values in the correct order.

To switch from position controller to velocity controller you need to stop the position controller by sending a switch service command on the following topic:

    /ur_driver/controller_manager/switch_controller

The command contains:
- start_controllers: ['']
- stop_controllers:    ['vel_based_pos_traj_controller']

And vice versa to go back to position control.
See [here](http://wiki.ros.org/controller_manager?distro=kinetic) for additional information on how to switch between position/trajectory and velocity controllers.

## Control Example

Look inside the ``ur_examples`` package for a simple example on how to use position and velocity control from the inside of a node:

- ``ur_sim_mixed_publisher`` for simulation case
- ``ur_real_mixed_publisher`` for real robot case

## Launch Parameters and Flags

- ``ft_sensor`` Robotiq force torque sensor mounted between end effector and camera/gripper (default true) 
   force torque measurements are published on the topic robotic_force_torque_wrench as messages ft_sensor 
- ``camera`` Robotiq vision system mounted between end effector/ft sensor and gripper (default true)



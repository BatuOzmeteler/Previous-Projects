cmake_minimum_required(VERSION 2.8.3)
project(ur_gazebo)

set(CMAKE_BUILD_TYPE Release)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  gazebo_ros
  trajectory_msgs
  std_msgs
  control_msgs
  actionlib
  controller_manager_msgs
  gazebo_ros_control
  ur_utilities
)

add_compile_options(-std=c++11)

# Depend on system install of Gazebo
find_package(gazebo REQUIRED)

# Gazebo cxx flags should have all the required C++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

#######################################
## Declare ROS messages and services ##
#######################################

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES roboticsgroup_gazebo_mimic_joint_plugin roboticsgroup_gazebo_disable_link_plugin
  CATKIN_DEPENDS roscpp gazebo_ros trajectory_msgs std_msgs control_msgs actionlib controller_manager_msgs ros_control ur_utilities gazebo_ros_control
  DEPENDS gazebo_ros
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
  include
)

## Declare a cpp library
# add_library(ur_gazebo
#   src/${PROJECT_NAME}/ur_gazebo.cpp
# )

## Declare a cpp executable
# add_executable(ur_gazebo_node src/ur_gazebo_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(ur_gazebo_node ur_gazebo_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(ur_gazebo_node
#   ${catkin_LIBRARIES}
# )

add_executable(ur_msgs_translation
    src/ur_msgs_translation/ur_msgs_translation_main.cpp
)

target_link_libraries(ur_msgs_translation
    ${catkin_LIBRARIES}
)

add_library(roboticsgroup_gazebo_mimic_joint_plugin
    src/roboticsgroup_gazebo_plugins/mimic_joint_plugin.cpp
)
target_link_libraries(roboticsgroup_gazebo_mimic_joint_plugin
    ${catkin_LIBRARIES}
    ${GAZEBO_LIBRARIES}
)

add_library(roboticsgroup_gazebo_disable_link_plugin
    src/roboticsgroup_gazebo_plugins/disable_link_plugin.cpp
)

target_link_libraries(roboticsgroup_gazebo_disable_link_plugin
    ${catkin_LIBRARIES}
    ${GAZEBO_LIBRARIES}
)

#############
## Install ##
#############

install(TARGETS roboticsgroup_gazebo_mimic_joint_plugin roboticsgroup_gazebo_disable_link_plugin
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(DIRECTORY include/
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
)

install(TARGETS ur_msgs_translation
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

install(DIRECTORY config/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config
)

install(DIRECTORY controller/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/controller
)

install(DIRECTORY models/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/models
)

install(DIRECTORY worlds/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/worlds
)

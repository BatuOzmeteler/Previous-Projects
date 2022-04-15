cmake_minimum_required(VERSION 2.8.3)
project(ur_examples)

set(CMAKE_BUILD_TYPE Release)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  trajectory_msgs
  control_msgs
  actionlib
  controller_manager_msgs
  ur_utilities
)

# check c++11 / c++0x
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "-std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

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
  # INCLUDE_DIRS include
  # LIBRARIES
  CATKIN_DEPENDS roscpp trajectory_msgs control_msgs actionlib controller_manager_msgs ur_utilities
#  DEPENDS
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

include_directories(
  ${catkin_INCLUDE_DIRS}
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

add_executable(ur_mixed_publisher src/ur_examples/ur_mixed_publisher.cpp)
target_link_libraries(ur_mixed_publisher ${catkin_LIBRARIES})

add_executable(ur_sim_gripper_control_client src/ur_examples/ur_sim_gripper_control_client.cpp)
target_link_libraries(ur_sim_gripper_control_client ${catkin_LIBRARIES})

#############
## Install ##
#############

install(TARGETS ur_mixed_publisher ur_sim_gripper_control_client
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

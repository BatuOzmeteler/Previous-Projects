cmake_minimum_required(VERSION 2.8.3)
project(ur_utilities)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
set(CMAKE_BUILD_TYPE Release)

# Optimization, use -O2 if any problems occur
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  kdl_parser # tests only
  sensor_msgs
  visualization_msgs
  robot_utilities
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ur_kinematic ur_description ur_collision ur_manipulator ur_misc
  CATKIN_DEPENDS roscpp kdl_parser sensor_msgs visualization_msgs robot_utilities
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
add_library(ur_description STATIC  
  src/ur_description/ur3_description.cpp
  src/ur_description/ur10_description.cpp
  src/ur_description/ur_description.cpp
)

add_library(ur_kinematic STATIC
  src/ur_kinematic/ur_kinematic.cpp
  src/ur_kinematic/ur_inverse_kinematic.cpp
)

add_library(ur_collision STATIC
  src/ur_collision/ur_collision.cpp
)

add_library(ur_manipulator STATIC
  src/ur_manipulator/ur_manipulator.cpp
)

add_library(ur_misc STATIC
  src/ur_misc/ur_utility.cpp
)

#add_dependencies(ur_kinematics ${catkin_EXPORTED_TARGETS})
#set_property(TARGET ur_kinematics PROPERTY POSITION_INDEPENDENT_CODE ON)

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide

add_executable(ur_kinematic_test
  src/benchmarks/ur_kinematic_test.cpp
)

add_executable(ur_inverse_kinematic_test
  src/benchmarks/ur_inverse_kinematic_test.cpp
)

add_executable(ur_collision_test
  src/benchmarks/ur_collision_test.cpp
)

add_executable(ur_distance_publisher
  src/ur_tools/collision_marker_publisher.cpp
)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against

target_link_libraries(ur_collision
  ur_kinematic
  ${catkin_LIBRARIES}
)

target_link_libraries(ur_description
  ${catkin_LIBRARIES}
)

target_link_libraries(ur_kinematic
  ur_misc
  ur_description
  ${catkin_LIBRARIES}
)

target_link_libraries(ur_manipulator
  ${catkin_LIBRARIES}
)

target_link_libraries(ur_misc
  ${catkin_LIBRARIES}
)

target_link_libraries(ur_kinematic_test
  ur_kinematic
  ${catkin_LIBRARIES}
)

target_link_libraries(ur_inverse_kinematic_test
  ur_kinematic
  ${catkin_LIBRARIES}
)

target_link_libraries(ur_collision_test
  ur_collision
  ${catkin_LIBRARIES}
)

target_link_libraries(ur_distance_publisher
  ur_collision
  ur_misc
  ${catkin_LIBRARIES}
)

#############
## Install ##
#############

install(TARGETS ur_kinematic ur_description ur_collision ur_manipulator ur_misc
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(DIRECTORY include/
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
)

install(TARGETS ur_kinematic_test ur_inverse_kinematic_test ur_collision_test ur_distance_publisher
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

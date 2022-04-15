cmake_minimum_required(VERSION 2.8.3)
project(robotiq_action_server)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  robotiq_c_model_control
  roscpp
  control_msgs
  std_msgs
  ur_utilities
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS actionlib actionlib_msgs std_msgs robotiq_c_model_control roscpp ur_utilities
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${robotiq_action_server_INCLUDE_DIRS}
)

# The action server
add_executable(robotiq_c_model_action_server_node
  src/robotiq_c_model_action_server_node.cpp
  src/robotiq_c_model_action_server.cpp
)

add_dependencies(robotiq_c_model_action_server_node
  ${robotiq_action_server_EXPORTED_TARGETS}
  ${robotiq_c_model_control_EXPORTED_TARGETS})

target_link_libraries(robotiq_c_model_action_server_node
  ${catkin_LIBRARIES}
)

# The test server
add_executable(robotiq_action_server_client_test
  src/robotiq_action_server_client_test.cpp
)

target_link_libraries(robotiq_action_server_client_test
  ${catkin_LIBRARIES}
)


install(DIRECTORY include/
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
)

install(TARGETS robotiq_c_model_action_server_node robotiq_action_server_client_test
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

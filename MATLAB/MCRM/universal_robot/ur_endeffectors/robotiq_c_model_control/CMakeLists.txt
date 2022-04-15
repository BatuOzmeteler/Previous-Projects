# http://ros.org/doc/groovy/api/catkin/html/user_guide/supposed.html
cmake_minimum_required(VERSION 2.8.3)
project(robotiq_c_model_control)

find_package(catkin REQUIRED COMPONENTS roscpp rospy message_generation)

###################################
## catkin message generation     ##
###################################
add_message_files(
  FILES
  CModel_robot_input.msg
  CModel_robot_output.msg
)

catkin_python_setup()

generate_messages()

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
    CATKIN_DEPENDS rospy message_runtime roscpp
    #INCLUDE_DIRS
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${robotiq_ethercat_INCLUDE_DIRS}
)

catkin_install_python(PROGRAMS nodes/CModelRtuNode.py nodes/CModelSimpleController.py nodes/CModelStatusListener.py nodes/CModelTcpNode.py
    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

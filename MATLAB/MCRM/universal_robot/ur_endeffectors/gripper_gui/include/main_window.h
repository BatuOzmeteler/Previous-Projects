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
 * Authors: Rodrigo Velasco
 *********************************************************************/

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <control_msgs/FollowJointTrajectoryActionGoal.h>
#include <ros/package.h>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <ur_utilities/ur_misc/ur_utility.h>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QToolButton>

namespace gripper_gui {

const QString OrganizationName = "rst";
const QString ApplicationName  = "Gripper-GUI";

using URUtility = robot_utilities::robot_misc::URUtility;

/**
 * @brief
 *
 * MainWindow is meant to create a Graphical User Interface for
 * control of a real robotiq gripper.
 *
 * @author Rodrigo Velasco
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

 public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

 public slots:
    void enableUI(bool enable = true);

 private slots:
    void showInfoScreen();
    void positionEdit(double value);
    void positionSlide(int value);
    void speedEdit(double value);
    void speedSlide(int value);
    void effortEdit(double value);
    void effortSlide(int value);

 private:
    ros::NodeHandle _nh;

    ros::Publisher _target_pub;
    ros::Subscriber _state_sub;

    // menu
    QMenu* _menu_file;
    QMenu* _menu_about;

    // menu actions
    QAction* _info_action;
    QAction* _exit_action;

    // widgets
    QWidget* _window;

    // layouts
    QGridLayout* _top_level_layout;
    QHBoxLayout *_switch_layout, *_position_slider_layout, *_speed_slider_layout, *_effort_slider_layout;
    QVBoxLayout* _slider_layout;
    QDoubleSpinBox *_position_edit, *_speed_edit, *_effort_edit;

    // control elements
    QToolButton* _control_switch;
    QSlider *_position_slider, *_speed_slider, *_effort_slider;

    // variables
    double _target_position, _target_speed, _target_effort;

    void setLayout();
    void createMenus();
    void connectSignals();
    void sendGoal(double position, double speed, double effort);
    void gripperStateCallback(const sensor_msgs::JointStateConstPtr& msg);
};
}  // namespace gripper_gui

#endif  // MAIN_WINDOW_H

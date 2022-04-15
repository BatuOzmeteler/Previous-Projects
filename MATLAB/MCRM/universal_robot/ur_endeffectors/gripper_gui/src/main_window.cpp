#include <main_window.h>
#include <QDebug>
#include <QMenuBar>

namespace gripper_gui {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), _nh()
{
    setWindowTitle(tr("Gripper control GUI"));

    resize(400, 270);
    setLayout();
    createMenus();
    connectSignals();
    enableUI(false);

    // topics will be remaped in launch file
    std::string target_topic = "/gripper_topic";
    std::string state_topic  = "/gripper_state";

    _target_pub = _nh.advertise<control_msgs::FollowJointTrajectoryActionGoal>(target_topic, 1);
    _state_sub  = _nh.subscribe(state_topic, 1, &MainWindow::gripperStateCallback, this);
}

MainWindow::~MainWindow() {}

void MainWindow::setLayout()
{
    _window = new QWidget();
    setCentralWidget(_window);

    _top_level_layout = new QGridLayout();
    _top_level_layout->setAlignment(Qt::AlignTop);

    _window->setLayout(_top_level_layout);

    // Control switch
    _control_switch = new QToolButton;
    _control_switch->setCheckable(true);

    QIcon qIcon;
    qIcon.addFile(QString::fromUtf8((ros::package::getPath(std::string("gripper_gui")) + std::string("/icons/on_button.png")).c_str()), QSize(50, 50),
                  QIcon::Normal, QIcon::On);
    qIcon.addFile(QString::fromUtf8((ros::package::getPath(std::string("gripper_gui")) + std::string("/icons/off_button.png")).c_str()),
                  QSize(50, 50), QIcon::Normal, QIcon::Off);

    _control_switch->setIcon(qIcon);
    _control_switch->setFixedSize(50, 50);
    _control_switch->setIconSize(QSize(50, 50));

    _switch_layout = new QHBoxLayout();
    _switch_layout->addWidget(_control_switch);

    // Sliders
    QLabel* pos_label    = new QLabel("Position [%]");
    QLabel* vel_label    = new QLabel("Speed [%]");
    QLabel* effort_label = new QLabel("Effort [%]");

    _position_slider_layout = new QHBoxLayout();

    _position_slider = new QSlider(Qt::Horizontal);
    _position_slider->setMinimum(0);
    _position_slider->setMaximum(100);
    _position_slider->setTickInterval(50);
    _position_slider->setTickPosition(QSlider::TicksBelow);

    _position_edit = new QDoubleSpinBox();
    _position_edit->setMinimum(0);
    _position_edit->setMaximum(100);
    _position_edit->setDecimals(0);
    _position_edit->setSingleStep(1);

    _position_slider_layout->addWidget(_position_slider);
    _position_slider_layout->addWidget(_position_edit);

    _speed_slider_layout = new QHBoxLayout();

    _speed_slider = new QSlider(Qt::Horizontal);
    _speed_slider->setMinimum(0);
    _speed_slider->setMaximum(100);
    _speed_slider->setTickInterval(50);
    _speed_slider->setTickPosition(QSlider::TicksBelow);

    _speed_edit = new QDoubleSpinBox();
    _speed_edit->setMinimum(0);
    _speed_edit->setMaximum(100);
    _speed_edit->setDecimals(0);
    _speed_edit->setSingleStep(1);

    _speed_slider_layout->addWidget(_speed_slider);
    _speed_slider_layout->addWidget(_speed_edit);

    _effort_slider_layout = new QHBoxLayout();

    _effort_slider = new QSlider(Qt::Horizontal);
    _effort_slider->setMinimum(0);
    _effort_slider->setMaximum(100);
    _effort_slider->setTickInterval(50);
    _effort_slider->setTickPosition(QSlider::TicksBelow);

    _effort_edit = new QDoubleSpinBox();
    _effort_edit->setMinimum(0);
    _effort_edit->setMaximum(100);
    _effort_edit->setDecimals(0);
    _effort_edit->setSingleStep(1);

    _effort_slider_layout->addWidget(_effort_slider);
    _effort_slider_layout->addWidget(_effort_edit);

    _slider_layout = new QVBoxLayout();
    _slider_layout->addWidget(pos_label);
    _slider_layout->addLayout(_position_slider_layout);
    _slider_layout->addWidget(vel_label);
    _slider_layout->addLayout(_speed_slider_layout);
    _slider_layout->addWidget(effort_label);
    _slider_layout->addLayout(_effort_slider_layout);

    _top_level_layout->addLayout(_switch_layout, 0, 0, 1, 1);
    _top_level_layout->addLayout(_slider_layout, 1, 0);
}

void MainWindow::connectSignals()
{
    // menu actions
    connect(_exit_action, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(_info_action, SIGNAL(triggered(bool)), this, SLOT(showInfoScreen()));

    connect(_control_switch, SIGNAL(clicked(bool)), this, SLOT(enableUI(bool)));

    connect(_position_slider, SIGNAL(sliderMoved(int)), this, SLOT(positionSlide(int)));
    connect(_speed_slider, SIGNAL(sliderMoved(int)), this, SLOT(speedSlide(int)));
    connect(_effort_slider, SIGNAL(sliderMoved(int)), this, SLOT(effortSlide(int)));

    connect(_position_edit, SIGNAL(valueChanged(double)), this, SLOT(positionEdit(double)));
    connect(_speed_edit, SIGNAL(valueChanged(double)), this, SLOT(speedEdit(double)));
    connect(_effort_edit, SIGNAL(valueChanged(double)), this, SLOT(effortEdit(double)));
}

void MainWindow::showInfoScreen() { qDebug() << "[showInfoScreen] Not yet implemented"; }

void MainWindow::enableUI(bool enable)
{
    _position_slider->setEnabled(enable);
    _position_edit->setEnabled(enable);
    _speed_slider->setEnabled(enable);
    _speed_edit->setEnabled(enable);
    _effort_slider->setEnabled(enable);
    _effort_edit->setEnabled(enable);
}

void MainWindow::createMenus()
{
    _menu_file  = menuBar()->addMenu(tr("File"));
    _menu_about = menuBar()->addMenu(tr("About"));

    _exit_action = new QAction(tr("Exit"), this);
    _info_action = new QAction(tr("Info"), this);

    _menu_file->addAction(_exit_action);
    _menu_about->addAction(_info_action);

    _exit_action->setStatusTip("Exit Gripper Control GUI");
    _info_action->setStatusTip("Info about Gripper Control GUI");

    menuBar()->setNativeMenuBar(false);
}

void MainWindow::positionEdit(double value)
{
    _target_position = value;
    _position_slider->setValue((int)value);

    if (_control_switch->isChecked()) sendGoal(_target_position, _target_speed, _target_effort);
}

void MainWindow::positionSlide(int value)
{
    _target_position = (double)value;
    _position_edit->setValue(value);  // This triggers positionEdit
}

void MainWindow::speedEdit(double value)
{
    _target_speed = value;
    _speed_slider->setValue((int)value);

    if (_control_switch->isChecked()) sendGoal(_target_position, _target_speed, _target_effort);
}

void MainWindow::speedSlide(int value)
{
    _target_speed = (double)value;
    _speed_edit->setValue(value);  // This triggers speedEdit
}

void MainWindow::effortEdit(double value)
{
    _target_effort = value;
    _effort_slider->setValue((int)value);

    if (_control_switch->isChecked()) sendGoal(_target_position, _target_speed, _target_effort);
}

void MainWindow::effortSlide(int value)
{
    _target_effort = (double)value;
    _effort_edit->setValue(value);  // This triggers effortEdit
}

void MainWindow::sendGoal(double position, double speed, double effort)
{
    control_msgs::FollowJointTrajectoryActionGoal goal_msg;

    control_msgs::FollowJointTrajectoryGoal goal = URUtility::getGripperCommandGoal(position, speed, effort, 1);

    goal_msg.goal         = goal;
    goal_msg.header.stamp = ros::Time::now();

    _target_pub.publish(goal_msg);
}

void MainWindow::gripperStateCallback(const sensor_msgs::JointStateConstPtr& msg)
{
    if (!_control_switch->isChecked())
    {
        int percentage = (int)std::round((msg->position[0] - 0.402) * 100 / (-0.804));
        _position_edit->setValue(percentage);
    }
}

}  // namespace gripper_gui

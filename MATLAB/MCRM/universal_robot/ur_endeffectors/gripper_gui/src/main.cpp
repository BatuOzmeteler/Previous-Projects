#include <main_window.h>
#include <ros/ros.h>
#include <QApplication>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "gripper_gui");
    ros::NodeHandle n("~");

    // Async spinner for callbacks
    ros::AsyncSpinner spinner(1);
    spinner.start();

    QApplication app(argc, argv);
    app.setOrganizationName(gripper_gui::OrganizationName);
    app.setApplicationName(gripper_gui::ApplicationName);

    gripper_gui::MainWindow main_window;

    main_window.show();
    int retval = app.exec();

    spinner.stop();

    return retval;
}

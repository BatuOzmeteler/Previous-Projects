#include <ur_utilities/ur_description/ur_description.h>
#include <iostream>

namespace robot_utilities {
namespace robot_description {

URDescription::URDescription() {}

URDescription::URDescription(const std::string& urdf_param, const std::string& first_frame, const std::string& last_frame)
    : RobotDescription(urdf_param, first_frame, last_frame)
{
}

bool URDescription::checkParameters(const std::string& urdf_param)
{
    if (_urdf_model.getName() != "ur10" && _urdf_model.getName() != "ur3")
    {
        ROS_ERROR("URDescription: Not an UR urdf.");
        return false;
    }

    if (_segment_structs.size() < 9)
    {
        ROS_ERROR("URDescription: Incorrect UR Kinematic structure.");
        return false;
    }

    if (_segment_structs.size() < _link_names.size())
    {
        ROS_ERROR("URDescription: Not as many links as expected link names.");
        return false;
    }

    int n_dynamic_joints =
        std::count_if(_segment_structs.begin(), _segment_structs.end(), [](const robot_misc::Segment& s) { return !s.joint.fixed; });

    for (int i = 0; i < _link_names.size(); ++i)
    {
        if (_link_names[i].compare(_segment_structs[i].name) != 0)
        {
            ROS_ERROR("URDescription: Expected link %s at %u position", _link_names[i].c_str(), i);
            return false;
        }
    }

    if (n_dynamic_joints != 6)
    {
        ROS_ERROR("URDescription: Incorrect UR joint configuration");
        return false;
    }

    if (fabs(_segment_structs[0].joint.Rx) > 1e-5)
    {
        ROS_ERROR("URDescription: Expected no rotation around x axis on %s", _segment_structs[0].name.c_str());
        return false;
    }

    if (fabs(_segment_structs[0].joint.Ry) > 1e-5)
    {
        ROS_ERROR("URDescription: Expected no rotation around y axis on %s", _segment_structs[0].name.c_str());
        return false;
    }

    return true;
}

}  // namespace robot_description
}  // namespace robot_utilities

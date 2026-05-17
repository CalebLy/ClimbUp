#include "external/doctest.h"

#include "ClimbMath.h"

#include <cmath>

TEST_CASE("getDistanceBetweenJoints returns correct 3D distance")
{
    Pose pose;

    pose.joints.resize(static_cast<std::size_t>(JointType::JointCount));

    pose.joints[static_cast<std::size_t>(JointType::LeftShoulder)].position =
        {0.0, 0.0, 0.0};

    pose.joints[static_cast<std::size_t>(JointType::LeftElbow)].position =
        {3.0, 4.0, 0.0};

    double distance = getDistanceBetweenJoints(
        pose,
        JointType::LeftShoulder,
        JointType::LeftElbow
    );

    CHECK(distance == doctest::Approx(5.0));
}
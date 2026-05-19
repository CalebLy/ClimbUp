#include "external/doctest.h"
#include "Balance.h"
#include "PoseFactory.h"
#include <EnumUtils.h>

TEST_CASE("calculateCenterOfGravity returns expected center for symmetric pose")
{
    UserProportions user{.weight = 80.0};
    Pose pose = createDefaultPose(user);

    // Create a perfectly symmetric upright pose around x = 0.
    pose.joints[toIndex(JointType::Pelvis)].position = {0.0, 0.0, 0.0};
    pose.joints[toIndex(JointType::Spine)].position = {0.0, 1.0, 0.0};
    pose.joints[toIndex(JointType::Chest)].position = {0.0, 2.0, 0.0};
    pose.joints[toIndex(JointType::Neck)].position = {0.0, 3.0, 0.0};
    pose.joints[toIndex(JointType::Head)].position = {0.0, 4.0, 0.0};

    pose.joints[toIndex(JointType::LeftShoulder)].position = {-1.0, 2.0, 0.0};
    pose.joints[toIndex(JointType::LeftElbow)].position = {-1.0, 1.5, 0.0};
    pose.joints[toIndex(JointType::LeftWrist)].position = {-1.0, 1.0, 0.0};
    pose.joints[toIndex(JointType::LeftHand)].position = {-1.0, 0.5, 0.0};

    pose.joints[toIndex(JointType::RightShoulder)].position = {1.0, 2.0, 0.0};
    pose.joints[toIndex(JointType::RightElbow)].position = {1.0, 1.5, 0.0};
    pose.joints[toIndex(JointType::RightWrist)].position = {1.0, 1.0, 0.0};
    pose.joints[toIndex(JointType::RightHand)].position = {1.0, 0.5, 0.0};

    pose.joints[toIndex(JointType::LeftHip)].position = {-0.5, -1.0, 0.0};
    pose.joints[toIndex(JointType::LeftKnee)].position = {-0.5, -2.0, 0.0};
    pose.joints[toIndex(JointType::LeftAnkle)].position = {-0.5, -3.0, 0.0};
    pose.joints[toIndex(JointType::LeftFoot)].position = {-0.5, -3.5, 0.0};

    pose.joints[toIndex(JointType::RightHip)].position = {0.5, -1.0, 0.0};
    pose.joints[toIndex(JointType::RightKnee)].position = {0.5, -2.0, 0.0};
    pose.joints[toIndex(JointType::RightAnkle)].position = {0.5, -3.0, 0.0};
    pose.joints[toIndex(JointType::RightFoot)].position = {0.5, -3.5, 0.0};

    Vec3 centerOfGravity = calculateCenterOfGravity(pose);

    CHECK(centerOfGravity.x == doctest::Approx(0.0));
    CHECK(centerOfGravity.z == doctest::Approx(0.0));

    // Should be somewhere near the torso/pelvis region vertically.
    CHECK(centerOfGravity.y > -0.5);
    CHECK(centerOfGravity.y < 1.5);
}
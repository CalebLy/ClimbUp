#include "external/doctest.h"

#include "PoseMovement.h"
#include "Pose.h"
#include "ClimbMath.h"
#include "EnumUtils.h"

static Pose createInitializedPose()
{
    Pose pose;
    pose.joints.resize(toIndex(JointType::JointCount));

    return pose;
}

TEST_CASE("translatePose moves every joint by translation amount")
{
    Pose pose = createInitializedPose();

    pose.joints[toIndex(JointType::Head)].position = {1.0, 2.0, 3.0};
    pose.joints[toIndex(JointType::LeftHand)].position = {4.0, 5.0, 6.0};

    Vec3 translation = {10.0, -2.0, 1.0};

    translatePose(pose, translation);

    Vec3 head = pose.joints[toIndex(JointType::Head)].position;
    Vec3 hand = pose.joints[toIndex(JointType::LeftHand)].position;

    CHECK(head.x == doctest::Approx(11.0));
    CHECK(head.y == doctest::Approx(0.0));
    CHECK(head.z == doctest::Approx(4.0));

    CHECK(hand.x == doctest::Approx(14.0));
    CHECK(hand.y == doctest::Approx(3.0));
    CHECK(hand.z == doctest::Approx(7.0));
}

TEST_CASE("translatePose throws if pose is not initialized")
{
    Pose pose;

    Vec3 translation = {1.0, 1.0, 1.0};

    CHECK_THROWS_AS(translatePose(pose, translation), std::invalid_argument);
}

TEST_CASE("moveJointTo directly moves non IK joints")
{
    Pose pose = createInitializedPose();

    WallPoint target = {7.0, 8.0};

    moveJointTo(pose, JointType::Head, target);

    Vec3 head = pose.joints[toIndex(JointType::Head)].position;

    CHECK(head.x == doctest::Approx(7.0));
    CHECK(head.y == doctest::Approx(8.0));
    CHECK(head.z == doctest::Approx(0.0));
}

TEST_CASE("moveJointTo uses IK for left hand")
{
    Pose pose = createInitializedPose();

    pose.joints[toIndex(JointType::LeftShoulder)].position = {0.0, 0.0, 0.0};
    pose.joints[toIndex(JointType::LeftElbow)].position = {5.0, 0.0, 0.0};
    pose.joints[toIndex(JointType::LeftHand)].position = {10.0, 0.0, 0.0};

    WallPoint target = {6.0, 6.0};

    moveJointTo(pose, JointType::LeftHand, target);

    Vec3 shoulder = pose.joints[toIndex(JointType::LeftShoulder)].position;
    Vec3 elbow = pose.joints[toIndex(JointType::LeftElbow)].position;
    Vec3 hand = pose.joints[toIndex(JointType::LeftHand)].position;

    CHECK(hand.x == doctest::Approx(6.0).epsilon(0.001));
    CHECK(hand.y == doctest::Approx(6.0).epsilon(0.001));

    CHECK(distance(shoulder, elbow) == doctest::Approx(5.0).epsilon(0.001));
    CHECK(distance(elbow, hand) == doctest::Approx(5.0).epsilon(0.001));
}

TEST_CASE("moveJointTo clamps unreachable IK targets")
{
    Pose pose = createInitializedPose();

    pose.joints[toIndex(JointType::LeftShoulder)].position = {0.0, 0.0, 0.0};
    pose.joints[toIndex(JointType::LeftElbow)].position = {5.0, 0.0, 0.0};
    pose.joints[toIndex(JointType::LeftHand)].position = {10.0, 0.0, 0.0};

    WallPoint target = {20.0, 0.0};

    moveJointTo(pose, JointType::LeftHand, target);

    Vec3 shoulder = pose.joints[toIndex(JointType::LeftShoulder)].position;
    Vec3 hand = pose.joints[toIndex(JointType::LeftHand)].position;

    CHECK(distance(shoulder, hand) == doctest::Approx(10.0).epsilon(0.001));

    CHECK(hand.x == doctest::Approx(10.0).epsilon(0.001));
    CHECK(hand.y == doctest::Approx(0.0).epsilon(0.001));
}
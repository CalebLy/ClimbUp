#include "external/doctest.h"

#include "Pose.h"
#include "ClimbMath.h"
#include "IKSolver.h"
#include "EnumUtils.h"

static Pose createSimpleLeftArmPose()
{
    Pose pose;
    pose.joints.resize(toIndex(JointType::JointCount));

    pose.joints[toIndex(JointType::LeftShoulder)].position = {0.0, 0.0, 0.0};
    pose.joints[toIndex(JointType::LeftElbow)].position = {5.0, 0.0, 0.0};
    pose.joints[toIndex(JointType::LeftHand)].position = {10.0, 0.0, 0.0};

    return pose;
}

TEST_CASE("solveTwoBoneIK reaches a reachable target")
{
    Pose pose = createSimpleLeftArmPose();

    Vec3 target = {6.0, 6.0, 0.0};

    bool success = solveTwoBoneIK(
        pose,
        JointType::LeftShoulder,
        JointType::LeftElbow,
        JointType::LeftHand,
        target);

    CHECK(success);

    Vec3 hand = pose.joints[toIndex(JointType::LeftHand)].position;

    CHECK(hand.x == doctest::Approx(6.0).epsilon(0.001));
    CHECK(hand.y == doctest::Approx(6.0).epsilon(0.001));
    CHECK(hand.z == doctest::Approx(0.0).epsilon(0.001));
}

TEST_CASE("solveTwoBoneIK preserves limb lengths")
{
    Pose pose = createSimpleLeftArmPose();

    Vec3 target = {6.0, 6.0, 0.0};

    solveTwoBoneIK(
        pose,
        JointType::LeftShoulder,
        JointType::LeftElbow,
        JointType::LeftHand,
        target);

    Vec3 shoulder = pose.joints[toIndex(JointType::LeftShoulder)].position;
    Vec3 elbow = pose.joints[toIndex(JointType::LeftElbow)].position;
    Vec3 hand = pose.joints[toIndex(JointType::LeftHand)].position;

    CHECK(distance(shoulder, elbow) == doctest::Approx(5.0).epsilon(0.001));
    CHECK(distance(elbow, hand) == doctest::Approx(5.0).epsilon(0.001));
}

TEST_CASE("solveTwoBoneIK clamps unreachable target")
{
    Pose pose = createSimpleLeftArmPose();

    Vec3 target = {20.0, 0.0, 0.0};

    bool success = solveTwoBoneIK(
        pose,
        JointType::LeftShoulder,
        JointType::LeftElbow,
        JointType::LeftHand,
        target);

    CHECK(success);

    Vec3 shoulder = pose.joints[toIndex(JointType::LeftShoulder)].position;
    Vec3 hand = pose.joints[toIndex(JointType::LeftHand)].position;

    CHECK(distance(shoulder, hand) == doctest::Approx(10.0).epsilon(0.001));
    CHECK(hand.x == doctest::Approx(10.0).epsilon(0.001));
    CHECK(hand.y == doctest::Approx(0.0).epsilon(0.001));
}

TEST_CASE("solveTwoBoneIK returns false for zero length first limb")
{
    Pose pose;
    pose.joints.resize(toIndex(JointType::JointCount));

    pose.joints[toIndex(JointType::LeftShoulder)].position = {0.0, 0.0, 0.0};
    pose.joints[toIndex(JointType::LeftElbow)].position = {0.0, 0.0, 0.0};
    pose.joints[toIndex(JointType::LeftHand)].position = {5.0, 0.0, 0.0};

    Vec3 target = {3.0, 3.0, 0.0};

    CHECK_FALSE(solveTwoBoneIK(
        pose,
        JointType::LeftShoulder,
        JointType::LeftElbow,
        JointType::LeftHand,
        target));
}

TEST_CASE("solveTwoBoneIK returns false when target is exactly at root")
{
    Pose pose = createSimpleLeftArmPose();

    Vec3 target = {0.0, 0.0, 0.0};

    CHECK_FALSE(solveTwoBoneIK(
        pose,
        JointType::LeftShoulder,
        JointType::LeftElbow,
        JointType::LeftHand,
        target));
}
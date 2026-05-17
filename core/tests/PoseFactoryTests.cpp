#include "external/doctest.h"

#include "PoseFactory.h"
#include "EnumUtils.h"

static BodyProportions makeTestBody()
{
    BodyProportions body;

    body.shoulderWidth = 40.0;
    body.torsoLength = 60.0;
    body.neckLength = 10.0;
    body.headHeight = 20.0;

    body.upperArmLength = 30.0;
    body.forearmLength = 25.0;
    body.handLength = 10.0;

    body.upperLegLength = 45.0;
    body.lowerLegLength = 40.0;
    body.footLength = 15.0;

    return body;
}

TEST_CASE("createPoseFromBodyProportions creates the correct number of joints and limbs")
{
    Pose pose = createPoseFromBodyProportions(makeTestBody());

    CHECK(pose.joints.size() == toIndex(JointType::JointCount));
    CHECK(pose.limbs.size() == toIndex(LimbType::LimbCount));
}

TEST_CASE("createPoseFromBodyProportions stores the provided body proportions")
{
    BodyProportions body = makeTestBody();

    Pose pose = createPoseFromBodyProportions(body);

    CHECK(pose.body.shoulderWidth == doctest::Approx(40.0));
    CHECK(pose.body.torsoLength == doctest::Approx(60.0));
    CHECK(pose.body.upperArmLength == doctest::Approx(30.0));
}

TEST_CASE("pose places pelvis at origin")
{
    Pose pose = createPoseFromBodyProportions(makeTestBody());

    Vec3 pelvis = pose.joints[toIndex(JointType::Pelvis)].position;

    CHECK(pelvis.x == doctest::Approx(0.0));
    CHECK(pelvis.y == doctest::Approx(0.0));
    CHECK(pelvis.z == doctest::Approx(0.0));
}

TEST_CASE("pose places torso joints vertically")
{
    Pose pose = createPoseFromBodyProportions(makeTestBody());

    CHECK(pose.joints[toIndex(JointType::Spine)].position.y == doctest::Approx(30.0));
    CHECK(pose.joints[toIndex(JointType::Chest)].position.y == doctest::Approx(60.0));
    CHECK(pose.joints[toIndex(JointType::Neck)].position.y == doctest::Approx(70.0));
    CHECK(pose.joints[toIndex(JointType::Head)].position.y == doctest::Approx(90.0));
}

TEST_CASE("pose places shoulders symmetrically")
{
    Pose pose = createPoseFromBodyProportions(makeTestBody());

    Vec3 leftShoulder = pose.joints[toIndex(JointType::LeftShoulder)].position;
    Vec3 rightShoulder = pose.joints[toIndex(JointType::RightShoulder)].position;

    CHECK(leftShoulder.x == doctest::Approx(-20.0));
    CHECK(rightShoulder.x == doctest::Approx(20.0));
    CHECK(leftShoulder.y == doctest::Approx(rightShoulder.y));
    CHECK(leftShoulder.z == doctest::Approx(rightShoulder.z));
}

TEST_CASE("pose places arms hanging downward")
{
    Pose pose = createPoseFromBodyProportions(makeTestBody());

    CHECK(pose.joints[toIndex(JointType::LeftElbow)].position.y == doctest::Approx(30.0));
    CHECK(pose.joints[toIndex(JointType::LeftWrist)].position.y == doctest::Approx(5.0));
    CHECK(pose.joints[toIndex(JointType::LeftHand)].position.y == doctest::Approx(-5.0));

    CHECK(pose.joints[toIndex(JointType::RightElbow)].position.y == doctest::Approx(30.0));
    CHECK(pose.joints[toIndex(JointType::RightWrist)].position.y == doctest::Approx(5.0));
    CHECK(pose.joints[toIndex(JointType::RightHand)].position.y == doctest::Approx(-5.0));
}

TEST_CASE("pose places hips symmetrically")
{
    Pose pose = createPoseFromBodyProportions(makeTestBody());

    Vec3 leftHip = pose.joints[toIndex(JointType::LeftHip)].position;
    Vec3 rightHip = pose.joints[toIndex(JointType::RightHip)].position;

    CHECK(leftHip.x == doctest::Approx(-14.0));
    CHECK(rightHip.x == doctest::Approx(14.0));
    CHECK(leftHip.y == doctest::Approx(0.0));
    CHECK(rightHip.y == doctest::Approx(0.0));
}

TEST_CASE("pose places legs downward")
{
    Pose pose = createPoseFromBodyProportions(makeTestBody());

    CHECK(pose.joints[toIndex(JointType::LeftKnee)].position.y == doctest::Approx(-45.0));
    CHECK(pose.joints[toIndex(JointType::LeftAnkle)].position.y == doctest::Approx(-85.0));
    CHECK(pose.joints[toIndex(JointType::LeftFoot)].position.z == doctest::Approx(15.0));

    CHECK(pose.joints[toIndex(JointType::RightKnee)].position.y == doctest::Approx(-45.0));
    CHECK(pose.joints[toIndex(JointType::RightAnkle)].position.y == doctest::Approx(-85.0));
    CHECK(pose.joints[toIndex(JointType::RightFoot)].position.z == doctest::Approx(15.0));
}

TEST_CASE("pose connects important limb pairs")
{
    Pose pose = createPoseFromBodyProportions(makeTestBody());

    CHECK(pose.limbs[toIndex(LimbType::PelvisToSpine)].startJointIndex == toIndex(JointType::Pelvis));
    CHECK(pose.limbs[toIndex(LimbType::PelvisToSpine)].endJointIndex == toIndex(JointType::Spine));

    CHECK(pose.limbs[toIndex(LimbType::LeftUpperArm)].startJointIndex == toIndex(JointType::LeftShoulder));
    CHECK(pose.limbs[toIndex(LimbType::LeftUpperArm)].endJointIndex == toIndex(JointType::LeftElbow));

    CHECK(pose.limbs[toIndex(LimbType::RightUpperArm)].startJointIndex == toIndex(JointType::RightShoulder));
    CHECK(pose.limbs[toIndex(LimbType::RightUpperArm)].endJointIndex == toIndex(JointType::RightElbow));

    CHECK(pose.limbs[toIndex(LimbType::LeftThigh)].startJointIndex == toIndex(JointType::LeftHip));
    CHECK(pose.limbs[toIndex(LimbType::LeftThigh)].endJointIndex == toIndex(JointType::LeftKnee));

    CHECK(pose.limbs[toIndex(LimbType::RightThigh)].startJointIndex == toIndex(JointType::RightHip));
    CHECK(pose.limbs[toIndex(LimbType::RightThigh)].endJointIndex == toIndex(JointType::RightKnee));
}

TEST_CASE("pose limbs only use valid joint indices")
{
    Pose pose = createPoseFromBodyProportions(makeTestBody());

    for (const Limb& limb : pose.limbs)
    {
        CHECK(limb.startJointIndex >= 0);
        CHECK(limb.startJointIndex < toIndex(JointType::JointCount));

        CHECK(limb.endJointIndex >= 0);
        CHECK(limb.endJointIndex < toIndex(JointType::JointCount));
    }
}
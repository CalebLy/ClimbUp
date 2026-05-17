#include "PoseFactory.h"
#include "EnumUtils.h"



static std::vector<Joint> initializeJoints(const BodyProportions& body);
static std::vector<Limb> initializeLimbs();

// Initializes a pose using default body proportions, and optional user provided
// proportions. The default pose is a standing pose with the arms by the side,
// legs straight, and the head facing forward.
Pose createDefaultPose(const UserProportions& user)
{
    BodyProportions body = estimateBodyProportions(user);
    return createPoseFromBodyProportions(body);
}

// Initializes a pose using the provided body proportions...
// Later on, can reimplement this function to allow user to
// specify different body proportions... ex: long torso... 
Pose createPoseFromBodyProportions(const BodyProportions& body)
{
    Pose pose;
    pose.body = body;
    pose.joints = initializeJoints(body);
    pose.limbs = initializeLimbs();
    return pose;
}

static std::vector<Joint> initializeJoints(const BodyProportions &body)
{

    std::vector<Joint> joints(toIndex(JointType::JointCount));
    const double halfShoulderWidth = body.shoulderWidth / 2.0;
    const double hipWidth = body.shoulderWidth * 0.7;
    const double halfHipWidth = hipWidth / 2.0;

    // =========================
    // Pelvis is the root.
    joints[toIndex(JointType::Pelvis)].position = {0.0, 0.0, 0.0};

    // =========================
    // Torso
    joints[toIndex(JointType::Spine)].position =
        {0.0, body.torsoLength * 0.5, 0.0};

    joints[toIndex(JointType::Chest)].position =
        {0.0, body.torsoLength, 0.0};

    joints[toIndex(JointType::Neck)].position =
        {0.0, body.torsoLength + body.neckLength, 0.0};

    joints[toIndex(JointType::Head)].position =
        {0.0, body.torsoLength + body.neckLength + body.headHeight, 0.0};

    // =========================
    // Shoulders
    joints[toIndex(JointType::LeftShoulder)].position =
        {-halfShoulderWidth, body.torsoLength, 0.0};

    joints[toIndex(JointType::RightShoulder)].position =
        {halfShoulderWidth, body.torsoLength, 0.0};

    // =========================
    // Left arm, hanging straight down
    joints[toIndex(JointType::LeftElbow)].position =
        {-halfShoulderWidth, body.torsoLength - body.upperArmLength, 0.0};

    joints[toIndex(JointType::LeftWrist)].position =
        {-halfShoulderWidth, body.torsoLength - body.upperArmLength - body.forearmLength, 0.0};

    joints[toIndex(JointType::LeftHand)].position =
        {-halfShoulderWidth, body.torsoLength - body.upperArmLength - body.forearmLength - body.handLength, 0.0};

    // =========================
    // Right arm, hanging straight down
    joints[toIndex(JointType::RightElbow)].position =
        {halfShoulderWidth, body.torsoLength - body.upperArmLength, 0.0};

    joints[toIndex(JointType::RightWrist)].position =
        {halfShoulderWidth, body.torsoLength - body.upperArmLength - body.forearmLength, 0.0};

    joints[toIndex(JointType::RightHand)].position =
        {halfShoulderWidth, body.torsoLength - body.upperArmLength - body.forearmLength - body.handLength, 0.0};

    // =========================
    // Hips
    joints[toIndex(JointType::LeftHip)].position =
        {-halfHipWidth, 0.0, 0.0};

    joints[toIndex(JointType::RightHip)].position =
        {halfHipWidth, 0.0, 0.0};

    // =========================
    // Left leg
    joints[toIndex(JointType::LeftKnee)].position =
        {-halfHipWidth, -body.upperLegLength, 0.0};

    joints[toIndex(JointType::LeftAnkle)].position =
        {-halfHipWidth, -body.upperLegLength - body.lowerLegLength, 0.0};

    joints[toIndex(JointType::LeftFoot)].position =
        {-halfHipWidth, -body.upperLegLength - body.lowerLegLength, body.footLength};

    // =========================
    // Right leg
    joints[toIndex(JointType::RightKnee)].position =
        {halfHipWidth, -body.upperLegLength, 0.0};

    joints[toIndex(JointType::RightAnkle)].position =
        {halfHipWidth, -body.upperLegLength - body.lowerLegLength, 0.0};

    joints[toIndex(JointType::RightFoot)].position =
        {halfHipWidth, -body.upperLegLength - body.lowerLegLength, body.footLength};

    return joints;
}

static std::vector<Limb> initializeLimbs()
{
    std::vector<Limb> limbs(toIndex(LimbType::LimbCount));

    // =========================
    // Torso
    limbs[toIndex(LimbType::PelvisToSpine)] =
    {
        toIndex(JointType::Pelvis),
        toIndex(JointType::Spine)
    };

    limbs[toIndex(LimbType::SpineToChest)] =
    {
        toIndex(JointType::Spine),
        toIndex(JointType::Chest)
    };

    limbs[toIndex(LimbType::ChestToNeck)] =
    {
        toIndex(JointType::Chest),
        toIndex(JointType::Neck)
    };

    limbs[toIndex(LimbType::NeckToHead)] =
    {
        toIndex(JointType::Neck),
        toIndex(JointType::Head)
    };

    // =========================
    // Left arm
    limbs[toIndex(LimbType::ChestToLeftShoulder)] =
    {
        toIndex(JointType::Chest),
        toIndex(JointType::LeftShoulder)
    };

    limbs[toIndex(LimbType::LeftUpperArm)] =
    {
        toIndex(JointType::LeftShoulder),
        toIndex(JointType::LeftElbow)
    };

    limbs[toIndex(LimbType::LeftForearm)] =
    {
        toIndex(JointType::LeftElbow),
        toIndex(JointType::LeftWrist)
    };

    limbs[toIndex(LimbType::LeftHand)] =
    {
        toIndex(JointType::LeftWrist),
        toIndex(JointType::LeftHand)
    };

    // =========================
    // Right arm
    limbs[toIndex(LimbType::ChestToRightShoulder)] =
    {
        toIndex(JointType::Chest),
        toIndex(JointType::RightShoulder)
    };

    limbs[toIndex(LimbType::RightUpperArm)] =
    {
        toIndex(JointType::RightShoulder),
        toIndex(JointType::RightElbow)
    };

    limbs[toIndex(LimbType::RightForearm)] =
    {
        toIndex(JointType::RightElbow),
        toIndex(JointType::RightWrist)
    };

    limbs[toIndex(LimbType::RightHand)] =
    {
        toIndex(JointType::RightWrist),
        toIndex(JointType::RightHand)
    };

    // =========================
    // Left leg
    limbs[toIndex(LimbType::PelvisToLeftHip)] =
    {
        toIndex(JointType::Pelvis),
        toIndex(JointType::LeftHip)
    };

    limbs[toIndex(LimbType::LeftThigh)] =
    {
        toIndex(JointType::LeftHip),
        toIndex(JointType::LeftKnee)
    };

    limbs[toIndex(LimbType::LeftShin)] =
    {
        toIndex(JointType::LeftKnee),
        toIndex(JointType::LeftAnkle)
    };

    limbs[toIndex(LimbType::LeftFoot)] =
    {
        toIndex(JointType::LeftAnkle),
        toIndex(JointType::LeftFoot)
    };

    // =========================
    // Right leg
    limbs[toIndex(LimbType::PelvisToRightHip)] =
    {
        toIndex(JointType::Pelvis),
        toIndex(JointType::RightHip)
    };

    limbs[toIndex(LimbType::RightThigh)] =
    {
        toIndex(JointType::RightHip),
        toIndex(JointType::RightKnee)
    };

    limbs[toIndex(LimbType::RightShin)] =
    {
        toIndex(JointType::RightKnee),
        toIndex(JointType::RightAnkle)
    };

    limbs[toIndex(LimbType::RightFoot)] =
    {
        toIndex(JointType::RightAnkle),
        toIndex(JointType::RightFoot)
    };

    return limbs;
}


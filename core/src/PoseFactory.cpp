#include "PoseFactory.h"
#include "EnumUtils.h"

static std::vector<Joint> initializeJoints(const BodyProportions &body);
static std::vector<Limb> initializeLimbs();

// Initializes a pose using default body proportions, and optional user provided
// proportions. The default pose is a standing pose with the arms by the side,
// legs straight, and the head facing forward.
Pose createDefaultPose(const UserProportions &user)
{
    BodyProportions body = estimateBodyProportions(user);
    return createPoseFromBodyProportions(body);
}

// Initializes a pose using the provided body proportions...
// Later on, can reimplement this function to allow user to
// specify different body proportions... ex: long torso...
Pose createPoseFromBodyProportions(const BodyProportions &body)
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
    joints[toIndex(JointType::Pelvis)] =
        {
            {0.0, 0.0, 0.0},
            JointType::Pelvis};

    // =========================
    // Torso
    joints[toIndex(JointType::Spine)] =
        {
            {0.0, body.torsoLength * 0.5, 0.0},
            JointType::Spine};

    joints[toIndex(JointType::Chest)] =
        {
            {0.0, body.torsoLength, 0.0},
            JointType::Chest};

    joints[toIndex(JointType::Neck)] =
        {
            {0.0, body.torsoLength + body.neckLength, 0.0},
            JointType::Neck};

    joints[toIndex(JointType::Head)] =
        {
            {0.0, body.torsoLength + body.neckLength + body.headHeight, 0.0},
            JointType::Head};

    // =========================
    // Shoulders
    joints[toIndex(JointType::LeftShoulder)] =
        {
            {-halfShoulderWidth, body.torsoLength, 0.0},
            JointType::LeftShoulder};

    joints[toIndex(JointType::RightShoulder)] =
        {
            {halfShoulderWidth, body.torsoLength, 0.0},
            JointType::RightShoulder};

    // =========================
    // Left arm, hanging straight down
    joints[toIndex(JointType::LeftElbow)] =
        {
            {-halfShoulderWidth, body.torsoLength - body.upperArmLength, 0.0},
            JointType::LeftElbow};

    joints[toIndex(JointType::LeftWrist)] =
        {
            {-halfShoulderWidth, body.torsoLength - body.upperArmLength - body.forearmLength, 0.0},
            JointType::LeftWrist};

    joints[toIndex(JointType::LeftHand)] =
        {
            {-halfShoulderWidth,
             body.torsoLength - body.upperArmLength - body.forearmLength - body.handLength,
             0.0},
            JointType::LeftHand};

    // =========================
    // Right arm, hanging straight down
    joints[toIndex(JointType::RightElbow)] =
        {
            {halfShoulderWidth, body.torsoLength - body.upperArmLength, 0.0},
            JointType::RightElbow};

    joints[toIndex(JointType::RightWrist)] =
        {
            {halfShoulderWidth, body.torsoLength - body.upperArmLength - body.forearmLength, 0.0},
            JointType::RightWrist};

    joints[toIndex(JointType::RightHand)] =
        {
            {halfShoulderWidth,
             body.torsoLength - body.upperArmLength - body.forearmLength - body.handLength,
             0.0},
            JointType::RightHand};

    // =========================
    // Hips
    joints[toIndex(JointType::LeftHip)] =
        {
            {-halfHipWidth, 0.0, 0.0},
            JointType::LeftHip};

    joints[toIndex(JointType::RightHip)] =
        {
            {halfHipWidth, 0.0, 0.0},
            JointType::RightHip};

    // =========================
    // Left leg
    joints[toIndex(JointType::LeftKnee)] =
        {
            {-halfHipWidth, -body.upperLegLength, 0.0},
            JointType::LeftKnee};

    joints[toIndex(JointType::LeftAnkle)] =
        {
            {-halfHipWidth, -body.upperLegLength - body.lowerLegLength, 0.0},
            JointType::LeftAnkle};

    joints[toIndex(JointType::LeftFoot)] =
        {
            {-halfHipWidth, -body.upperLegLength - body.lowerLegLength, body.footLength},
            JointType::LeftFoot};

    // =========================
    // Right leg
    joints[toIndex(JointType::RightKnee)] =
        {
            {halfHipWidth, -body.upperLegLength, 0.0},
            JointType::RightKnee};

    joints[toIndex(JointType::RightAnkle)] =
        {
            {halfHipWidth, -body.upperLegLength - body.lowerLegLength, 0.0},
            JointType::RightAnkle};

    joints[toIndex(JointType::RightFoot)] =
        {
            {halfHipWidth, -body.upperLegLength - body.lowerLegLength, body.footLength},
            JointType::RightFoot};

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
            toIndex(JointType::Spine),
            LimbType::PelvisToSpine};

    limbs[toIndex(LimbType::SpineToChest)] =
        {
            toIndex(JointType::Spine),
            toIndex(JointType::Chest),
            LimbType::SpineToChest};

    limbs[toIndex(LimbType::ChestToNeck)] =
        {
            toIndex(JointType::Chest),
            toIndex(JointType::Neck),
            LimbType::ChestToNeck};

    limbs[toIndex(LimbType::NeckToHead)] =
        {
            toIndex(JointType::Neck),
            toIndex(JointType::Head),
            LimbType::NeckToHead};

    // =========================
    // Left arm
    limbs[toIndex(LimbType::ChestToLeftShoulder)] =
        {
            toIndex(JointType::Chest),
            toIndex(JointType::LeftShoulder),
            LimbType::ChestToLeftShoulder};

    limbs[toIndex(LimbType::LeftUpperArm)] =
        {
            toIndex(JointType::LeftShoulder),
            toIndex(JointType::LeftElbow),
            LimbType::LeftUpperArm};

    limbs[toIndex(LimbType::LeftForearm)] =
        {
            toIndex(JointType::LeftElbow),
            toIndex(JointType::LeftWrist),
            LimbType::LeftForearm};

    limbs[toIndex(LimbType::LeftHand)] =
        {
            toIndex(JointType::LeftWrist),
            toIndex(JointType::LeftHand),
            LimbType::LeftHand};

    // =========================
    // Right arm
    limbs[toIndex(LimbType::ChestToRightShoulder)] =
        {
            toIndex(JointType::Chest),
            toIndex(JointType::RightShoulder),
            LimbType::ChestToRightShoulder};

    limbs[toIndex(LimbType::RightUpperArm)] =
        {
            toIndex(JointType::RightShoulder),
            toIndex(JointType::RightElbow),
            LimbType::RightUpperArm};

    limbs[toIndex(LimbType::RightForearm)] =
        {
            toIndex(JointType::RightElbow),
            toIndex(JointType::RightWrist),
            LimbType::RightForearm};

    limbs[toIndex(LimbType::RightHand)] =
        {
            toIndex(JointType::RightWrist),
            toIndex(JointType::RightHand),
            LimbType::RightHand};

    // =========================
    // Left leg
    limbs[toIndex(LimbType::PelvisToLeftHip)] =
        {
            toIndex(JointType::Pelvis),
            toIndex(JointType::LeftHip),
            LimbType::PelvisToLeftHip};

    limbs[toIndex(LimbType::LeftThigh)] =
        {
            toIndex(JointType::LeftHip),
            toIndex(JointType::LeftKnee),
            LimbType::LeftThigh};

    limbs[toIndex(LimbType::LeftShin)] =
        {
            toIndex(JointType::LeftKnee),
            toIndex(JointType::LeftAnkle),
            LimbType::LeftShin};

    limbs[toIndex(LimbType::LeftFoot)] =
        {
            toIndex(JointType::LeftAnkle),
            toIndex(JointType::LeftFoot),
            LimbType::LeftFoot};

    // =========================
    // Right leg
    limbs[toIndex(LimbType::PelvisToRightHip)] =
        {
            toIndex(JointType::Pelvis),
            toIndex(JointType::RightHip),
            LimbType::PelvisToRightHip};

    limbs[toIndex(LimbType::RightThigh)] =
        {
            toIndex(JointType::RightHip),
            toIndex(JointType::RightKnee),
            LimbType::RightThigh};

    limbs[toIndex(LimbType::RightShin)] =
        {
            toIndex(JointType::RightKnee),
            toIndex(JointType::RightAnkle),
            LimbType::RightShin};

    limbs[toIndex(LimbType::RightFoot)] =
        {
            toIndex(JointType::RightAnkle),
            toIndex(JointType::RightFoot),
            LimbType::RightFoot};

    return limbs;
}

#include "Pose.h"
#include "BodyProportions.h"
#include "EnumUtils.h"


Pose createDefaultPose(const UserProportions &user)
{
    BodyProportions body = estimateBodyProportions(user);

    Pose pose;
    pose.joints.resize(toIndex(JointType::JointCount));

    auto &joints = pose.joints;

    const double halfShoulderWidth = body.shoulderWidth / 2.0;
    const double hipWidth = body.shoulderWidth * 0.7;
    const double halfHipWidth = hipWidth / 2.0;

    // Pelvis is the root.
    joints[toIndex(JointType::Pelvis)].position = {0.0, 0.0, 0.0};

    // Torso
    joints[toIndex(JointType::Spine)].position = 
        {0.0, body.torsoLength * 0.5, 0.0};

    joints[toIndex(JointType::Chest)].position = 
        {0.0, body.torsoLength, 0.0};

    joints[toIndex(JointType::Neck)].position = 
        {0.0, body.torsoLength + body.neckLength, 0.0};

    joints[toIndex(JointType::Head)].position = 
        {0.0, body.torsoLength + body.neckLength + body.headHeight, 0.0};

    // Shoulders
    joints[toIndex(JointType::LeftShoulder)].position = 
        {-halfShoulderWidth, body.torsoLength, 0.0};

    joints[toIndex(JointType::RightShoulder)].position = 
        {halfShoulderWidth, body.torsoLength, 0.0};

    // Left arm, hanging straight down
    joints[toIndex(JointType::LeftElbow)].position = 
        {-halfShoulderWidth, body.torsoLength - body.upperArmLength, 0.0};

    joints[toIndex(JointType::LeftWrist)].position = 
        {-halfShoulderWidth, body.torsoLength - body.upperArmLength - body.forearmLength, 0.0};

    joints[toIndex(JointType::LeftHand)].position = 
        {-halfShoulderWidth, body.torsoLength - body.upperArmLength - body.forearmLength - body.handLength, 0.0};

    // Right arm, hanging straight down
    joints[toIndex(JointType::RightElbow)].position = 
        {halfShoulderWidth, body.torsoLength - body.upperArmLength, 0.0};

    joints[toIndex(JointType::RightWrist)].position = 
        {halfShoulderWidth, body.torsoLength - body.upperArmLength - body.forearmLength, 0.0};

    joints[toIndex(JointType::RightHand)].position = 
        {halfShoulderWidth, body.torsoLength - body.upperArmLength - body.forearmLength - body.handLength, 0.0};

    // Hips
    joints[toIndex(JointType::LeftHip)].position = 
        {-halfHipWidth, 0.0, 0.0};

    joints[toIndex(JointType::RightHip)].position = 
        {halfHipWidth, 0.0, 0.0};

    // Left leg
    joints[toIndex(JointType::LeftKnee)].position = 
        {-halfHipWidth, -body.upperLegLength, 0.0};

    joints[toIndex(JointType::LeftAnkle)].position = 
        {-halfHipWidth, -body.upperLegLength - body.lowerLegLength, 0.0};

    joints[toIndex(JointType::LeftFoot)].position = 
        {-halfHipWidth, -body.upperLegLength - body.lowerLegLength, body.footLength};

    // Right leg
    joints[toIndex(JointType::RightKnee)].position = 
        {halfHipWidth, -body.upperLegLength, 0.0};

    joints[toIndex(JointType::RightAnkle)].position = 
        {halfHipWidth, -body.upperLegLength - body.lowerLegLength, 0.0};

    joints[toIndex(JointType::RightFoot)].position = 
        {halfHipWidth, -body.upperLegLength - body.lowerLegLength, body.footLength};

    return pose;
}
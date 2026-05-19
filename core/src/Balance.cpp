#include "Balance.h"
#include <stdexcept>


static double getSegmentMass(const Limb& limb, const BodyProportions& body);
static double getSegmentMassPercentage(LimbType limb);

Vec3 calculateCenterOfGravity(const Pose& pose)
{
    Vec3 weightedSum{0.0, 0.0, 0.0};
    double totalMass = 0.0;

    for (const Limb& limb : pose.limbs) {
        const Joint& start = pose.joints[limb.startJointIndex];
        const Joint& end = pose.joints[limb.endJointIndex];

        Vec3 segmentCenter = (start.position + end.position) / 2.0;
        double segmentMass = getSegmentMass(limb, pose.body);

        weightedSum += segmentCenter * segmentMass;
        totalMass += segmentMass;
    }

    return weightedSum / totalMass;
}


static double getSegmentMass(const Limb& limb, const BodyProportions& body)
{
    return body.weight * getSegmentMassPercentage(limb.type);
}

static double getSegmentMassPercentage(LimbType limb) {
    switch (limb) {
        // =========================
        // Torso / Head

        case LimbType::PelvisToSpine:
            return 0.16;

        case LimbType::SpineToChest:
            return 0.16;

        case LimbType::ChestToNeck:
            return 0.08;

        case LimbType::NeckToHead:
            return 0.08;

        // =========================
        // Left Arm

        case LimbType::ChestToLeftShoulder:
            return 0.01;

        case LimbType::LeftUpperArm:
            return 0.03;

        case LimbType::LeftForearm:
            return 0.02;

        case LimbType::LeftHand:
            return 0.01;

        // =========================
        // Right Arm

        case LimbType::ChestToRightShoulder:
            return 0.01;

        case LimbType::RightUpperArm:
            return 0.03;

        case LimbType::RightForearm:
            return 0.02;

        case LimbType::RightHand:
            return 0.01;

        // =========================
        // Left Leg

        case LimbType::PelvisToLeftHip:
            return 0.02;

        case LimbType::LeftThigh:
            return 0.10;

        case LimbType::LeftShin:
            return 0.05;

        case LimbType::LeftFoot:
            return 0.02;

        // =========================
        // Right Leg

        case LimbType::PelvisToRightHip:
            return 0.02;

        case LimbType::RightThigh:
            return 0.10;

        case LimbType::RightShin:
            return 0.05;

        case LimbType::RightFoot:
            return 0.02;


        default:
            throw std::invalid_argument("Unknown limb type.");
    }
}
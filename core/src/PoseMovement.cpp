#include "PoseMovement.h"
#include "EnumUtils.h"
#include <stdexcept>
#include <IKSolver.h>

static bool isPoseInitialized(const Pose& pose);
void translatePose(Pose &pose, const Vec3 &translation)
{
    if (!isPoseInitialized(pose))
    {
        throw std::invalid_argument("Pose is not initialized.");
    }

    for (Joint &joint : pose.joints)
    {
        joint.position += translation;
    }
}
void moveJointTo(Pose& pose, JointType joint, const WallPoint& targetPosition)
{
    TwoBoneChain chain;

    if (getTwoBoneChainForEndJoint(joint, chain))
    {
        Vec3 target = {targetPosition.x, targetPosition.y, 0.0};
        solveTwoBoneIK(pose, chain.root, chain.middle, chain.end, target);
        return;
    }

    pose.joints[toIndex(joint)].position = {targetPosition.x, targetPosition.y, 0.0};
}

static bool isPoseInitialized(const Pose &pose)
{
    return pose.joints.size() == toIndex(JointType::JointCount);
}

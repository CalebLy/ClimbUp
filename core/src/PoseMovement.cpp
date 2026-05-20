#include "PoseMovement.h"
#include "EnumUtils.h"
#include <stdexcept>
#include <IKSolver.h>
#include <Validation.h>
#include "ClimbMath.h"

static Vec3 clampTargetToReach(const Pose& pose, JointType root, JointType middle, JointType wristOrAnkle,
                               JointType handOrFoot, const Vec3& target);
void translatePose(Pose &pose, const Vec3 &translation)
{
    
    validatePoseInitialized(pose);

    for (Joint &joint : pose.joints)
    {
        joint.position += translation;
    }
}
void moveJointTo(Pose& pose, JointType joint, const WallPoint& targetPosition)
{
    Vec3 target = {targetPosition.x, targetPosition.y, 0.0};

    TwoBoneChain chain;

    if (getTwoBoneChainForEndJoint(joint, chain))
    {
        Vec3 clampedTarget = clampTargetToReach(pose, chain.root, chain.middle, chain.end, joint, target);
        Vec3 ikTarget = getWristOrAnkleTarget(pose, chain.root, chain.end, joint, clampedTarget);

        solveTwoBoneIK(pose, chain.root, chain.middle, chain.end, ikTarget);

        pose.joints[toIndex(joint)].position = clampedTarget;

        return;
    }

    pose.joints[toIndex(joint)].position = target;
}

static Vec3 clampTargetToReach(const Pose& pose, JointType root, JointType middle, JointType wristOrAnkle,
                               JointType handOrFoot, const Vec3& target)
{
    Vec3 rootPos = pose.joints[toIndex(root)].position;
    Vec3 middlePos = pose.joints[toIndex(middle)].position;
    Vec3 wristOrAnklePos = pose.joints[toIndex(wristOrAnkle)].position;
    Vec3 handOrFootPos = pose.joints[toIndex(handOrFoot)].position;

    double lengthA = distance(rootPos, middlePos);
    double lengthB = distance(middlePos, wristOrAnklePos);
    double lengthC = distance(wristOrAnklePos, handOrFootPos);

    double maxReach = lengthA + lengthB + lengthC;

    Vec3 rootToTarget = target - rootPos;
    double targetDistance = rootToTarget.length();

    if (targetDistance <= 0.0 || targetDistance <= maxReach) {
        return target;
    }

    Vec3 direction = rootToTarget / targetDistance;
    return rootPos + direction * maxReach;
}
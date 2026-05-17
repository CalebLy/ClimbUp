#include "PoseMovement.h"
#include "EnumUtils.h"
#include <stdexcept>

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
void moveJointTo(Pose &pose, JointType joint, const WallPoint &targetPosition)
{
}

static bool isPoseInitialized(const Pose &pose)
{
    return pose.joints.size() == toIndex(JointType::JointCount);
}

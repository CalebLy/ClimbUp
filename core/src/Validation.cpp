#include "Validation.h"
#include <EnumUtils.h>

bool isPoseInitialized(const Pose& pose) {
    return pose.joints.size() == toIndex(JointType::JointCount);
}

// Throws if pose is not initialized
void validatePoseInitialized(const Pose& pose)
{
    if (!isPoseInitialized(pose))
    {
        throw std::invalid_argument("Pose is not initialized.");
    }
}
// TODO: when you figure out how you want to compile, you would make sure
//       that core/include is in the include path
#include "ClimbMath.h"
#include "Pose.h"
#include <cstddef>
#include <vector>
#include <cmath>

double getDistanceBetweenJoints(const Pose &pose, JointType jointA, JointType jointB)
{
    const Joint &a = pose.joints[static_cast<int>(jointA)];
    const Joint &b = pose.joints[static_cast<int>(jointB)];

    double dx = a.position.x - b.position.x;
    double dy = a.position.y - b.position.y;
    double dz = a.position.z - b.position.z;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
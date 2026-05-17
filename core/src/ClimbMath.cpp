// TODO: when you figure out how you want to compile, you would make sure
//       that core/include is in the include path
#include "ClimbMath.h"
#include "Pose.h"
#include "EnumUtils.h"
#include <cstddef>
#include <vector>
#include <cmath>


double distance(const Vec3& a, const Vec3& b)
{
    return (a - b).length();
}

double getDistanceBetweenJoints(const Pose& pose, JointType jointA, JointType jointB)
{
    const Joint& a = pose.joints[toIndex(jointA)];
    const Joint& b = pose.joints[toIndex(jointB)];

    return distance(a.position, b.position);
}
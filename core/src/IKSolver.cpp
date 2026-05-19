#include "IKSolver.h"
#include "Pose.h"
#include "EnumUtils.h"
#include <ClimbMath.h>


bool solveTwoBoneIK(Pose &pose, JointType root, JointType middle, JointType end, const Vec3 &target)
{

    int rootIndex = toIndex(root);
    int middleIndex = toIndex(middle);
    int endIndex = toIndex(end);

    Vec3 rootPos = pose.joints[rootIndex].position;
    Vec3 middlePos = pose.joints[middleIndex].position;
    Vec3 endPos = pose.joints[endIndex].position;

    double lengthA = distance(rootPos, middlePos);
    double lengthB = distance(middlePos, endPos);

    if (lengthA <= 0.0 || lengthB <= 0.0) {
        return false;
    }

    Vec3 rootToTarget = target - rootPos;
    double targetDistance = rootToTarget.length();

    if (targetDistance <= 0.0) {
        return false;
    }

    // Clamp the target so the arm/leg does not stretch beyond its max reach.
    double maxReach = lengthA + lengthB;
    double minReach = std::abs(lengthA - lengthB);

    double clampedDistance = std::max(minReach, std::min(targetDistance, maxReach));

    Vec3 direction = rootToTarget / targetDistance;

    // Point along the root-to-target line where the middle joint projects.
    double a = (lengthA * lengthA - lengthB * lengthB + clampedDistance * clampedDistance)
             / (2.0 * clampedDistance);

    double hSquared = lengthA * lengthA - a * a;

    if (hSquared < 0.0) {
        hSquared = 0.0;
    }

    double h = std::sqrt(hSquared);

    Vec3 basePoint = rootPos + direction * a;

    // Perpendicular direction in XY plane.
    // TODO: When we implement 3D IK, this should be 3D.
    Vec3 perpendicular = {-direction.y, direction.x, 0.0};

    // Decide which side the elbow/knee should bend toward.
    Vec3 currentRootToMiddle = middlePos - rootPos;

    double side =
        currentRootToMiddle.x * perpendicular.x +
        currentRootToMiddle.y * perpendicular.y;

    if (side < 0.0) {
        perpendicular = perpendicular * -1.0;
    }

    Vec3 newMiddlePos = basePoint + perpendicular * h;
    Vec3 newEndPos = rootPos + direction * clampedDistance;

    pose.joints[middleIndex].position = newMiddlePos;
    pose.joints[endIndex].position = newEndPos;

    return true;
}

bool getTwoBoneChainForEndJoint(JointType joint, TwoBoneChain& chain)
{
    switch (joint)
    {
        case JointType::LeftHand:
            chain = {
                JointType::LeftShoulder,
                JointType::LeftElbow,
                JointType::LeftHand
            };
            return true;

        case JointType::RightHand:
            chain = {
                JointType::RightShoulder,
                JointType::RightElbow,
                JointType::RightHand
            };
            return true;

        case JointType::LeftFoot:
            chain = {
                JointType::LeftHip,
                JointType::LeftKnee,
                JointType::LeftFoot
            };
            return true;

        case JointType::RightFoot:
            chain = {
                JointType::RightHip,
                JointType::RightKnee,
                JointType::RightFoot
            };
            return true;

        default:
            return false;
    }
}
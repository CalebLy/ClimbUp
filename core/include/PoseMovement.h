#pragma once

#include "Pose.h"
#include "Wall.h"

// Default placement of a pose on the wall
void translatePose(Pose &pose, const Vec3 &translation);

void moveJointTo(Pose &pose, JointType joint, const WallPoint &targetPosition);
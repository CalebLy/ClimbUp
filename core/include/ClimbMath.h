#pragma once 

#include "Pose.h"

double distance(const Vec3& a, const Vec3& b);
double getDistanceBetweenJoints(const Pose& pose, JointType jointA, JointType jointB);


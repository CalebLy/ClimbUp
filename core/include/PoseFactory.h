#pragma once
#include "BodyProportions.h"
#include "Pose.h"

Pose createDefaultPose(const UserProportions& user = UserProportions());
Pose createPoseFromBodyProportions(const BodyProportions& body);    
#pragma once

#include <vector>
#include "Pose.h"

struct IKChain
{
    std::vector<JointType> joints;
};

struct TwoBoneChain
{
    JointType root;
    JointType middle;
    JointType end;
};

bool solveTwoBoneIK(Pose &pose, JointType root,
    JointType middle, JointType end, const Vec3 &target);

bool getTwoBoneChainForEndJoint(JointType joint, TwoBoneChain& chain);

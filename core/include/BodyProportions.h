#pragma once

#include "UserProportions.h"
struct BodyProportions
{
    double wingspan;

    double torsoLength;
    double neckLength;
    double headHeight;
    double shoulderWidth;

    double upperArmLength;
    double forearmLength;
    double handLength;

    double upperLegLength;
    double lowerLegLength;
    double footLength;
};

BodyProportions estimateBodyProportions(const UserProportions& user = UserProportions());
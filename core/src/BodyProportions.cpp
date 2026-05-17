#include "BodyProportions.h"

BodyProportions estimateBodyProportions(const UserProportions& user) {
    BodyProportions body;

    body.wingspan = user.height + user.apeIndex;

    body.headHeight = user.height * 0.13;
    body.neckLength = user.height * 0.04;
    body.torsoLength = user.height * 0.30;

    body.shoulderWidth = user.height * 0.23;

    body.upperLegLength = user.height * 0.245;
    body.lowerLegLength = user.height * 0.246;
    body.footLength = user.height * 0.15;

    double armLengthPerSide = (body.wingspan - body.shoulderWidth) / 2.0;

    body.handLength = user.height * 0.11;

    double remainingArmLength = armLengthPerSide - body.handLength;

    body.upperArmLength = remainingArmLength * 0.52;
    body.forearmLength = remainingArmLength * 0.48;

    return body;
}
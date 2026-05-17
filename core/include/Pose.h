#pragma once

#include <vector>
#include "BodyProportions.h"
#include "Vec3.h"

// Design note:
// Pose stores all joints in one shared vector. Each Joint owns its actual
// position data. Limbs do not store copies of joints; instead, each Limb
// stores the indices of the two joints it connects.
//
// This keeps the skeleton consistent. For example, if the elbow moves,
// every limb connected to that elbow automatically uses the updated position
// because they all reference the same joint in Pose::joints.
//
// Mental model:
// Joint = a point on the body
// Limb  = a connection between two joints
// Pose  = the full skeleton state
// Please refer to the diagram docs\diagrams\skeleton_joints_and_limbs.png

enum class JointType
{
    // Head / torso
    Head,
    Neck,
    Chest,
    Spine,
    Pelvis,

    // Left arm
    LeftShoulder,
    LeftElbow,
    LeftWrist,
    LeftHand,

    // Right arm
    RightShoulder,
    RightElbow,
    RightWrist,
    RightHand,

    // Left leg
    LeftHip,
    LeftKnee,
    LeftAnkle,
    LeftFoot,

    // Right leg
    RightHip,
    RightKnee,
    RightAnkle,
    RightFoot,

    JointCount
};

enum class LimbType
{
    // Torso
    PelvisToSpine,
    SpineToChest,
    ChestToNeck,
    NeckToHead,

    // Left arm
    ChestToLeftShoulder,
    LeftUpperArm,
    LeftForearm,
    LeftHand,

    // Right arm
    ChestToRightShoulder,
    RightUpperArm,
    RightForearm,
    RightHand,

    // Left leg
    PelvisToLeftHip,
    LeftThigh,
    LeftShin,
    LeftFoot,

    // Right leg
    PelvisToRightHip,
    RightThigh,
    RightShin,
    RightFoot,

    LimbCount
};

/**
 * Represents a single point on the climber's body.
 * Examples include elbows, knees, wrists, ankles, etc.
 */
struct Joint
{
    Vec3 position;
};

/**
 * Represents a connection between two joints in the skeleton.
 * Limbs reference shared joints inside Pose::joints using indices.
 */
struct Limb
{
    int startJointIndex;
    int endJointIndex;
};

/**
 * Represents the full state of the climber skeleton.
 * Stores all joints and the limb relationships between them.
 */
struct Pose
{
    std::vector<Joint> joints;
    std::vector<Limb> limbs;
    BodyProportions body;
};

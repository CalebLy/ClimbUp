// ClimbCore.cpp
#include "ClimbCore.h"

#include "PoseFactory.h"
#include "Balance.h"
#include "PoseMovement.h"
#include "EnumUtils.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/array.hpp>

using namespace godot;

// ========================================================
// Helper Conversion Functions
// ========================================================

static Dictionary vec3ToDictionary(const Vec3 &v)
{
    Dictionary dict;

    dict["x"] = v.x;
    dict["y"] = v.y;
    dict["z"] = v.z;

    return dict;
}

static Vec3 dictionaryToVec3(const Dictionary &dict)
{
    Vec3 v;

    v.x = static_cast<double>(dict["x"]);
    v.y = static_cast<double>(dict["y"]);
    v.z = static_cast<double>(dict["z"]);

    return v;
}

static Dictionary poseToDictionary(const Pose &pose)
{
    Dictionary result;

    // =========================
    // Joints

    Array joints_array;

    for (const Joint &joint : pose.joints)
    {
        Dictionary joint_dict;

        joint_dict["x"] = joint.position.x;
        joint_dict["y"] = joint.position.y;
        joint_dict["z"] = joint.position.z;
        joint_dict["type"] = static_cast<int>(joint.type);

        joints_array.append(joint_dict);
    }

    result["joints"] = joints_array;

    // =========================
    // Limbs

    Array limbs_array;

    for (const Limb &limb : pose.limbs)
    {
        Dictionary limb_dict;

        limb_dict["startJointIndex"] = limb.startJointIndex;
        limb_dict["endJointIndex"] = limb.endJointIndex;
        limb_dict["type"] = static_cast<int>(limb.type);

        limbs_array.append(limb_dict);
    }

    result["limbs"] = limbs_array;

    // =========================
    // Center Of Gravity

    Vec3 cog = calculateCenterOfGravity(pose);

    result["centerOfGravity"] = vec3ToDictionary(cog);
    Dictionary body_dict;

    body_dict["wingspan"] = pose.body.wingspan;
    body_dict["weight"] = pose.body.weight;

    body_dict["torsoLength"] = pose.body.torsoLength;
    body_dict["neckLength"] = pose.body.neckLength;
    body_dict["headHeight"] = pose.body.headHeight;
    body_dict["shoulderWidth"] = pose.body.shoulderWidth;

    body_dict["upperArmLength"] = pose.body.upperArmLength;
    body_dict["forearmLength"] = pose.body.forearmLength;
    body_dict["handLength"] = pose.body.handLength;

    body_dict["upperLegLength"] = pose.body.upperLegLength;
    body_dict["lowerLegLength"] = pose.body.lowerLegLength;
    body_dict["footLength"] = pose.body.footLength;

    result["body"] = body_dict;

    return result;
}

static Pose dictionaryToPose(const Dictionary &data)
{
    Pose pose;

    // =========================
    // Joints

    Array joints_array = data["joints"];

    for (int i = 0; i < joints_array.size(); i++)
    {
        Dictionary joint_dict = joints_array[i];

        Joint joint;

        joint.position = dictionaryToVec3(joint_dict);
        joint.type = static_cast<JointType>(static_cast<int>(joint_dict["type"]));

        pose.joints.push_back(joint);
    }

    // =========================
    // Limbs

    Array limbs_array = data["limbs"];

    for (int i = 0; i < limbs_array.size(); i++)
    {
        Dictionary limb_dict = limbs_array[i];

        Limb limb;

        limb.startJointIndex = static_cast<int>(limb_dict["startJointIndex"]);
        limb.endJointIndex = static_cast<int>(limb_dict["endJointIndex"]);
        limb.type = static_cast<LimbType>(static_cast<int>(limb_dict["type"]));

        pose.limbs.push_back(limb);
    }
    if (data.has("body"))
    {
        Dictionary body_dict = data["body"];

        pose.body.wingspan = static_cast<double>(body_dict["wingspan"]);
        pose.body.weight = static_cast<double>(body_dict["weight"]);

        pose.body.torsoLength = static_cast<double>(body_dict["torsoLength"]);
        pose.body.neckLength = static_cast<double>(body_dict["neckLength"]);
        pose.body.headHeight = static_cast<double>(body_dict["headHeight"]);
        pose.body.shoulderWidth = static_cast<double>(body_dict["shoulderWidth"]);

        pose.body.upperArmLength = static_cast<double>(body_dict["upperArmLength"]);
        pose.body.forearmLength = static_cast<double>(body_dict["forearmLength"]);
        pose.body.handLength = static_cast<double>(body_dict["handLength"]);

        pose.body.upperLegLength = static_cast<double>(body_dict["upperLegLength"]);
        pose.body.lowerLegLength = static_cast<double>(body_dict["lowerLegLength"]);
        pose.body.footLength = static_cast<double>(body_dict["footLength"]);
    }

    return pose;
}

static String jointTypeToString(JointType joint)
{
    switch (joint)
    {
    case JointType::Head:
        return "Head";
    case JointType::Neck:
        return "Neck";
    case JointType::Chest:
        return "Chest";
    case JointType::Spine:
        return "Spine";
    case JointType::Pelvis:
        return "Pelvis";
    case JointType::LeftShoulder:
        return "LeftShoulder";
    case JointType::LeftElbow:
        return "LeftElbow";
    case JointType::LeftWrist:
        return "LeftWrist";
    case JointType::LeftHand:
        return "LeftHand";
    case JointType::RightShoulder:
        return "RightShoulder";
    case JointType::RightElbow:
        return "RightElbow";
    case JointType::RightWrist:
        return "RightWrist";
    case JointType::RightHand:
        return "RightHand";
    case JointType::LeftHip:
        return "LeftHip";
    case JointType::LeftKnee:
        return "LeftKnee";
    case JointType::LeftAnkle:
        return "LeftAnkle";
    case JointType::LeftFoot:
        return "LeftFoot";
    case JointType::RightHip:
        return "RightHip";
    case JointType::RightKnee:
        return "RightKnee";
    case JointType::RightAnkle:
        return "RightAnkle";
    case JointType::RightFoot:
        return "RightFoot";
    default:
        return "Unknown";
    }
}

static String limbTypeToString(LimbType limb)
{
    switch (limb)
    {
    case LimbType::PelvisToSpine:
        return "PelvisToSpine";
    case LimbType::SpineToChest:
        return "SpineToChest";
    case LimbType::ChestToNeck:
        return "ChestToNeck";
    case LimbType::NeckToHead:
        return "NeckToHead";
    case LimbType::ChestToLeftShoulder:
        return "ChestToLeftShoulder";
    case LimbType::LeftUpperArm:
        return "LeftUpperArm";
    case LimbType::LeftForearm:
        return "LeftForearm";
    case LimbType::LeftHand:
        return "LeftHand";
    case LimbType::ChestToRightShoulder:
        return "ChestToRightShoulder";
    case LimbType::RightUpperArm:
        return "RightUpperArm";
    case LimbType::RightForearm:
        return "RightForearm";
    case LimbType::RightHand:
        return "RightHand";
    case LimbType::PelvisToLeftHip:
        return "PelvisToLeftHip";
    case LimbType::LeftThigh:
        return "LeftThigh";
    case LimbType::LeftShin:
        return "LeftShin";
    case LimbType::LeftFoot:
        return "LeftFoot";
    case LimbType::PelvisToRightHip:
        return "PelvisToRightHip";
    case LimbType::RightThigh:
        return "RightThigh";
    case LimbType::RightShin:
        return "RightShin";
    case LimbType::RightFoot:
        return "RightFoot";
    default:
        return "Unknown";
    }
}

static LimbType stringToLimbType(String limb)
{
    if (limb == "PelvisToSpine")
        return LimbType::PelvisToSpine;
    if (limb == "SpineToChest")
        return LimbType::SpineToChest;
    if (limb == "ChestToNeck")
        return LimbType::ChestToNeck;
    if (limb == "NeckToHead")
        return LimbType::NeckToHead;
    if (limb == "ChestToLeftShoulder")
        return LimbType::ChestToLeftShoulder;
    if (limb == "LeftUpperArm")
        return LimbType::LeftUpperArm;
    if (limb == "LeftForearm")
        return LimbType::LeftForearm;
    if (limb == "LeftHand")
        return LimbType::LeftHand;
    if (limb == "ChestToRightShoulder")
        return LimbType::ChestToRightShoulder;
    if (limb == "RightUpperArm")
        return LimbType::RightUpperArm;
    if (limb == "RightForearm")
        return LimbType::RightForearm;
    if (limb == "RightHand")
        return LimbType::RightHand;
    if (limb == "PelvisToLeftHip")
        return LimbType::PelvisToLeftHip;
    if (limb == "LeftThigh")
        return LimbType::LeftThigh;
    if (limb == "LeftShin")
        return LimbType::LeftShin;
    if (limb == "LeftFoot")
        return LimbType::LeftFoot;
    if (limb == "PelvisToRightHip")
        return LimbType::PelvisToRightHip;
    if (limb == "RightThigh")
        return LimbType::RightThigh;
    if (limb == "RightShin")
        return LimbType::RightShin;
    if (limb == "RightFoot")
        return LimbType::RightFoot;

    return LimbType::PelvisToSpine;
}

static JointType stringToJointType(String joint)
{
    if (joint == "Head")
        return JointType::Head;

    if (joint == "Neck")
        return JointType::Neck;

    if (joint == "Chest")
        return JointType::Chest;

    if (joint == "Spine")
        return JointType::Spine;

    if (joint == "Pelvis")
        return JointType::Pelvis;

    if (joint == "LeftShoulder")
        return JointType::LeftShoulder;

    if (joint == "LeftElbow")
        return JointType::LeftElbow;

    if (joint == "LeftWrist")
        return JointType::LeftWrist;

    if (joint == "LeftHand")
        return JointType::LeftHand;

    if (joint == "RightShoulder")
        return JointType::RightShoulder;

    if (joint == "RightElbow")
        return JointType::RightElbow;

    if (joint == "RightWrist")
        return JointType::RightWrist;

    if (joint == "RightHand")
        return JointType::RightHand;

    if (joint == "LeftHip")
        return JointType::LeftHip;

    if (joint == "LeftKnee")
        return JointType::LeftKnee;

    if (joint == "LeftAnkle")
        return JointType::LeftAnkle;

    if (joint == "LeftFoot")
        return JointType::LeftFoot;

    if (joint == "RightHip")
        return JointType::RightHip;

    if (joint == "RightKnee")
        return JointType::RightKnee;

    if (joint == "RightAnkle")
        return JointType::RightAnkle;

    if (joint == "RightFoot")
        return JointType::RightFoot;

    return JointType::Pelvis;
}

// ========================================================
// Godot Bindings
// ========================================================

void ClimbCore::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("create_default_pose"), &ClimbCore::create_default_pose);

    ClassDB::bind_method(
        D_METHOD("translate_pose", "pose_data", "x", "y", "z"),
        &ClimbCore::translate_pose);

    ClassDB::bind_method(
        D_METHOD("move_joint_to", "pose_data", "joint_type", "x", "y"),
        &ClimbCore::move_joint_to);

    ClassDB::bind_method(D_METHOD("get_joint_position", "joint_name"),
                         &ClimbCore::get_joint_position);

    ClassDB::bind_method(D_METHOD("set_joint_position", "joint_name", "position"),
                         &ClimbCore::set_joint_position);

    ClassDB::bind_method(D_METHOD("get_body_data"),
                         &ClimbCore::get_body_data);

    ClassDB::bind_method(D_METHOD("set_body_data", "body_data"),
                         &ClimbCore::set_body_data);

    ClassDB::bind_method(D_METHOD("get_limb_data"),
                         &ClimbCore::get_limb_data);

    ClassDB::bind_method(D_METHOD("set_limb_data", "limbs"),
                         &ClimbCore::set_limb_data);

    ClassDB::bind_method(D_METHOD("get_center_of_gravity"),
                         &ClimbCore::get_center_of_gravity);
}

Dictionary ClimbCore::create_default_pose()
{
    UserProportions user = {};

    pose = createDefaultPose(user);

    return poseToDictionary(pose);
}

Dictionary ClimbCore::translate_pose(Dictionary pose_data, double x, double y, double z)
{
    pose = dictionaryToPose(pose_data);

    translatePose(pose, Vec3{x, y, z});

    return poseToDictionary(pose);
}

Dictionary ClimbCore::move_joint_to(Dictionary pose_data, int joint_type, double x, double y)
{
    pose = dictionaryToPose(pose_data);

    WallPoint target = {x, y};

    moveJointTo(pose, static_cast<JointType>(joint_type), target);

    return poseToDictionary(pose);
}

Vector3 ClimbCore::get_joint_position(String joint_name)
{
    JointType joint = stringToJointType(joint_name);

    Vec3 pos = pose.joints[toIndex(joint)].position;

    return Vector3(pos.x, pos.y, pos.z);
}

void ClimbCore::set_joint_position(String joint_name, Vector3 position)
{
    JointType jointType = stringToJointType(joint_name);
    int index = toIndex(jointType);

    pose.joints[index].position = {
        position.x,
        position.y,
        position.z
    };
}

Dictionary ClimbCore::get_body_data()
{
    Dictionary body;

    body["wingspan"] = pose.body.wingspan;
    body["weight"] = pose.body.weight;

    body["torso_length"] = pose.body.torsoLength;
    body["neck_length"] = pose.body.neckLength;
    body["head_height"] = pose.body.headHeight;
    body["shoulder_width"] = pose.body.shoulderWidth;

    body["upper_arm_length"] = pose.body.upperArmLength;
    body["forearm_length"] = pose.body.forearmLength;
    body["hand_length"] = pose.body.handLength;

    body["upper_leg_length"] = pose.body.upperLegLength;
    body["lower_leg_length"] = pose.body.lowerLegLength;
    body["foot_length"] = pose.body.footLength;

    return body;
}

void ClimbCore::set_body_data(Dictionary body_data)
{
    pose.body.wingspan = double(body_data.get("wingspan", pose.body.wingspan));
    pose.body.weight = double(body_data.get("weight", pose.body.weight));

    pose.body.torsoLength = double(body_data.get("torso_length", pose.body.torsoLength));
    pose.body.neckLength = double(body_data.get("neck_length", pose.body.neckLength));
    pose.body.headHeight = double(body_data.get("head_height", pose.body.headHeight));
    pose.body.shoulderWidth = double(body_data.get("shoulder_width", pose.body.shoulderWidth));

    pose.body.upperArmLength = double(body_data.get("upper_arm_length", pose.body.upperArmLength));
    pose.body.forearmLength = double(body_data.get("forearm_length", pose.body.forearmLength));
    pose.body.handLength = double(body_data.get("hand_length", pose.body.handLength));

    pose.body.upperLegLength = double(body_data.get("upper_leg_length", pose.body.upperLegLength));
    pose.body.lowerLegLength = double(body_data.get("lower_leg_length", pose.body.lowerLegLength));
    pose.body.footLength = double(body_data.get("foot_length", pose.body.footLength));
}

Array ClimbCore::get_limb_data()
{
    Array limbs;

    for (const Limb &limb : pose.limbs)
    {
        Dictionary limb_data;

        limb_data["type"] = limbTypeToString(limb.type);
        limb_data["start_joint"] = jointTypeToString(static_cast<JointType>(limb.startJointIndex));
        limb_data["end_joint"] = jointTypeToString(static_cast<JointType>(limb.endJointIndex));
        limb_data["start_joint_index"] = limb.startJointIndex;
        limb_data["end_joint_index"] = limb.endJointIndex;

        limbs.append(limb_data);
    }

    return limbs;
}

void ClimbCore::set_limb_data(Array limbs)
{
    pose.limbs.clear();

    for (int i = 0; i < limbs.size(); i++)
    {
        Dictionary limb_data = limbs[i];

        Limb limb;

        limb.type = stringToLimbType(limb_data["type"]);
        limb.startJointIndex = int(limb_data["start_joint_index"]);
        limb.endJointIndex = int(limb_data["end_joint_index"]);

        pose.limbs.push_back(limb);
    }
}
godot::Vector3 ClimbCore::get_center_of_gravity() {
    Vec3 cog = calculateCenterOfGravity(pose);
    return godot::Vector3(cog.x, cog.y, cog.z);
}

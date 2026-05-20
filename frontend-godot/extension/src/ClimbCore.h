// ClimbCore.h
#pragma once
#include "Pose.h"
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

class ClimbCore : public RefCounted {
    GDCLASS(ClimbCore, RefCounted)

private:
    Pose pose;

protected:
    static void _bind_methods();

public:
    Dictionary create_default_pose();
    Dictionary translate_pose(Dictionary pose_data, double x, double y, double z);
    Dictionary move_joint_to(Dictionary pose_data, int joint_type, double x, double y);

    Vector3 get_joint_position(String joint_name);
    void set_joint_position(String joint_name, Vector3 position);

    Dictionary get_body_data();
    void set_body_data(Dictionary body_data);

    Array get_limb_data();
    void set_limb_data(Array limbs);
    godot::Vector3 ClimbCore::get_center_of_gravity();
};
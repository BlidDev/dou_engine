#pragma once
#include <espch.h>

namespace engine {
    float dist_vec3(Vector3& a, Vector3& b);
    void addop_vec3(Vector3* a, Vector3 b);
    Vector3 add_vec3(Vector3 a, Vector3 b);
    Vector3 sub_vec3(Vector3 a, Vector3 b);
    Vector3 mul_vec3(Vector3& a, Vector3& b);
    Vector3 mul_vec3_f(Vector3 a, float b);

    Vector3 normalize_vec3(Vector3 a);
    Vector3 vec3_cross_product(Vector3 a, Vector3 b);
    float get_magnitude(Vector3 a);

    Vector3 get_forward(Vector3& target, Vector3& position);
    Vector3 get_up(Vector3& up);
    Vector3 get_right(Vector3& target, Vector3& position, Vector3& up);
    Vector3 vec3_rotate_by_axis_angle(Vector3 v, Vector3 axis, float angle);
    float vec3_angle(Vector3 v1, Vector3 v2);
    Vector3 vec3_neg(Vector3 v);

    void camera_yaw(Camera* camera, float angle);
    void camera_pitch(Camera* camera, float angle, bool lock);
    void handle_mouse_delta(Camera* camera, Vector2 delta, bool lock);

    bool aabb_3d_intersects(Vector3 a, Vector3 a_s, Vector3 b, Vector3 b_s);



}

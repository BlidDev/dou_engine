#pragma once
#include <espch.h>
#include "components/camera.h"
namespace engine {
    float dist_vec3(glm::vec3& a, glm::vec3& b);
    //void addop_vec3(glm::vec3* a, glm::vec3 b);
    //glm::vec3 add_vec3(glm::vec3 a, glm::vec3 b);
    //glm::vec3 sub_vec3(glm::vec3 a, glm::vec3 b);
    //glm::vec3 mul_vec3(glm::vec3& a, glm::vec3& b);
    //glm::vec3 mul_vec3_f(glm::vec3 a, float b);

    //glm::vec3 normalize_vec3(glm::vec3 a);
    //glm::vec3 vec3_cross_product(glm::vec3 a, glm::vec3 b);
    float get_magnitude(glm::vec3 a);

    glm::vec3 get_forward(glm::vec3& target, glm::vec3& position);
    glm::vec3 thing(glm::vec3 yo);
    glm::vec3 get_up(glm::vec3& up);
    glm::vec3 get_right(glm::vec3& target, glm::vec3& position, glm::vec3& up);
    glm::vec3 vec3_rotate_by_axis_angle(glm::vec3 v, glm::vec3 axis, float angle);
    float vec3_angle(glm::vec3 v1, glm::vec3 v2);
    glm::vec3 vec3_neg(glm::vec3 v);

    void camera_yaw(CameraComp* camera, glm::vec3 position, float angle);
    void camera_pitch(CameraComp* camera, glm::vec3 position, float angle, bool lock);
    void handle_mouse_delta(CameraComp* camera, glm::vec3 position,glm::vec2 delta, bool lock);


    struct AABBReturn {
        bool x = false;
        bool y = false;
        bool z = false;

        explicit operator bool() {
            return x || y || z;
        }

        glm::vec3 to_glm();
    };
    AABBReturn aabb_3d_intersects(glm::vec3 a, glm::vec3 d, glm::vec3 a_s, glm::vec3 b, glm::vec3 b_s);


    void print_v4(const char* name, glm::vec4& v);
    void print_v3(const char* name, glm::vec3& v);
    void print_v2(const char* name, glm::vec2& v);

    std::string trim(const std::string& str);
}


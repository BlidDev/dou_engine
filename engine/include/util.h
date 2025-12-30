#pragma once
#include <espch.h>
#include "components/camera.h"
namespace engine {

    float wrap_angle(float angle, float min, float max);


    struct AABBReturn {
        bool x = false;
        bool y = false;
        bool z = false;

        explicit operator bool() {
            return x || y || z;
        }

        glm::vec3 to_glm();
    };

    /** 
     * does_aabb_intersects - Returns a simple AABB test
     * @a: object a's position
     * @a_s: object a's size
     * @b: object b's position
     * @b_s: object b's size
     */
    bool does_aabb_intersects(glm::vec3 a, glm::vec3 a_s, glm::vec3 b, glm::vec3 b_s);

    /** 
     * aabb_3d_intersects - Returns an AABB test on each axis
     * @d: move delta of object a
     * @a: object a's position
     * @a_s: object a's size
     * @b: object b's position
     * @b_s: object b's size
     */
    AABBReturn aabb_3d_intersects(glm::vec3 d, glm::vec3 a, glm::vec3 a_s, glm::vec3 b, glm::vec3 b_s);


    void print_v4(const char* name, glm::vec4& v);
    void print_v3(const char* name, glm::vec3& v);
    void print_v2(const char* name, glm::vec2& v);

    std::string trim(const std::string& str);
}


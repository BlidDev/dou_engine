#include "util.h"

namespace engine {
    
    float wrap_angle(float angle, float min, float max) {
        float range = max - min;
        float wrapped = std::fmod(angle - min, range);
        if (wrapped < 0)
            wrapped += range;
        return wrapped + min;
    }



    glm::vec3 AABBReturn::to_glm() {
        return {(float)(!x),(float)(!y),(float)(!z)};
    }

    static bool aabb_test_axis(glm::vec3 a, glm::vec3 a_s, glm::vec3 b, glm::vec3 b_s) {
        return 
           std::abs(a.x - b.x) * 2 < (a_s.x + b_s.x) &&
           std::abs(a.y - b.y) * 2 < (a_s.y + b_s.y) &&
           std::abs(a.z - b.z) * 2 < (a_s.z + b_s.z);
    }

    bool does_aabb_intersects(glm::vec3 a, glm::vec3 a_s, glm::vec3 b, glm::vec3 b_s) {
        return aabb_test_axis(a,a_s, b, b_s);
    }

    AABBReturn aabb_3d_intersects(glm::vec3 a, glm::vec3 d, glm::vec3 a_s, glm::vec3 b, glm::vec3 b_s) {
        glm::vec3 tmp =  { a.x + d.x, a.y, a.z};
        bool x = aabb_test_axis(tmp, a_s, b, b_s);
        tmp =  { a.x, a.y + d.y, a.z};
        bool y = aabb_test_axis(tmp, a_s, b, b_s);
        tmp =  { a.x, a.y, a.z + d.z};
        bool z = aabb_test_axis(tmp, a_s, b, b_s);
        return {x,y,z};
    }

    void print_v4(const char* name, glm::vec4& v) {
        DU_CORE_INFO("{}: {} {} {} {}",name,v.x, v.y,v.z, v.w);
    }
    void print_v3(const char* name, glm::vec3& v) {
        DU_CORE_INFO("{}: {} {} {}",name,v.x, v.y,v.z);
    }

    void print_v2(const char* name, glm::vec2& v) {
        DU_CORE_INFO("{}: {} {}",name,v.x, v.y);
    }


    std::string trim(const std::string& str) {
        auto start = str.begin();
        while (start != str.end() && std::isspace(*start)) {
            ++start;
        }

        auto end = str.end();
        do {
            --end;
        } while (end != start && std::isspace(*end));

        return std::string(start, end + 1);
    }
}


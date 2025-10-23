#include "util.h"

namespace engine {
    float dist_vec3(glm::vec3& a, glm::vec3& b) {
        float x = (b.x - a.x) * (b.x - a.x);
        float y = (b.y - a.y) * (b.y - a.y);
        float z = (b.z - a.z) * (b.z - a.z);
        return std::sqrt(x + y +z);
    }


    float get_magnitude(glm::vec3 a) {
        return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    }

    
    glm::vec3 get_forward(glm::vec3& target, glm::vec3& position) {
        return glm::normalize(target - position);
    }

    glm::vec3 get_up(glm::vec3& up) {
        return glm::normalize(up);
    }

    glm::vec3 get_right(glm::vec3& target, glm::vec3& position, glm::vec3& up) {
        glm::vec3 forward = get_forward(target, position);
        glm::vec3 tmp_up = get_up(up);

        return glm::normalize(glm::cross(forward, tmp_up));
    }


    glm::vec3 vec3_rotate_by_axis_angle(glm::vec3 v, glm::vec3 axis, float angle)
    {
        glm::vec3 result = v;

        // glm::vec3Normalize(axis);
        float length = sqrtf(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
        if (length == 0.0f) length = 1.0f;
        float ilength = 1.0f/length;
        axis.x *= ilength;
        axis.y *= ilength;
        axis.z *= ilength;

        angle /= 2.0f;
        float a = sinf(angle);
        float b = axis.x*a;
        float c = axis.y*a;
        float d = axis.z*a;
        a = cosf(angle);
        glm::vec3 w = { b, c, d };

        // glm::vec3CrossProduct(w, v)
        glm::vec3 wv = { w.y*v.z - w.z*v.y, w.z*v.x - w.x*v.z, w.x*v.y - w.y*v.x };

        // glm::vec3CrossProduct(w, wv)
        glm::vec3 wwv = { w.y*wv.z - w.z*wv.y, w.z*wv.x - w.x*wv.z, w.x*wv.y - w.y*wv.x };

        // glm::vec3Scale(wv, 2*a)
        a *= 2;
        wv.x *= a;
        wv.y *= a;
        wv.z *= a;

        // glm::vec3Scale(wwv, 2)
        wwv.x *= 2;
        wwv.y *= 2;
        wwv.z *= 2;

        result.x += wv.x;
        result.y += wv.y;
        result.z += wv.z;

        result.x += wwv.x;
        result.y += wwv.y;
        result.z += wwv.z;

        return result;
    }


    float vec3_angle(glm::vec3 v1, glm::vec3 v2) {

        float result = 0.0f;

        glm::vec3 cross = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
        float len = sqrtf(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z);
        float dot = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
        result = atan2f(len, dot);

        return result;
    }

    glm::vec3 vec3_neg(glm::vec3 v)
    {
        glm::vec3 result = { -v.x, -v.y, -v.z };

        return result;
    }

    void camera_yaw(CameraComp* camera, glm::vec3 position, float angle) {

        // Rotation axis
        glm::vec3 up = get_up(camera->up);

        // View vector
        glm::vec3 target_position = camera->target - position;

        // Rotate view vector around up axis
        target_position = vec3_rotate_by_axis_angle(target_position, up, angle);

        // Move target relative to position
        camera->target = position + target_position;
    }

    void camera_pitch(CameraComp* camera,glm::vec3 position,  float angle, bool lock) {

        // Up direction
        glm::vec3 up = get_up(camera->up);

        // View vector
        glm::vec3 target_position = camera->target - position;

        if (lock)
        {
            // In these camera modes we clamp the Pitch angle
            // to allow only viewing straight up or down.

            // Clamp view up
            float maxAngleUp = vec3_angle(up, target_position);
            maxAngleUp -= 0.001f; // avoid numerical errors
            if (angle > maxAngleUp) angle = maxAngleUp;

            // Clamp view down
            float maxAngleDown = vec3_angle(vec3_neg(up), target_position);
            maxAngleDown *= -1.0f; // downwards angle is negative
            maxAngleDown += 0.001f; // avoid numerical errors
            if (angle < maxAngleDown) angle = maxAngleDown;
        }

        // Rotation axis
        glm::vec3 right = get_right(camera->target, position,up);

        // Rotate view vector around right axis
        target_position = vec3_rotate_by_axis_angle(target_position, right, angle);

        camera->target = position + target_position;

    }

    void handle_mouse_delta(CameraComp* camera, glm::vec3 position, glm::vec2 delta, bool lock) { 
        camera_yaw  (camera, position, glm::radians(-delta.x));
        camera_pitch(camera, position, glm::radians(-delta.y), lock);
    }

    glm::vec3 AABBReturn::to_glm() {
        return {(float)(!x),(float)(!y),(float)(!z)};
    }

    bool aabb_test_axis(glm::vec3 a, glm::vec3 a_s, glm::vec3 b, glm::vec3 b_s) {
        return 
           std::abs(a.x - b.x) * 2 < (a_s.x + b_s.x) &&
           std::abs(a.y - b.y) * 2 < (a_s.y + b_s.y) &&
           std::abs(a.z - b.z) * 2 < (a_s.z + b_s.z);
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
        EG_CORE_INFO("{}: {} {} {} {}",name,v.x, v.y,v.z, v.w);
    }
    void print_v3(const char* name, glm::vec3& v) {
        EG_CORE_INFO("{}: {} {} {}",name,v.x, v.y,v.z);
    }

    void print_v2(const char* name, glm::vec2& v) {
        EG_CORE_INFO("{}: {} {}",name,v.x, v.y);
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


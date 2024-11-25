#include "util.h"

namespace engine {
    float dist_vec3(Vector3& a, Vector3& b) {
        float x = (b.x - a.x) * (b.x - a.x);
        float y = (b.y - a.y) * (b.y - a.y);
        float z = (b.z - a.z) * (b.z - a.z);
        return std::sqrt(x + y +z);
    }


    void addop_vec3(Vector3* a, Vector3 b) {
        a->x += b.x;
        a->y += b.y;
        a->z += b.z;
    }



    Vector3 normalize_vec3(Vector3 a) {
        float mag = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
        return { a.x / mag, a.y/ mag, a.z/ mag };
    }

    Vector3 vec3_cross_product(Vector3 a, Vector3 b) {

       Vector3 result = { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };

       return result;
    }


    float get_magnitude(Vector3 a) {
        return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    }

    Vector3 add_vec3(Vector3 a, Vector3 b) {
        return { a.x + b.x, a.y + b.y, a.z + b.z };
    }
    Vector3 sub_vec3(Vector3 a, Vector3 b) {
        return { a.x - b.x, a.y - b.y, a.z - b.z };
    }

    Vector3 mul_vec3(Vector3& a, Vector3& b) {
        return { a.x * b.x, a.y * b.y, a.z * b.z };
    }

    Vector3 mul_vec3_f(Vector3 a, float b) {
        return { a.x * b, a.y * b, a.z * b};
    }

    Vector3 get_forward(Vector3& target, Vector3& position) {
        return normalize_vec3(sub_vec3(target, position));
    }

    Vector3 get_up(Vector3& up) {
        return normalize_vec3(up);
    }

    Vector3 get_right(Vector3& target, Vector3& position, Vector3& up) {
        Vector3 forward = get_forward(target, position);
        Vector3 tmp_up = get_up(up);

        return normalize_vec3(vec3_cross_product(forward, tmp_up));
    }


    Vector3 vec3_rotate_by_axis_angle(Vector3 v, Vector3 axis, float angle)
    {
        // Using Euler-Rodrigues Formula
        // Ref.: https://en.wikipedia.org/w/index.php?title=Euler%E2%80%93Rodrigues_formula

        Vector3 result = v;

        // Vector3Normalize(axis);
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
        Vector3 w = { b, c, d };

        // Vector3CrossProduct(w, v)
        Vector3 wv = { w.y*v.z - w.z*v.y, w.z*v.x - w.x*v.z, w.x*v.y - w.y*v.x };

        // Vector3CrossProduct(w, wv)
        Vector3 wwv = { w.y*wv.z - w.z*wv.y, w.z*wv.x - w.x*wv.z, w.x*wv.y - w.y*wv.x };

        // Vector3Scale(wv, 2*a)
        a *= 2;
        wv.x *= a;
        wv.y *= a;
        wv.z *= a;

        // Vector3Scale(wwv, 2)
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


    float vec3_angle(Vector3 v1, Vector3 v2) {

        float result = 0.0f;

        Vector3 cross = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
        float len = sqrtf(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z);
        float dot = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
        result = atan2f(len, dot);

        return result;
    }

    Vector3 vec3_neg(Vector3 v)
    {
        Vector3 result = { -v.x, -v.y, -v.z };

        return result;
    }

    void camera_yaw(Camera* camera, float angle) {

        // Rotation axis
        Vector3 up = get_up(camera->up);

        // View vector
        Vector3 targetPosition = sub_vec3(camera->target, camera->position);

        // Rotate view vector around up axis
        targetPosition = vec3_rotate_by_axis_angle(targetPosition, up, angle);

        // Move target relative to position
        camera->target = add_vec3(camera->position, targetPosition);
    }

    void camera_pitch(Camera* camera, float angle, bool lock) {

        // Up direction
        Vector3 up = get_up(camera->up);

        // View vector
        Vector3 targetPosition = sub_vec3(camera->target, camera->position);

        if (lock)
        {
            // In these camera modes we clamp the Pitch angle
            // to allow only viewing straight up or down.

            // Clamp view up
            float maxAngleUp = vec3_angle(up, targetPosition);
            maxAngleUp -= 0.001f; // avoid numerical errors
            if (angle > maxAngleUp) angle = maxAngleUp;

            // Clamp view down
            float maxAngleDown = vec3_angle(vec3_neg(up), targetPosition);
            maxAngleDown *= -1.0f; // downwards angle is negative
            maxAngleDown += 0.001f; // avoid numerical errors
            if (angle < maxAngleDown) angle = maxAngleDown;
        }

        // Rotation axis
        Vector3 right = get_right(camera->target, camera->position,up);

        // Rotate view vector around right axis
        targetPosition = vec3_rotate_by_axis_angle(targetPosition, right, angle);

        camera->target = add_vec3(camera->position, targetPosition);

    }

    void handle_mouse_delta(Camera* camera, Vector3 delta, bool lock) { 
        camera_yaw(camera, -delta.x * DEG2RAD);
        camera_pitch(camera, -delta.y * DEG2RAD, lock);
    }


    bool aabb_3d_intersects(Vector3 a, Vector3 a_s, Vector3 b, Vector3 b_s) {

        return (
           std::abs(a.x - b.x) * 2 < (a_s.x + b_s.x) &&
           std::abs(a.y - b.y) * 2 < (a_s.y + b_s.y) &&
           std::abs(a.z - b.z) * 2 < (a_s.z + b_s.z)
        );
    }
}

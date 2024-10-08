#include "ops.hpp"


Vector3 operator+(const Vector3 a, const Vector3 b) {
    Vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Vector3 operator-(const Vector3 a, const Vector3 b) {
    Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

Vector3 operator*(const Vector3 a, const Vector3 b) {
    Vector3 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;
}


Vector3 operator/(const Vector3 a, const Vector3 b) {
    Vector3 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    return result;
}

Vector3& operator+=(Vector3& a, const Vector3 b) {
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    a.z = a.z + b.z;
    return a;
}

Vector3& operator-=(Vector3& a, const Vector3 b) {
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    a.z = a.z - b.z;
    return a;
}

Vector3& operator*=(Vector3& a, const Vector3 b) {
    a.x = a.x * b.x;
    a.y = a.y * b.y;
    a.z = a.z * b.z;
    return a;
}

Vector3& operator/=(Vector3& a, const Vector3 b) {
    a.x = a.x / b.x;
    a.y = a.y / b.y;
    a.z = a.z / b.z;
    return a;
}

bool operator==(const Vector3 a, const Vector3 b) {
    return (
            a.x == b.x &&
            a.y == b.y &&
            a.z == b.z
           );
}

std::ostream& operator<<(std::ostream& s, const Vector3& v) {
    auto f = TextFormat("(%f, %f, %f)", v.x, v.y, v.z);
    s<<f;
    return s;
}


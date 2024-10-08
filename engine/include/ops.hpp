#ifndef OPS
#define OPS
#include <iostream>
#include <raylib.h>

Vector3 operator+(const Vector3 a, const Vector3 b);

Vector3 operator-(const Vector3 a, const Vector3 b);

Vector3 operator*(const Vector3 a, const Vector3 b);

Vector3 operator/(const Vector3 a, const Vector3 b);

Vector3& operator+=(Vector3& a, const Vector3 b);

Vector3& operator-=(Vector3& a, const Vector3 b);

Vector3& operator*=(Vector3& a, const Vector3 b);

Vector3& operator/=(Vector3& a, const Vector3 b);

bool operator==(const Vector3 a, const Vector3 b);


template <typename T>
Vector3 operator+(const Vector3 a, const T b) {
    Vector3 result;
    result.x = a.x + b;
    result.y = a.y + b;
    result.z = a.z + b;
    return result;
}

template <typename T>
Vector3 operator-(const Vector3 a, const T b) {
    Vector3 result;
    result.x = a.x - b;
    result.y = a.y - b;
    result.z = a.z - b;
    return result;
}

template <typename T>
Vector3 operator*(const Vector3 a, const T b) {
    Vector3 result;
    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    return result;
}

template <typename T>
Vector3 operator/(const Vector3 a, const T b) {
    Vector3 result;
    result.x = a.x / b;
    result.y = a.y / b;
    result.z = a.z / b;
    return result;
}


std::ostream& operator<<(std::ostream& s, const Vector3& v);
#endif

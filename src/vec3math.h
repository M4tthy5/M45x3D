#pragma once
#include <algorithm>
#include <cmath>

//using  u = int;
typedef int u;

struct vec3
{
    u x;
    u y;
    u z;
    u length = 0;

    explicit vec3(const u x)
    {
        this->x = x;
        this->y = x;
        this->z = x;
        this->length = static_cast<u>(std::sqrt(x * x + y * y + z * z));
    }

    explicit vec3(const u x, const u y, const u z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->length = static_cast<u>(std::sqrt(x * x + y * y + z * z));
    }

    vec3()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0.0;
        this->length = 0;
    }
};

inline vec3 max(const vec3& a, const vec3& b)
{
    return vec3(
        std::max(a.x,b.x),
        std::max(a.y,b.y),
        std::max(a.z,b.z));
}

inline vec3 min(const vec3& a, const vec3& b)
{
    return vec3(
        std::min(a.x,b.x),
        std::min(a.y,b.y),
        std::min(a.z,b.z));
}

inline vec3 operator+(const vec3& a, const vec3& b)
{
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline vec3 operator-(const vec3& a, const vec3& b)
{
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline vec3 operator*(const vec3& a, const vec3& b)
{
    return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline vec3 operator*(const vec3& a, const u b)
{
    const auto s = vec3(b);
    return a * s;
}

inline vec3 operator/(const vec3& a, const vec3& b)
{
    return vec3(a.x / b.x, a.y / b.y, a.z / b.z);
}

inline vec3 operator+=(const vec3& a, const vec3& b)
{
    return a + b;
}

inline vec3 operator-=(const vec3& a, const vec3& b)
{
    return a - b;
}

#pragma once
#include <cmath>
#include <string>

struct Vec2
{
    float x = 0.0f, y = 0.0f;

    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2  operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2  operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2  operator*(float s)       const { return {x * s,   y * s};   }
    Vec2  operator/(float s)       const { return {x / s,   y / s};   }
    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
    Vec2& operator*=(float s)       { x *= s;   y *= s;   return *this; }
    bool  operator==(const Vec2& o) const { return x == o.x && y == o.y; }
    bool  operator!=(const Vec2& o) const { return !(*this == o); }

    float length()    const { return std::sqrt(x * x + y * y); }
    float lengthSq()  const { return x * x + y * y; }
    Vec2  normalized() const { float l = length(); return l > 0 ? *this / l : Vec2{}; }
    float dot(const Vec2& o) const { return x * o.x + y * o.y; }

    std::string toString() const
    {
        return "Vec2(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    static Vec2 zero()  { return {0, 0}; }
    static Vec2 one()   { return {1, 1}; }
    static Vec2 up()    { return {0, -1}; }
    static Vec2 down()  { return {0,  1}; }
    static Vec2 left()  { return {-1, 0}; }
    static Vec2 right() { return { 1, 0}; }
};

inline Vec2 operator*(float s, const Vec2& v) { return v * s; }

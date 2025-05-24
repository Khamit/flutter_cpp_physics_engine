#pragma once

class Vector2 {
public:
    float x, y;

    Vector2(float x = 0, float y = 0);
    void set(float newX, float newY);
    Vector2& operator+=(const Vector2& other);
    Vector2& operator*=(float scalar);
    Vector2 operator/(float scalar) const;
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator*(float scalar) const;
};
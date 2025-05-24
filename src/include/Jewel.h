#pragma once
#include "PhysicsBody.h"  // Assuming Vector2 is defined here

enum class JewelColor {
    Red, Blue, Green, Yellow, Purple, Orange, Count
};

enum class JewelShape {
    Circle, Square, Triangle, Diamond, Star, Heart, Count
};
enum class JewelType { Normal, HorizontalClear, VerticalClear, Bomb, Rainbow };

struct JewelData {
    Vector2 position;
    Vector2 velocity;
    float mass;
    JewelShape shape;
    JewelColor color;
    bool isMatched;

    // Default constructor (new)
    JewelData()
        : position(0, 0), velocity(0, 0), mass(1.0f),
        shape(JewelShape::Circle), color(JewelColor::Red), isMatched(false) {}

    // Main constructor (already existed, now extended)
    JewelData(const Vector2& pos, const Vector2& vel, float m, JewelShape s, JewelColor c, bool matched = false)
        : position(pos), velocity(vel), mass(m), shape(s), color(c), isMatched(matched) {}
};


class Jewel {
public:
    Jewel(const Vector2& pos, const Vector2& vel, float mass, JewelShape s, JewelColor c);

    JewelData& getData();
    const JewelData& getData() const;

private:
    JewelData data;
};

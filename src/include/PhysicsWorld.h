#pragma once

#include <vector>
#include <memory>
#include "Vector2.h"
#include "PhysicsBody.h"
#include "Jewel.h"

class PhysicsWorld {
public:
    PhysicsWorld(float gx, float gy);  // Constructor declaration
    PhysicsBody* createBody(const Vector2& pos, const Vector2& vel, float mass);
    Jewel* createJewel(const Vector2& pos, const Vector2& vel, float mass, JewelShape shape, JewelColor color);

    void update(float dt);
    void setGravity(float gx, float gy);

private:
    Vector2 gravity;
    std::vector<std::unique_ptr<PhysicsBody>> bodies;
    std::vector<std::unique_ptr<Jewel>> jewels;
};

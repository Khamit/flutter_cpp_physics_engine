#include "PhysicsWorld.h"
#include "Jewel.h"

// Constructor
PhysicsWorld::PhysicsWorld(float gx, float gy)
    : gravity(gx, gy) {
    // future logic for PhysicsWorld
}

// createJewel
Jewel* PhysicsWorld::createJewel(const Vector2& pos, const Vector2& vel, float mass, JewelShape shape, JewelColor color) {
    return new Jewel(pos, vel, mass, shape, color);
}

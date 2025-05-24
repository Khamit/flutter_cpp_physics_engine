#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(const Vector2& pos, const Vector2& vel, float m, float friction, bool isDynamic)
    : position(pos), velocity(vel), mass(m), friction(friction), isDynamic(isDynamic), acceleration(0, 0), force(0, 0) {}

void PhysicsBody::applyForce(const Vector2& f) {
    force += f;
}

void PhysicsBody::update(float deltaTime) {
    if (isDynamic) {
        acceleration = force / mass;
        velocity += acceleration * deltaTime;
        velocity *= (1.0f - friction * deltaTime); // ah thats it?
        position += velocity * deltaTime;
        force = Vector2(0, 0);
    }
}

#pragma once
#include "Vector2.h"

class PhysicsBody {
public:
    PhysicsBody(const Vector2& pos, const Vector2& vel, float m, float friction = 0.0f, bool isDynamic = true);

    virtual ~PhysicsBody() = default;

    // Getters
    const Vector2& getPosition() const { return position; }
    const Vector2& getVelocity() const { return velocity; }
    const Vector2& getAcceleration() const { return acceleration; }
    float getMass() const { return mass; }
    float getFriction() const { return friction; }
    bool isDynamicObject() const { return isDynamic; }

    // Setters
    void setPosition(const Vector2& pos) { position = pos; }
    void setVelocity(const Vector2& vel) { velocity = vel; }
    void setAcceleration(const Vector2& acc) { acceleration = acc; }
    void setMass(float m) { mass = m; }
    void setFriction(float fric) { friction = fric; }
    void setDynamic(bool dynamic) { isDynamic = dynamic; }

    // Physics updates
    void applyForce(const Vector2& force);
    void update(float deltaTime);

protected:
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    Vector2 force;
    float mass;
    float friction;
    bool isDynamic;
};

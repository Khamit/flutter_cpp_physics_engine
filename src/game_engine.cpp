#include <cmath>
#include "game_engine.h" // optional

struct Vector2 {
    float x, y;

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    void add(const Vector2& v) {
        x += v.x;
        y += v.y;
    }

    void scale(float scalar) {
        x *= scalar;
        y *= scalar;
    }

    Vector2 scaled(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    void set(float newX, float newY) {
        x = newX;
        y = newY;
    }
};

struct PhysicsBody {
    Vector2 position;
    Vector2 velocity;
    Vector2 gravity;
    float mass;

    PhysicsBody(float posX, float posY, float velX, float velY, float gravX = 0.0f, float gravY = 9.8f, float m = 1.0f)
        : position(posX, posY), velocity(velX, velY), gravity(gravX, gravY), mass(m) {}

    void update(float dt) {
        // Apply gravity to velocity
        Vector2 gravityEffect = gravity.scaled(dt);
        velocity.add(gravityEffect);

        // Apply velocity to position
        Vector2 displacement = velocity.scaled(dt);
        position.add(displacement);
    }
};

extern "C" {

// Create a physics body with position, velocity, gravity, and mass
PhysicsBody* create_body(float posX, float posY, float velX, float velY, float gravX, float gravY, float mass) {
    return new PhysicsBody(posX, posY, velX, velY, gravX, gravY, mass);
}

void update_body(PhysicsBody* body, float dt) {
    body->update(dt);
}

void get_body_position(PhysicsBody* body, float* x, float* y) {
    *x = body->position.x;
    *y = body->position.y;
}

void set_body_velocity(PhysicsBody* body, float vx, float vy) {
    body->velocity.set(vx, vy);
}

void set_body_gravity(PhysicsBody* body, float gx, float gy) {
    body->gravity.set(gx, gy);
}

void destroy_body(PhysicsBody* body) {
    delete body;
}

}

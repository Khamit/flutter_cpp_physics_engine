#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration of the C++ class
struct PhysicsBody;

// C API functions
PhysicsBody* create_body(float posX, float posY, float velX, float velY, float gravX, float gravY, float mass);
void update_body(PhysicsBody* body, float dt);
void get_body_position(PhysicsBody* body, float* x, float* y);
void set_body_velocity(PhysicsBody* body, float vx, float vy);
void set_body_gravity(PhysicsBody* body, float gx, float gy);
void destroy_body(PhysicsBody* body);

#ifdef __cplusplus
}
#endif

#endif // GAME_ENGINE_H

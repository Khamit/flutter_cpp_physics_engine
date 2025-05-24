#ifndef GAME_ENGINE_H // when androidff
#define GAME_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

// A C-compatible struct mirroring JewelData (for FFI)
typedef struct {
    float posX;
    float posY;
    float velX;
    float velY;
    float mass;
    int shape;     // enum as int
    int color;     // enum as int
    int isMatched; // bool as int (0 or 1)
} JewelDataFFI;

// Forward declaration of the C++ struct for PhysicsBody
typedef struct PhysicsBody PhysicsBody;

// Get jewel data at a grid cell (returns C-compatible struct)
__attribute__((visibility("default")))
JewelDataFFI get_jewel_data_ffi(int row, int col);

// Physics API functions
__attribute__((visibility("default")))
PhysicsBody* create_body(float posX, float posY, float velX, float velY, float gravX, float gravY, float mass);

__attribute__((visibility("default")))
void update_body(PhysicsBody* body, float dt);

__attribute__((visibility("default")))
void get_body_position(PhysicsBody* body, float* x, float* y);

__attribute__((visibility("default")))
void set_body_velocity(PhysicsBody* body, float vx, float vy);

__attribute__((visibility("default")))
void set_body_gravity(PhysicsBody* body, float gx, float gy);

__attribute__((visibility("default")))
void destroy_body(PhysicsBody* body);

#ifdef __cplusplus
}
#endif

#endif // GAME_ENGINE_H

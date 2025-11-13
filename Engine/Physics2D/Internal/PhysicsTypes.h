#pragma once
#include "../../Math/Vector2.h"

namespace Engine {
namespace Physics2D {

// Forward declarations
class Rigidbody2D;
class Collider2D;

// Body type - how the object behaves
enum class BodyType {
    Static,      // Doesn't move (walls, ground) - infinite mass
    Kinematic,   // Moves but not affected by forces (moving platforms)
    Dynamic      // Fully simulated (ball, player, boxes)
};

// Collider shape types
enum class ColliderType {
    Circle,
    Box
};

// Physics material - how objects interact
struct PhysicsMaterial2D {
    float friction = 0.3f;         // Surface friction (0 = ice, 1 = rubber)
    float restitution = 0.0f;      // Bounciness (0 = no bounce, 1 = perfect bounce)
    float density = 1.0f;          // Mass per unit area

    PhysicsMaterial2D() = default;
    PhysicsMaterial2D(float f, float r)
        : friction(f), restitution(r) {}
    PhysicsMaterial2D(float f, float r, float d)
        : friction(f), restitution(r), density(d) {}
};

// Collision information passed to user callbacks
struct CollisionInfo {
    Rigidbody2D* bodyA = nullptr;
    Rigidbody2D* bodyB = nullptr;
    Collider2D* colliderA = nullptr;
    Collider2D* colliderB = nullptr;
    Math::Vector2 normal;          // Direction of collision
    Math::Vector2 contactPoint;    // Where they touched
    float penetration = 0.0f;      // How deep is the overlap

    CollisionInfo() = default;
};

// Internal manifold for collision resolution
struct Manifold {
    Rigidbody2D* bodyA = nullptr;
    Rigidbody2D* bodyB = nullptr;
    Collider2D* colliderA = nullptr;
    Collider2D* colliderB = nullptr;
    Math::Vector2 normal;
    float penetration = 0.0f;
    float restitution = 0.0f;      // Combined restitution
    float friction = 0.0f;         // Combined friction

    Manifold() = default;
};

// Physics constants
namespace PhysicsConstants {
    const float FIXED_TIMESTEP = 1.0f / 60.0f;  // 60 Hz physics update
    const float SLOP = 0.01f;                    // Penetration allowance
    const float BAUMGARTE = 0.2f;               // Position correction percent
    const float VELOCITY_EPSILON = 0.0001f;     // Minimum velocity to consider
}

}}

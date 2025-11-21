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

// Forward declare material (defined in PhysicsMaterial.h)
class PhysicsMaterial2D;

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

    // Accumulated impulses for warm starting (Erin Catto technique)
    float accumulatedNormalImpulse = 0.0f;
    float accumulatedTangentImpulse = 0.0f;

    Manifold() = default;
};

// Physics constants
namespace PhysicsConstants {
    const float FIXED_TIMESTEP = 1.0f / 60.0f;  // 60 Hz physics update
    const float SLOP = 0.005f;                   // Penetration allowance (1-2 pixels)
    const float BAUMGARTE = 0.2f;                // Position correction percent (0.2 = 20% per frame)
    const float VELOCITY_EPSILON = 0.0001f;      // Minimum velocity to consider
    const int VELOCITY_ITERATIONS = 6;           // Velocity solver iterations (6-8 recommended)
    const int POSITION_ITERATIONS = 2;           // Position solver iterations (2-3 recommended)
    const float MAX_LINEAR_CORRECTION = 5.0f;    // Max position correction per frame (pixels)
}

}}

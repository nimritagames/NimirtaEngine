#pragma once

namespace Engine {
namespace Physics2D {

// Simple 2D material presets for common surfaces
// Under the hood: friction and restitution values
class PhysicsMaterial2D {
public:
    float friction;
    float restitution;
    float density;

    // Default constructor
    PhysicsMaterial2D()
        : friction(0.3f), restitution(0.0f), density(1.0f) {}

    // Custom material
    PhysicsMaterial2D(float friction, float restitution, float density = 1.0f)
        : friction(friction), restitution(restitution), density(density) {}

    // ========== EASY PRESETS ==========
    // Users can just write: PhysicsMaterial2D::Bouncy()

    static PhysicsMaterial2D Default() {
        return PhysicsMaterial2D(0.3f, 0.0f, 1.0f);
    }

    static PhysicsMaterial2D Bouncy() {
        return PhysicsMaterial2D(0.3f, 0.8f, 1.0f);  // High bounce
    }

    static PhysicsMaterial2D SuperBouncy() {
        return PhysicsMaterial2D(0.2f, 0.95f, 0.5f);  // Super bounce
    }

    static PhysicsMaterial2D Ice() {
        return PhysicsMaterial2D(0.03f, 0.1f, 0.9f);  // Very slippery
    }

    static PhysicsMaterial2D Rubber() {
        return PhysicsMaterial2D(0.8f, 0.7f, 1.2f);  // High friction + bounce
    }

    static PhysicsMaterial2D Wood() {
        return PhysicsMaterial2D(0.4f, 0.3f, 0.6f);
    }

    static PhysicsMaterial2D Metal() {
        return PhysicsMaterial2D(0.2f, 0.4f, 7.8f);  // Dense
    }

    static PhysicsMaterial2D Stone() {
        return PhysicsMaterial2D(0.6f, 0.1f, 2.5f);  // Dense, low bounce
    }
};

}}

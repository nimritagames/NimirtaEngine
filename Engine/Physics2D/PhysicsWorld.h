#pragma once
#include "../Math/Vector2.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "PhysicsMaterial.h"
#include "Internal/PhysicsTypes.h"
#include "Internal/CollisionDetection.h"
#include "Internal/ImpulseResolver.h"
#include <vector>
#include <functional>

namespace Engine {
namespace Physics2D {

// Collision callback types (for user code)
using CollisionCallback = std::function<void(const CollisionInfo&)>;

// Main 2D physics world - EASY API for users!
class PhysicsWorld2D {
private:
    std::vector<Rigidbody2D*> rigidbodies;
    Math::Vector2 gravity;
    float accumulator;  // For fixed timestep (expert advice!)
    bool debugDrawEnabled;

    // Collision callbacks
    CollisionCallback onCollisionEnter;
    CollisionCallback onCollisionStay;

public:
    // Constructor - smart defaults!
    PhysicsWorld2D()
        : gravity(0, -9.8f),  // Default Earth gravity
          accumulator(0.0f),
          debugDrawEnabled(false) {}

    ~PhysicsWorld2D() {
        // Clean up all rigidbodies
        for (auto rb : rigidbodies) {
            delete rb;
        }
        rigidbodies.clear();
    }

    // ========== EASY API FOR USERS ==========

    // Create a circle rigidbody - super easy!
    Rigidbody2D* createCircle(float x, float y, float radius, const PhysicsMaterial2D& material = PhysicsMaterial2D::Default()) {
        Rigidbody2D* rb = new Rigidbody2D();
        rb->setPosition(x, y);

        CircleCollider2D* collider = new CircleCollider2D(radius);
        collider->setMaterial(material);
        rb->attachCollider(collider);

        rigidbodies.push_back(rb);
        return rb;
    }

    // Create a box rigidbody - super easy!
    Rigidbody2D* createBox(float x, float y, float width, float height, const PhysicsMaterial2D& material = PhysicsMaterial2D::Default()) {
        Rigidbody2D* rb = new Rigidbody2D();
        rb->setPosition(x, y);

        BoxCollider2D* collider = new BoxCollider2D(width, height);
        collider->setMaterial(material);
        rb->attachCollider(collider);

        rigidbodies.push_back(rb);
        return rb;
    }

    // Create empty rigidbody (for custom colliders)
    Rigidbody2D* createRigidbody() {
        Rigidbody2D* rb = new Rigidbody2D();
        rigidbodies.push_back(rb);
        return rb;
    }

    // Destroy a rigidbody
    void destroyRigidbody(Rigidbody2D* rb) {
        auto it = std::find(rigidbodies.begin(), rigidbodies.end(), rb);
        if (it != rigidbodies.end()) {
            rigidbodies.erase(it);
            delete rb;
        }
    }

    // Set gravity (easy!)
    void setGravity(const Math::Vector2& g) { gravity = g; }
    void setGravity(float x, float y) { gravity = Math::Vector2(x, y); }
    Math::Vector2 getGravity() const { return gravity; }

    // Main update - uses fixed timestep internally (expert advice!)
    void update(float deltaTime) {
        const float fixedDT = PhysicsConstants::FIXED_TIMESTEP;

        // Accumulator pattern (expert advice from Glenn Fiedler!)
        accumulator += deltaTime;

        // Clamp to prevent spiral of death (expert advice!)
        const float maxAccumulator = fixedDT * 5.0f;  // Max 5 steps per frame
        if (accumulator > maxAccumulator) {
            accumulator = maxAccumulator;
        }

        // Consume accumulator in fixed steps
        while (accumulator >= fixedDT) {
            step(fixedDT);
            accumulator -= fixedDT;
        }

        // Note: For interpolation, use:
        // float alpha = accumulator / fixedDT;
        // renderPos = rb->getInterpolatedPosition(alpha);
    }

    // Collision callbacks (easy to use!)
    void setCollisionEnterCallback(CollisionCallback callback) {
        onCollisionEnter = callback;
    }

    void setCollisionStayCallback(CollisionCallback callback) {
        onCollisionStay = callback;
    }

    // Debug drawing
    void setDebugDraw(bool enabled) { debugDrawEnabled = enabled; }
    bool isDebugDrawEnabled() const { return debugDrawEnabled; }

    void debugDraw(sf::RenderWindow* window) {
        if (!debugDrawEnabled) return;

        for (auto rb : rigidbodies) {
            for (auto collider : rb->getColliders()) {
                collider->debugDraw(window);
            }
        }
    }

    // Get all rigidbodies (for advanced users)
    const std::vector<Rigidbody2D*>& getRigidbodies() const {
        return rigidbodies;
    }

    // Raycast (future feature - placeholder)
    // bool raycast(const Math::Vector2& origin, const Math::Vector2& direction, float maxDistance, RaycastHit& hit);

private:
    // Internal physics step (fixed timestep)
    void step(float dt) {
        // 1. Integrate forces and velocities (dynamics)
        for (auto rb : rigidbodies) {
            rb->integrate(dt, gravity);
        }

        // 2. Detect collisions (broad phase would go here - future optimization)
        std::vector<Manifold> manifolds;
        detectCollisions(manifolds);

        // 3. Resolve collisions (impulse + positional correction)
        for (auto& manifold : manifolds) {
            ImpulseResolver2D::resolve(manifold);

            // Fire collision callbacks
            if (onCollisionEnter) {
                CollisionInfo info;
                info.bodyA = manifold.bodyA;
                info.bodyB = manifold.bodyB;
                info.colliderA = manifold.colliderA;
                info.colliderB = manifold.colliderB;
                info.normal = manifold.normal;
                info.penetration = manifold.penetration;
                onCollisionEnter(info);
            }
        }
    }

    // Collision detection (brute force for now - O(n²))
    void detectCollisions(std::vector<Manifold>& manifolds) {
        // Check all pairs
        for (size_t i = 0; i < rigidbodies.size(); i++) {
            for (size_t j = i + 1; j < rigidbodies.size(); j++) {
                Rigidbody2D* rbA = rigidbodies[i];
                Rigidbody2D* rbB = rigidbodies[j];

                // Skip if both are static
                if (rbA->isStatic() && rbB->isStatic()) continue;

                // Check all collider pairs
                for (auto colliderA : rbA->getColliders()) {
                    for (auto colliderB : rbB->getColliders()) {
                        Manifold manifold;
                        if (CollisionDetection2D::detect(colliderA, colliderB, manifold)) {
                            manifolds.push_back(manifold);
                        }
                    }
                }
            }
        }
    }
};

}}

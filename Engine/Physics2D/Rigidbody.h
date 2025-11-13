#pragma once
#include "../Math/Vector2.h"
#include "Collider.h"
#include "Internal/PhysicsTypes.h"
#include <vector>

namespace Engine {
namespace Physics2D {

class Rigidbody2D {
private:
    // Transform
    Math::Vector2 position;
    Math::Vector2 previousPosition;  // For interpolation
    float rotation;
    float previousRotation;

    // Dynamics
    Math::Vector2 velocity;
    float angularVelocity;
    Math::Vector2 netForce;      // Running total (expert advice!)
    float netTorque;

    // Properties
    float mass;
    float inverseMass;           // Store 1/mass (expert advice!)
    float inertia;
    float inverseInertia;
    BodyType bodyType;

    // Physics properties
    float linearDamping;         // Air resistance
    float angularDamping;
    float gravityScale;          // 1.0 = normal, 0.0 = no gravity
    bool fixedRotation;          // Don't rotate

    // Colliders attached to this body
    std::vector<Collider2D*> colliders;

    // Internal state
    bool isAwake;
    void* userData;              // User can attach custom data

public:
    // Constructor - easy API!
    Rigidbody2D()
        : position(0, 0), previousPosition(0, 0),
          rotation(0), previousRotation(0),
          velocity(0, 0), angularVelocity(0),
          netForce(0, 0), netTorque(0),
          mass(1.0f), inverseMass(1.0f),
          inertia(1.0f), inverseInertia(1.0f),
          bodyType(BodyType::Dynamic),
          linearDamping(0.0f), angularDamping(0.0f),
          gravityScale(1.0f), fixedRotation(false),
          isAwake(true), userData(nullptr) {}

    ~Rigidbody2D() {
        // Clean up colliders
        for (auto collider : colliders) {
            delete collider;
        }
        colliders.clear();
    }

    // ========== EASY API FOR USERS ==========

    // Transform
    Math::Vector2 getPosition() const { return position; }
    void setPosition(const Math::Vector2& pos) {
        position = pos;
        previousPosition = pos;
    }
    void setPosition(float x, float y) { setPosition(Math::Vector2(x, y)); }

    float getRotation() const { return rotation; }
    void setRotation(float angle) {
        rotation = angle;
        previousRotation = angle;
    }

    // Velocity
    Math::Vector2 getVelocity() const { return velocity; }
    void setVelocity(const Math::Vector2& vel) { velocity = vel; }
    void setVelocity(float x, float y) { velocity = Math::Vector2(x, y); }

    float getAngularVelocity() const { return angularVelocity; }
    void setAngularVelocity(float vel) { angularVelocity = vel; }

    // Apply forces (easy!)
    void applyForce(const Math::Vector2& force) {
        if (bodyType != BodyType::Dynamic) return;
        netForce += force;  // Running total (expert advice!)
    }

    void applyImpulse(const Math::Vector2& impulse) {
        if (bodyType != BodyType::Dynamic) return;
        velocity += impulse * inverseMass;  // Instant velocity change
    }

    void applyTorque(float torque) {
        if (bodyType != BodyType::Dynamic || fixedRotation) return;
        netTorque += torque;
    }

    // Mass
    float getMass() const { return mass; }
    void setMass(float m) {
        if (m <= 0.0f) m = 1.0f;
        mass = m;
        inverseMass = 1.0f / mass;  // Store inverse (expert advice!)
        updateInertia();
    }

    float getInverseMass() const { return inverseMass; }
    float getInverseInertia() const { return inverseInertia; }

    // Body type (easy helpers!)
    BodyType getBodyType() const { return bodyType; }
    void setBodyType(BodyType type) {
        bodyType = type;
        if (type == BodyType::Static) {
            inverseMass = 0.0f;
            inverseInertia = 0.0f;
            velocity = Math::Vector2(0, 0);
            angularVelocity = 0.0f;
        } else if (type == BodyType::Dynamic) {
            inverseMass = 1.0f / mass;
            updateInertia();
        }
    }

    void makeStatic() { setBodyType(BodyType::Static); }
    void makeDynamic() { setBodyType(BodyType::Dynamic); }
    void makeKinematic() { setBodyType(BodyType::Kinematic); }

    bool isStatic() const { return bodyType == BodyType::Static; }
    bool isDynamic() const { return bodyType == BodyType::Dynamic; }
    bool isKinematic() const { return bodyType == BodyType::Kinematic; }

    // Gravity
    float getGravityScale() const { return gravityScale; }
    void setGravityScale(float scale) { gravityScale = scale; }
    void disableGravity() { gravityScale = 0.0f; }
    void enableGravity() { gravityScale = 1.0f; }

    // Rotation lock
    void setFixedRotation(bool fixed) { fixedRotation = fixed; }
    bool isRotationFixed() const { return fixedRotation; }

    // Damping (air resistance)
    void setLinearDamping(float damping) { linearDamping = damping; }
    void setAngularDamping(float damping) { angularDamping = damping; }

    // User data
    void setUserData(void* data) { userData = data; }
    void* getUserData() const { return userData; }

    // Colliders (easy!)
    void attachCollider(Collider2D* collider) {
        collider->setOwner(this);
        colliders.push_back(collider);
        updateInertia();
    }

    const std::vector<Collider2D*>& getColliders() const { return colliders; }

    // ========== INTERNAL - Used by PhysicsWorld ==========

    // Semi-implicit Euler integration (expert advice!)
    void integrate(float dt, const Math::Vector2& gravity) {
        if (bodyType != BodyType::Dynamic) return;

        // Store previous state for interpolation
        previousPosition = position;
        previousRotation = rotation;

        // Apply gravity
        netForce += gravity * gravityScale * mass;

        // Semi-implicit Euler: update velocity FIRST (expert advice!)
        velocity += netForce * inverseMass * dt;
        angularVelocity += netTorque * inverseInertia * dt;

        // Apply damping
        velocity *= (1.0f / (1.0f + dt * linearDamping));
        angularVelocity *= (1.0f / (1.0f + dt * angularDamping));

        // Then update position using NEW velocity
        position += velocity * dt;
        if (!fixedRotation) {
            rotation += angularVelocity * dt;
        }

        // Clear forces (automatic cleanup - expert advice!)
        netForce = Math::Vector2(0, 0);
        netTorque = 0.0f;
    }

    // Interpolation for smooth rendering
    Math::Vector2 getInterpolatedPosition(float alpha) const {
        return previousPosition * (1.0f - alpha) + position * alpha;
    }

    float getInterpolatedRotation(float alpha) const {
        return previousRotation * (1.0f - alpha) + rotation * alpha;
    }

    // Direct position modification (for collision correction)
    void translatePosition(const Math::Vector2& delta) {
        position += delta;
    }

private:
    // Calculate moment of inertia from colliders
    void updateInertia() {
        if (bodyType == BodyType::Static) {
            inverseInertia = 0.0f;
            return;
        }

        inertia = 0.0f;

        // Calculate inertia from attached colliders
        for (auto collider : colliders) {
            if (collider->getType() == ColliderType::Circle) {
                CircleCollider2D* circle = static_cast<CircleCollider2D*>(collider);
                float r = circle->getRadius();
                inertia += 0.5f * mass * r * r;  // I = 0.5 * m * r^2 for circle
            } else if (collider->getType() == ColliderType::Box) {
                BoxCollider2D* box = static_cast<BoxCollider2D*>(collider);
                float w = box->getWidth();
                float h = box->getHeight();
                inertia += mass * (w * w + h * h) / 12.0f;  // I = m * (w^2 + h^2) / 12 for box
            }
        }

        if (inertia > 0.0f) {
            inverseInertia = 1.0f / inertia;
        } else {
            inertia = 1.0f;
            inverseInertia = 1.0f;
        }
    }
};

// Implement Collider methods that need Rigidbody
inline Math::Vector2 CircleCollider2D::getWorldPosition() const {
    if (owner) {
        return owner->getPosition() + offset;
    }
    return offset;
}

inline Math::Vector2 BoxCollider2D::getWorldPosition() const {
    if (owner) {
        return owner->getPosition() + offset;
    }
    return offset;
}

}}

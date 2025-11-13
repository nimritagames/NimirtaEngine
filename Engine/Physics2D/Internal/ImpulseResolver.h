#pragma once
#include "../../Math/Vector2.h"
#include "../Rigidbody.h"
#include "PhysicsTypes.h"
#include <cmath>
#include <algorithm>

namespace Engine {
namespace Physics2D {

// Impulse-based collision resolution
// Based on expert recommendations from Erin Catto and physics tutorials
class ImpulseResolver2D {
public:
    // Resolve collision using impulse method
    static void resolve(Manifold& manifold) {
        Rigidbody2D* bodyA = manifold.bodyA;
        Rigidbody2D* bodyB = manifold.bodyB;

        if (!bodyA || !bodyB) return;

        // Skip if both are static or kinematic
        if (bodyA->getInverseMass() == 0 && bodyB->getInverseMass() == 0) {
            return;
        }

        // Calculate relative velocity
        Math::Vector2 velA = bodyA->getVelocity();
        Math::Vector2 velB = bodyB->getVelocity();
        Math::Vector2 relativeVel = velB - velA;

        // Velocity along the collision normal
        float velAlongNormal = relativeVel.dot(manifold.normal);

        // Only resolve if objects are approaching (expert advice!)
        if (velAlongNormal > 0) {
            return;  // Objects are separating, don't resolve
        }

        // Calculate impulse magnitude
        // j = -(1 + e) * velAlongNormal / (1/massA + 1/massB)
        float e = manifold.restitution;
        float j = -(1.0f + e) * velAlongNormal;
        j /= (bodyA->getInverseMass() + bodyB->getInverseMass());

        // Apply impulse
        Math::Vector2 impulse = manifold.normal * j;

        if (bodyA->isDynamic()) {
            bodyA->applyImpulse(impulse * -1.0f);
        }

        if (bodyB->isDynamic()) {
            bodyB->applyImpulse(impulse);
        }

        // Friction impulse (tangent to normal)
        applyFriction(manifold, relativeVel, j);

        // Positional correction (Baumgarte stabilization - expert advice!)
        correctPosition(manifold);
    }

private:
    // Apply friction along the tangent
    static void applyFriction(Manifold& manifold, const Math::Vector2& relativeVel, float normalImpulse) {
        Rigidbody2D* bodyA = manifold.bodyA;
        Rigidbody2D* bodyB = manifold.bodyB;

        if (manifold.friction <= 0.0f) return;

        // Calculate tangent (perpendicular to normal)
        Math::Vector2 tangent = relativeVel - (manifold.normal * relativeVel.dot(manifold.normal));

        float tangentLength = tangent.magnitude();
        if (tangentLength < PhysicsConstants::VELOCITY_EPSILON) {
            return;  // No tangential velocity
        }

        tangent = tangent.normalized();

        // Calculate friction impulse magnitude
        float velAlongTangent = relativeVel.dot(tangent);
        float frictionImpulse = -velAlongTangent;
        frictionImpulse /= (bodyA->getInverseMass() + bodyB->getInverseMass());

        // Coulomb's law: friction <= mu * normal force
        float mu = manifold.friction;
        frictionImpulse = std::max(-std::abs(normalImpulse) * mu,
                                     std::min(frictionImpulse, std::abs(normalImpulse) * mu));

        // Apply friction impulse
        Math::Vector2 frictionVec = tangent * frictionImpulse;

        if (bodyA->isDynamic()) {
            bodyA->applyImpulse(frictionVec * -1.0f);
        }

        if (bodyB->isDynamic()) {
            bodyB->applyImpulse(frictionVec);
        }
    }

    // Positional correction to prevent sinking (Baumgarte stabilization)
    // Expert advice: use slop and correction percentage
    static void correctPosition(Manifold& manifold) {
        Rigidbody2D* bodyA = manifold.bodyA;
        Rigidbody2D* bodyB = manifold.bodyB;

        // Only correct if penetration exceeds slop threshold (expert advice!)
        if (manifold.penetration <= PhysicsConstants::SLOP) {
            return;
        }

        // Calculate correction amount
        // Use Baumgarte stabilization with 20% correction (expert advice!)
        float correctionAmount = (manifold.penetration - PhysicsConstants::SLOP) * PhysicsConstants::BAUMGARTE;
        correctionAmount /= (bodyA->getInverseMass() + bodyB->getInverseMass());

        Math::Vector2 correction = manifold.normal * correctionAmount;

        // Apply position correction proportional to inverse mass
        if (bodyA->isDynamic()) {
            bodyA->translatePosition(correction * -bodyA->getInverseMass());
        }

        if (bodyB->isDynamic()) {
            bodyB->translatePosition(correction * bodyB->getInverseMass());
        }
    }
};

}}

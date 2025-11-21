#pragma once
#include "../../Math/Vector2.h"
#include "../Rigidbody.h"
#include "PhysicsTypes.h"
#include <cmath>
#include <algorithm>

namespace Engine {
namespace Physics2D {

// Sequential Impulse-based collision resolution
// Based on Erin Catto's GDC tutorials and Box2D implementation
class ImpulseResolver2D {
public:
    // Resolve collision using sequential impulses with iterations
    static void resolve(Manifold& manifold) {
        Rigidbody2D* bodyA = manifold.bodyA;
        Rigidbody2D* bodyB = manifold.bodyB;

        if (!bodyA || !bodyB) return;

        float totalInverseMass = bodyA->getInverseMass() + bodyB->getInverseMass();
        if (totalInverseMass == 0.0f) return; // Both static

        // VELOCITY ITERATIONS (6-8 iterations for convergence)
        for (int i = 0; i < PhysicsConstants::VELOCITY_ITERATIONS; ++i) {
            // Recalculate relative velocity each iteration
            Math::Vector2 rv = bodyB->getVelocity() - bodyA->getVelocity();
            float velAlongNormal = rv.dot(manifold.normal);

            // Stop if separating fast enough
            if (velAlongNormal > -PhysicsConstants::VELOCITY_EPSILON) {
                break;
            }

            // Calculate impulse scalar (with restitution only on first iteration)
            float e = (i == 0) ? manifold.restitution : 0.0f;
            float jn = -(1.0f + e) * velAlongNormal / totalInverseMass;

            // WARM STARTING: Accumulate impulse and clamp to prevent negative (pull)
            float newImpulse = std::max(manifold.accumulatedNormalImpulse + jn, 0.0f);
            float applyImpulse = newImpulse - manifold.accumulatedNormalImpulse;
            manifold.accumulatedNormalImpulse = newImpulse;

            // Apply impulse
            Math::Vector2 impulse = manifold.normal * applyImpulse;

            if (bodyA->isDynamic()) {
                bodyA->setVelocity(bodyA->getVelocity() - impulse * bodyA->getInverseMass());
            }

            if (bodyB->isDynamic()) {
                bodyB->setVelocity(bodyB->getVelocity() + impulse * bodyB->getInverseMass());
            }
        }

        // POSITION ITERATIONS (2-3 iterations for overlap correction)
        for (int i = 0; i < PhysicsConstants::POSITION_ITERATIONS; ++i) {
            if (manifold.penetration <= PhysicsConstants::SLOP) {
                break;
            }

            // Calculate correction (Baumgarte stabilization)
            float correction = (manifold.penetration - PhysicsConstants::SLOP) * PhysicsConstants::BAUMGARTE;

            // Clamp correction to prevent over-correction
            correction = std::min(correction, PhysicsConstants::MAX_LINEAR_CORRECTION);

            float correctionPerMass = correction / totalInverseMass;
            Math::Vector2 correctionVec = manifold.normal * correctionPerMass;

            // Apply position correction
            if (bodyA->isDynamic()) {
                bodyA->translatePosition(correctionVec * -bodyA->getInverseMass());
            }

            if (bodyB->isDynamic()) {
                bodyB->translatePosition(correctionVec * bodyB->getInverseMass());
            }

            // Update penetration for next iteration
            manifold.penetration -= correction;
        }
    }

    // Warm start - apply cached impulses from previous frame
    static void warmStart(Manifold& manifold) {
        Rigidbody2D* bodyA = manifold.bodyA;
        Rigidbody2D* bodyB = manifold.bodyB;

        if (!bodyA || !bodyB) return;
        if (manifold.accumulatedNormalImpulse == 0.0f) return;

        // Apply fraction of cached impulse (not full amount to prevent overshooting)
        Math::Vector2 impulse = manifold.normal * (manifold.accumulatedNormalImpulse * 0.8f);

        if (bodyA->isDynamic()) {
            bodyA->setVelocity(bodyA->getVelocity() - impulse * bodyA->getInverseMass());
        }

        if (bodyB->isDynamic()) {
            bodyB->setVelocity(bodyB->getVelocity() + impulse * bodyB->getInverseMass());
        }
    }
};

}}

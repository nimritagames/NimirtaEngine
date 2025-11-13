#pragma once
#include "../../Math/Vector2.h"
#include "../Collider.h"
#include "PhysicsTypes.h"
#include <cmath>

namespace Engine {
namespace Physics2D {

// Collision detection algorithms
// These are the "under the hood" implementations
class CollisionDetection2D {
public:
    // Main entry point - detects collision between any two colliders
    static bool detect(Collider2D* a, Collider2D* b, Manifold& manifold) {
        ColliderType typeA = a->getType();
        ColliderType typeB = b->getType();

        // Dispatch to specific algorithm
        if (typeA == ColliderType::Circle && typeB == ColliderType::Circle) {
            return circleVsCircle(
                static_cast<CircleCollider2D*>(a),
                static_cast<CircleCollider2D*>(b),
                manifold
            );
        }
        else if (typeA == ColliderType::Box && typeB == ColliderType::Box) {
            return boxVsBox(
                static_cast<BoxCollider2D*>(a),
                static_cast<BoxCollider2D*>(b),
                manifold
            );
        }
        else if (typeA == ColliderType::Circle && typeB == ColliderType::Box) {
            return circleVsBox(
                static_cast<CircleCollider2D*>(a),
                static_cast<BoxCollider2D*>(b),
                manifold
            );
        }
        else if (typeA == ColliderType::Box && typeB == ColliderType::Circle) {
            // Flip the manifold
            bool result = circleVsBox(
                static_cast<CircleCollider2D*>(b),
                static_cast<BoxCollider2D*>(a),
                manifold
            );
            if (result) {
                // Swap A and B in manifold
                std::swap(manifold.bodyA, manifold.bodyB);
                std::swap(manifold.colliderA, manifold.colliderB);
                manifold.normal = manifold.normal * -1.0f;  // Flip normal
            }
            return result;
        }

        return false;
    }

private:
    // Circle vs Circle collision (simplest and fastest)
    static bool circleVsCircle(CircleCollider2D* a, CircleCollider2D* b, Manifold& manifold) {
        Math::Vector2 centerA = a->getWorldPosition();
        Math::Vector2 centerB = b->getWorldPosition();

        float radiusA = a->getRadius();
        float radiusB = b->getRadius();

        // Calculate distance between centers
        Math::Vector2 diff = centerB - centerA;
        float distanceSq = diff.x * diff.x + diff.y * diff.y;
        float radiusSum = radiusA + radiusB;

        // No collision if distance > sum of radii
        if (distanceSq >= radiusSum * radiusSum) {
            return false;
        }

        // Collision detected!
        float distance = std::sqrt(distanceSq);

        manifold.bodyA = a->getOwner();
        manifold.bodyB = b->getOwner();
        manifold.colliderA = a;
        manifold.colliderB = b;
        manifold.penetration = radiusSum - distance;

        // Normal points from A to B
        if (distance > 0.0f) {
            manifold.normal = diff * (1.0f / distance);  // Normalize
        } else {
            // Circles are exactly on top of each other - use arbitrary normal
            manifold.normal = Math::Vector2(0, 1);
        }

        // Combine materials (use minimum for restitution/friction - expert advice!)
        PhysicsMaterial2D matA = a->getMaterial();
        PhysicsMaterial2D matB = b->getMaterial();
        manifold.restitution = std::min(matA.restitution, matB.restitution);
        manifold.friction = std::min(matA.friction, matB.friction);

        return true;
    }

    // Box vs Box collision (AABB using SAT - Separating Axis Theorem)
    static bool boxVsBox(BoxCollider2D* a, BoxCollider2D* b, Manifold& manifold) {
        Math::Vector2 minA = a->getMin();
        Math::Vector2 maxA = a->getMax();
        Math::Vector2 minB = b->getMin();
        Math::Vector2 maxB = b->getMax();

        // SAT - if separated on any axis, no collision
        if (maxA.x < minB.x || minA.x > maxB.x) return false;
        if (maxA.y < minB.y || minA.y > maxB.y) return false;

        // Collision detected! Calculate penetration on each axis
        float overlapX = std::min(maxA.x - minB.x, maxB.x - minA.x);
        float overlapY = std::min(maxA.y - minB.y, maxB.y - minA.y);

        // Use axis with smallest overlap as collision normal
        manifold.bodyA = a->getOwner();
        manifold.bodyB = b->getOwner();
        manifold.colliderA = a;
        manifold.colliderB = b;

        if (overlapX < overlapY) {
            // Collision on X axis
            manifold.penetration = overlapX;
            if (a->getWorldPosition().x < b->getWorldPosition().x) {
                manifold.normal = Math::Vector2(1, 0);  // A is left of B
            } else {
                manifold.normal = Math::Vector2(-1, 0);  // A is right of B
            }
        } else {
            // Collision on Y axis
            manifold.penetration = overlapY;
            if (a->getWorldPosition().y < b->getWorldPosition().y) {
                manifold.normal = Math::Vector2(0, 1);  // A is above B
            } else {
                manifold.normal = Math::Vector2(0, -1);  // A is below B
            }
        }

        // Combine materials
        PhysicsMaterial2D matA = a->getMaterial();
        PhysicsMaterial2D matB = b->getMaterial();
        manifold.restitution = std::min(matA.restitution, matB.restitution);
        manifold.friction = std::min(matA.friction, matB.friction);

        return true;
    }

    // Circle vs Box collision (closest point method)
    static bool circleVsBox(CircleCollider2D* circle, BoxCollider2D* box, Manifold& manifold) {
        Math::Vector2 circleCenter = circle->getWorldPosition();
        float radius = circle->getRadius();

        Math::Vector2 boxMin = box->getMin();
        Math::Vector2 boxMax = box->getMax();

        // Find closest point on box to circle center
        float closestX = Math::Vector2::clamp(circleCenter.x, boxMin.x, boxMax.x);
        float closestY = Math::Vector2::clamp(circleCenter.y, boxMin.y, boxMax.y);
        Math::Vector2 closest(closestX, closestY);

        // Calculate distance from circle center to closest point
        Math::Vector2 diff = circleCenter - closest;
        float distanceSq = diff.x * diff.x + diff.y * diff.y;

        // No collision if distance > radius
        if (distanceSq >= radius * radius) {
            return false;
        }

        // Collision detected!
        float distance = std::sqrt(distanceSq);

        manifold.bodyA = circle->getOwner();
        manifold.bodyB = box->getOwner();
        manifold.colliderA = circle;
        manifold.colliderB = box;
        manifold.penetration = radius - distance;

        // Normal points from closest point to circle center
        if (distance > 0.0f) {
            manifold.normal = diff * (1.0f / distance);  // Normalize
        } else {
            // Circle center is inside box - find best separating axis
            Math::Vector2 boxCenter = box->getWorldPosition();
            Math::Vector2 toCenter = circleCenter - boxCenter;

            // Use axis with largest separation
            if (std::abs(toCenter.x) > std::abs(toCenter.y)) {
                manifold.normal = Math::Vector2(toCenter.x > 0 ? 1.0f : -1.0f, 0);
            } else {
                manifold.normal = Math::Vector2(0, toCenter.y > 0 ? 1.0f : -1.0f);
            }
        }

        // Combine materials
        PhysicsMaterial2D matCircle = circle->getMaterial();
        PhysicsMaterial2D matBox = box->getMaterial();
        manifold.restitution = std::min(matCircle.restitution, matBox.restitution);
        manifold.friction = std::min(matCircle.friction, matBox.friction);

        return true;
    }
};

}}

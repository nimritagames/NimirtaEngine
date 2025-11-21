#pragma once
#include "../Math/Vector2.h"
#include "PhysicsMaterial.h"
#include "Internal/PhysicsTypes.h"
#include <SFML/Graphics.hpp>

namespace Engine {
namespace Physics2D {

// Forward declaration
class Rigidbody2D;

// Base 2D collider class
class Collider2D {
protected:
    ColliderType type;
    Rigidbody2D* owner;
    PhysicsMaterial2D material;
    Math::Vector2 offset;  // Offset from rigidbody position
    bool isTrigger;        // Trigger = no collision response, just callbacks

public:
    Collider2D(ColliderType type)
        : type(type), owner(nullptr), offset(0, 0), isTrigger(false) {}

    virtual ~Collider2D() = default;

    // Getters
    ColliderType getType() const { return type; }
    Rigidbody2D* getOwner() const { return owner; }
    PhysicsMaterial2D getMaterial() const { return material; }
    Math::Vector2 getOffset() const { return offset; }
    bool getIsTrigger() const { return isTrigger; }

    // Setters (easy API)
    void setMaterial(const PhysicsMaterial2D& mat) { material = mat; }
    void setOffset(const Math::Vector2& off) { offset = off; }
    void setTrigger(bool trigger) { isTrigger = trigger; }

    // Get world position (rigidbody pos + offset)
    virtual Math::Vector2 getWorldPosition() const = 0;

    // Debug rendering
    virtual void debugDraw(sf::RenderWindow* window) const = 0;

    // Internal - set owner
    void setOwner(Rigidbody2D* rb) { owner = rb; }
};

// Circle collider - simple and fast
class CircleCollider2D : public Collider2D {
private:
    float radius;

public:
    CircleCollider2D(float radius)
        : Collider2D(ColliderType::Circle), radius(radius) {}

    float getRadius() const { return radius; }
    void setRadius(float r) { radius = r; }

    Math::Vector2 getWorldPosition() const override;

    void debugDraw(sf::RenderWindow* window) const override {
        Math::Vector2 pos = getWorldPosition();
        sf::CircleShape circle(radius);
        circle.setPosition(sf::Vector2f(pos.x - radius, pos.y - radius));
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(sf::Color::Green);
        circle.setOutlineThickness(1.0f);
        window->draw(circle);
    }
};

// Box collider (AABB - Axis-Aligned Bounding Box)
class BoxCollider2D : public Collider2D {
private:
    float width;
    float height;

public:
    BoxCollider2D(float width, float height)
        : Collider2D(ColliderType::Box), width(width), height(height) {}

    float getWidth() const { return width; }
    float getHeight() const { return height; }
    void setWidth(float w) { width = w; }
    void setHeight(float h) { height = h; }
    void setSize(float w, float h) { width = w; height = h; }

    Math::Vector2 getWorldPosition() const override;

    // Get min/max for AABB collision
    Math::Vector2 getMin() const {
        Math::Vector2 pos = getWorldPosition();
        return Math::Vector2(pos.x - width / 2, pos.y - height / 2);
    }

    Math::Vector2 getMax() const {
        Math::Vector2 pos = getWorldPosition();
        return Math::Vector2(pos.x + width / 2, pos.y + height / 2);
    }

    void debugDraw(sf::RenderWindow* window) const override {
        Math::Vector2 pos = getWorldPosition();
        sf::RectangleShape rect(sf::Vector2f(width, height));
        rect.setPosition(sf::Vector2f(pos.x - width / 2, pos.y - height / 2));
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Green);
        rect.setOutlineThickness(1.0f);
        window->draw(rect);
    }
};

}}

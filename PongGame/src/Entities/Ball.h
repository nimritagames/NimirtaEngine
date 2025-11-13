#pragma once
#include "GameEntity.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

class Ball : public GameEntity {
private:
    float radius;
    float initialSpeed;
    float currentSpeed;

public:
    Ball(float x, float y, float radius, float speed)
        : radius(radius), initialSpeed(speed), currentSpeed(speed) {
        setPosition(x, y);
        setSize(radius * 2, radius * 2);
        setColor(sf::Color::White);
        srand(static_cast<unsigned>(time(nullptr)));
    }

    void reset(float x, float y) {
        setPosition(x, y);
        currentSpeed = initialSpeed;

        float angle = (rand() % 60 - 30) * 3.14159f / 180.0f;
        float direction = (rand() % 2 == 0) ? 1.0f : -1.0f;

        velocity.x = direction * currentSpeed * cos(angle);
        velocity.y = currentSpeed * sin(angle);
    }

    void update(float deltaTime) override {
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
    }

    void render(sf::RenderWindow* window) override {
        sf::CircleShape circle(radius);
        // Position is stored as center, SFML CircleShape uses top-left origin
        circle.setPosition({position.x - radius, position.y - radius});
        circle.setFillColor(color);
        window->draw(circle);
    }

    void bounceY() {
        velocity.y = -velocity.y;
    }

    void bounceX() {
        velocity.x = -velocity.x;
        currentSpeed *= 1.05f;
    }

    void handlePaddleCollision(float paddleCenterY) {
        float ballCenterY = position.y;
        float relativeIntersectY = paddleCenterY - ballCenterY;
        float normalizedIntersect = relativeIntersectY / 50.0f;
        float bounceAngle = normalizedIntersect * (60.0f * 3.14159f / 180.0f);

        float direction = (velocity.x > 0) ? -1.0f : 1.0f;
        velocity.x = direction * currentSpeed * cos(bounceAngle);
        velocity.y = -currentSpeed * sin(bounceAngle);

        currentSpeed *= 1.05f;
    }

    float getRadius() const {
        return radius;
    }

    // Get bounding box for the circle (for AABB collision checks)
    sf::FloatRect getBounds() const {
        return sf::FloatRect({position.x - radius, position.y - radius}, {radius * 2, radius * 2});
    }

    // Get center position (ball position is stored as center)
    Engine::Math::Vector2 getCenter() const {
        return position;
    }
};

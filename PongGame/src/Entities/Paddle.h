#pragma once
#include "GameEntity.h"

class Paddle : public GameEntity {
private:
    float speed;
    float minY;
    float maxY;

public:
    Paddle(float x, float y, float width, float height, float speed)
        : speed(speed), minY(0), maxY(600) {
        setPosition(x, y);
        setSize(width, height);
        setColor(sf::Color::White);
    }

    void setBounds(float minY, float maxY) {
        this->minY = minY;
        this->maxY = maxY;
    }

    void moveUp(float deltaTime) {
        position.y -= speed * deltaTime;
        if (position.y < minY) {
            position.y = minY;
        }
    }

    void moveDown(float deltaTime) {
        position.y += speed * deltaTime;
        if (position.y + size.y > maxY) {
            position.y = maxY - size.y;
        }
    }

    void render(sf::RenderWindow* window) override {
        sf::RectangleShape rect(size);
        rect.setPosition({position.x, position.y});
        rect.setFillColor(color);
        window->draw(rect);
    }

    float getCenterY() const {
        return position.y + size.y / 2.0f;
    }
};

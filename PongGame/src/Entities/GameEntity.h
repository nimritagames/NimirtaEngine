#pragma once
#include "../../../Engine/ECS/Entity.h"
#include <SFML/Graphics.hpp>

// Game-specific base entity that adds rendering and SFML-specific features
// This sits between the pure engine and specific game entities
class GameEntity : public Engine::ECS::Entity {
protected:
    sf::Vector2f size;
    sf::Color color;

public:
    GameEntity() : Engine::ECS::Entity(), size(0, 0), color(sf::Color::White) {}

    virtual ~GameEntity() {}

    // Game entities must implement rendering
    virtual void render(sf::RenderWindow* window) = 0;

    // Game-specific setters
    void setSize(float width, float height) {
        size.x = width;
        size.y = height;
    }

    void setColor(const sf::Color& c) {
        color = c;
    }

    sf::Vector2f getSize() const {
        return size;
    }

    sf::Color getColor() const {
        return color;
    }

    // Collision bounds - game-specific
    sf::FloatRect getBounds() const {
        return sf::FloatRect({position.x, position.y}, {size.x, size.y});
    }
};

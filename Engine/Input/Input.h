#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace Engine {
    namespace Input {
        class Input {
        public:
            static bool isKeyPressed(sf::Keyboard::Key key) {
                return sf::Keyboard::isKeyPressed(key);
            }

            static bool isMouseButtonPressed(sf::Mouse::Button button) {
                return sf::Mouse::isButtonPressed(button);
            }

            static sf::Vector2i getMousePosition(const sf::RenderWindow& window) {
                return sf::Mouse::getPosition(window);
            }
        };
    }
}

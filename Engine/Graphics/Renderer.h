#pragma once
#include <SFML/Graphics.hpp>
#include <array>

namespace Engine {
    namespace Graphics {
        class Renderer {
        private:
            sf::RenderWindow* window;

        public:
            Renderer(sf::RenderWindow* window) : window(window) {}

            void drawRectangle(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color) {
                sf::RectangleShape rect(size);
                rect.setPosition(position);
                rect.setFillColor(color);
                window->draw(rect);
            }

            void drawCircle(const sf::Vector2f& position, float radius, const sf::Color& color) {
                sf::CircleShape circle(radius);
                circle.setPosition(position - sf::Vector2f(radius, radius));
                circle.setFillColor(color);
                window->draw(circle);
            }

            void drawText(const std::string& text, const sf::Vector2f& position,
                         const sf::Font& font, unsigned int size, const sf::Color& color) {
                sf::Text textObj(font);
                textObj.setString(text);
                textObj.setCharacterSize(size);
                textObj.setFillColor(color);
                textObj.setPosition(position);
                window->draw(textObj);
            }

            void drawLine(const sf::Vector2f& start, const sf::Vector2f& end,
                         const sf::Color& color, float thickness = 1.0f) {
                std::array<sf::Vertex, 2> line = {{
                    {start, color},
                    {end, color}
                }};
                window->draw(line.data(), 2, sf::PrimitiveType::Lines);
            }
        };
    }
}

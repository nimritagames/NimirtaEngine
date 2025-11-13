#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

namespace Engine {
    namespace Core {
        class Window {
        private:
            sf::RenderWindow* window;
            std::string title;
            unsigned int width;
            unsigned int height;

        public:
            Window(const std::string& title, unsigned int width, unsigned int height)
                : title(title), width(width), height(height) {
                window = new sf::RenderWindow(sf::VideoMode({width, height}), title);
                window->setFramerateLimit(60);
            }

            ~Window() {
                if (window) {
                    window->close();
                    delete window;
                }
            }

            bool isOpen() const {
                return window->isOpen();
            }

            void clear(const sf::Color& color = sf::Color::Black) {
                window->clear(color);
            }

            void display() {
                window->display();
            }

            std::optional<sf::Event> pollEvent() {
                return window->pollEvent();
            }

            void close() {
                window->close();
            }

            sf::RenderWindow* getRenderWindow() {
                return window;
            }

            unsigned int getWidth() const {
                return width;
            }

            unsigned int getHeight() const {
                return height;
            }
        };
    }
}

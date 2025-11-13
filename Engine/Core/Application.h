#pragma once
#include "Window.h"
#include "Time.h"
#include "../Graphics/Renderer.h"
#include "../Input/Input.h"

namespace Engine {
    namespace Core {
        class Application {
        protected:
            Window* window;
            Graphics::Renderer* renderer;
            bool running;

        public:
            Application(const std::string& title, unsigned int width, unsigned int height)
                : running(false) {
                window = new Window(title, width, height);
                renderer = new Graphics::Renderer(window->getRenderWindow());
                Time::restart();
            }

            virtual ~Application() {
                delete renderer;
                delete window;
            }

            void run() {
                running = true;
                onStart();

                while (window->isOpen() && running) {
                    Time::update();
                    processEvents();
                    update(Time::getDeltaTime());
                    render();
                }

                onExit();
            }

            void stop() {
                running = false;
            }

        protected:
            virtual void onStart() {}
            virtual void onExit() {}
            virtual void update(float deltaTime) = 0;
            virtual void render() = 0;

            void processEvents() {
                while (auto event = window->pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window->close();
                    }
                    onEvent(*event);
                }
            }

            virtual void onEvent(const sf::Event& event) {}

            Window* getWindow() {
                return window;
            }

            Graphics::Renderer* getRenderer() {
                return renderer;
            }
        };
    }
}

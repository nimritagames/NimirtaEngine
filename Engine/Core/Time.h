#pragma once
#include <SFML/System/Clock.hpp>

namespace Engine {
    namespace Core {
        class Time {
        private:
            static sf::Clock clock;
            static float deltaTime;
            static float lastFrameTime;

        public:
            static void update() {
                float currentTime = clock.getElapsedTime().asSeconds();
                deltaTime = currentTime - lastFrameTime;
                lastFrameTime = currentTime;
            }

            static float getDeltaTime() {
                return deltaTime;
            }

            static float getElapsedTime() {
                return clock.getElapsedTime().asSeconds();
            }

            static void restart() {
                clock.restart();
                deltaTime = 0.0f;
                lastFrameTime = 0.0f;
            }
        };

        sf::Clock Time::clock;
        float Time::deltaTime = 0.0f;
        float Time::lastFrameTime = 0.0f;
    }
}

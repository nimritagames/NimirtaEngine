#pragma once
#include "../Math/Vector2.h"

namespace Engine {
    namespace ECS {
        // Pure, generic entity class - no game-specific or SFML-specific code
        class Entity {
        protected:
            Math::Vector2 position;
            Math::Vector2 velocity;
            bool active;

        public:
            Entity() : position(0, 0), velocity(0, 0), active(true) {}

            virtual ~Entity() {}

            // Generic update - just applies velocity
            virtual void update(float deltaTime) {
                position += velocity * deltaTime;
            }

            // Getters and setters - pure data access
            void setPosition(float x, float y) {
                position.x = x;
                position.y = y;
            }

            void setPosition(const Math::Vector2& pos) {
                position = pos;
            }

            void setVelocity(float x, float y) {
                velocity.x = x;
                velocity.y = y;
            }

            void setVelocity(const Math::Vector2& vel) {
                velocity = vel;
            }

            Math::Vector2 getPosition() const {
                return position;
            }

            Math::Vector2 getVelocity() const {
                return velocity;
            }

            bool isActive() const {
                return active;
            }

            void setActive(bool active) {
                this->active = active;
            }
        };
    }
}

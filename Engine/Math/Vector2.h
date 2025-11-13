#pragma once

namespace Engine {
    namespace Math {
        struct Vector2 {
            float x, y;

            Vector2() : x(0.0f), y(0.0f) {}
            Vector2(float x, float y) : x(x), y(y) {}

            Vector2 operator+(const Vector2& other) const {
                return Vector2(x + other.x, y + other.y);
            }

            Vector2 operator-(const Vector2& other) const {
                return Vector2(x - other.x, y - other.y);
            }

            Vector2 operator*(float scalar) const {
                return Vector2(x * scalar, y * scalar);
            }

            Vector2& operator+=(const Vector2& other) {
                x += other.x;
                y += other.y;
                return *this;
            }

            Vector2& operator-=(const Vector2& other) {
                x -= other.x;
                y -= other.y;
                return *this;
            }

            float magnitude() const {
                return std::sqrt(x * x + y * y);
            }

            Vector2 normalized() const {
                float mag = magnitude();
                if (mag > 0.0f) {
                    return Vector2(x / mag, y / mag);
                }
                return Vector2(0.0f, 0.0f);
            }

            // Dot product
            float dot(const Vector2& other) const {
                return x * other.x + y * other.y;
            }

            // Distance between two points
            static float distance(const Vector2& a, const Vector2& b) {
                return (b - a).magnitude();
            }

            // Clamp a value between min and max
            static float clamp(float value, float min, float max) {
                if (value < min) return min;
                if (value > max) return max;
                return value;
            }
        };
    }
}

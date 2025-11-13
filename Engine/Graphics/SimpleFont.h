#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace Engine {
    namespace Graphics {
        class SimpleFont {
        public:
            // 5x7 bitmap font patterns for uppercase letters and digits
            static const bool* getCharPattern(char c) {
                static const bool patterns[][35] = {
                    // A
                    {0,1,1,1,0,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,1,1,1,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1},
                    // B
                    {1,1,1,1,0,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,1,1,1,0,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,1,1,1,0},
                    // C
                    {0,1,1,1,0,
                     1,0,0,0,1,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     1,0,0,0,1,
                     0,1,1,1,0},
                    // D
                    {1,1,1,1,0,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,1,1,1,0},
                    // E
                    {1,1,1,1,1,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     1,1,1,1,0,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     1,1,1,1,1},
                    // F
                    {1,1,1,1,1,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     1,1,1,1,0,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     1,0,0,0,0},
                    // G
                    {0,1,1,1,0,
                     1,0,0,0,1,
                     1,0,0,0,0,
                     1,0,1,1,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     0,1,1,1,0},
                    // H
                    {1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,1,1,1,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1},
                    // I
                    {1,1,1,1,1,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     1,1,1,1,1},
                    // J
                    {0,0,1,1,1,
                     0,0,0,1,0,
                     0,0,0,1,0,
                     0,0,0,1,0,
                     0,0,0,1,0,
                     1,0,0,1,0,
                     0,1,1,0,0},
                    // K
                    {1,0,0,0,1,
                     1,0,0,1,0,
                     1,0,1,0,0,
                     1,1,0,0,0,
                     1,0,1,0,0,
                     1,0,0,1,0,
                     1,0,0,0,1},
                    // L
                    {1,0,0,0,0,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     1,1,1,1,1},
                    // M
                    {1,0,0,0,1,
                     1,1,0,1,1,
                     1,0,1,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1},
                    // N
                    {1,0,0,0,1,
                     1,1,0,0,1,
                     1,0,1,0,1,
                     1,0,0,1,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1},
                    // O
                    {0,1,1,1,0,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     0,1,1,1,0},
                    // P
                    {1,1,1,1,0,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,1,1,1,0,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     1,0,0,0,0},
                    // Q
                    {0,1,1,1,0,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,1,0,1,
                     1,0,0,1,0,
                     0,1,1,0,1},
                    // R
                    {1,1,1,1,0,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,1,1,1,0,
                     1,0,1,0,0,
                     1,0,0,1,0,
                     1,0,0,0,1},
                    // S
                    {0,1,1,1,1,
                     1,0,0,0,0,
                     1,0,0,0,0,
                     0,1,1,1,0,
                     0,0,0,0,1,
                     0,0,0,0,1,
                     1,1,1,1,0},
                    // T
                    {1,1,1,1,1,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     0,0,1,0,0},
                    // U
                    {1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     0,1,1,1,0},
                    // V
                    {1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     0,1,0,1,0,
                     0,0,1,0,0},
                    // W
                    {1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,0,0,1,
                     1,0,1,0,1,
                     1,1,0,1,1,
                     1,0,0,0,1},
                    // X
                    {1,0,0,0,1,
                     1,0,0,0,1,
                     0,1,0,1,0,
                     0,0,1,0,0,
                     0,1,0,1,0,
                     1,0,0,0,1,
                     1,0,0,0,1},
                    // Y
                    {1,0,0,0,1,
                     1,0,0,0,1,
                     0,1,0,1,0,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     0,0,1,0,0},
                    // Z
                    {1,1,1,1,1,
                     0,0,0,0,1,
                     0,0,0,1,0,
                     0,0,1,0,0,
                     0,1,0,0,0,
                     1,0,0,0,0,
                     1,1,1,1,1},
                    // : (colon)
                    {0,0,0,0,0,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     0,0,0,0,0,
                     0,0,1,0,0,
                     0,0,1,0,0,
                     0,0,0,0,0},
                    // / (slash)
                    {0,0,0,0,1,
                     0,0,0,0,1,
                     0,0,0,1,0,
                     0,0,1,0,0,
                     0,1,0,0,0,
                     1,0,0,0,0,
                     1,0,0,0,0},
                };

                int index = -1;
                if (c >= 'A' && c <= 'Z') {
                    index = c - 'A';
                } else if (c >= 'a' && c <= 'z') {
                    index = c - 'a'; // Lowercase same as uppercase
                } else if (c == ':') {
                    index = 26;
                } else if (c == '/') {
                    index = 27;
                }

                if (index >= 0 && index < 28) {
                    return patterns[index];
                }

                // Return empty pattern for unknown characters
                static const bool empty[35] = {0};
                return empty;
            }

            static void drawText(sf::RenderWindow* window, const std::string& text,
                               float x, float y, float pixelSize, const sf::Color& color) {
                float currentX = x;

                for (char c : text) {
                    if (c == ' ') {
                        currentX += 6 * pixelSize;
                        continue;
                    }

                    const bool* pattern = getCharPattern(c);

                    // Draw 5x7 character
                    for (int row = 0; row < 7; row++) {
                        for (int col = 0; col < 5; col++) {
                            if (pattern[row * 5 + col]) {
                                sf::RectangleShape pixel({pixelSize, pixelSize});
                                pixel.setPosition({currentX + col * pixelSize, y + row * pixelSize});
                                pixel.setFillColor(color);
                                window->draw(pixel);
                            }
                        }
                    }

                    currentX += 6 * pixelSize; // 5 pixels + 1 spacing
                }
            }

            static float getTextWidth(const std::string& text, float pixelSize) {
                float width = 0;
                for (char c : text) {
                    if (c == ' ') {
                        width += 6 * pixelSize;
                    } else {
                        width += 6 * pixelSize;
                    }
                }
                return width - pixelSize; // Remove last spacing
            }

            static void drawTextCentered(sf::RenderWindow* window, const std::string& text,
                                        float centerX, float y, float pixelSize, const sf::Color& color) {
                float width = getTextWidth(text, pixelSize);
                drawText(window, text, centerX - width / 2.0f, y, pixelSize, color);
            }
        };
    }
}

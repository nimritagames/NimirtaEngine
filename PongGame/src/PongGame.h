#pragma once
#include "../../Engine/Core/Application.h"
#include "../../Engine/Graphics/SimpleFont.h"
#include "Entities/Paddle.h"
#include "Entities/Ball.h"
#include "AI/AIController.h"
#include <sstream>

enum class GameState {
    MainMenu,
    Playing,
    Paused,
    ExitConfirmation,
    GameOver
};

enum class GameMode {
    TwoPlayer,
    VsAI
};

class PongGame : public Engine::Core::Application {
private:
    Paddle* leftPaddle;
    Paddle* rightPaddle;
    Ball* ball;
    AIController* aiController;

    int leftScore;
    int rightScore;

    GameState gameState;
    GameMode gameMode;
    AIDifficulty aiDifficulty;

    int selectedMenuOption;
    int selectedDifficultyOption;
    int selectedPauseOption;
    int selectedExitOption;
    bool selectingDifficulty;
    GameState previousState;

    const float WINDOW_WIDTH = 800.0f;
    const float WINDOW_HEIGHT = 600.0f;
    const float PADDLE_WIDTH = 15.0f;
    const float PADDLE_HEIGHT = 100.0f;
    const float PADDLE_SPEED = 400.0f;
    const float BALL_RADIUS = 8.0f;
    const float BALL_SPEED = 300.0f;

public:
    PongGame() : Engine::Core::Application("Pong Game", 800, 600),
        leftScore(0), rightScore(0), gameState(GameState::MainMenu),
        gameMode(GameMode::TwoPlayer), aiDifficulty(AIDifficulty::Medium),
        selectedMenuOption(0), selectedDifficultyOption(1), selectedPauseOption(0),
        selectedExitOption(1), selectingDifficulty(false),
        previousState(GameState::MainMenu), aiController(nullptr) {

        leftPaddle = new Paddle(30, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2,
                                PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED);
        leftPaddle->setBounds(0, WINDOW_HEIGHT);

        rightPaddle = new Paddle(WINDOW_WIDTH - 30 - PADDLE_WIDTH,
                                 WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2,
                                 PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED);
        rightPaddle->setBounds(0, WINDOW_HEIGHT);

        ball = new Ball(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, BALL_RADIUS, BALL_SPEED);
        ball->reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    }

    ~PongGame() {
        delete leftPaddle;
        delete rightPaddle;
        delete ball;
        if (aiController) {
            delete aiController;
        }
    }

protected:
    void onStart() override {
    }

    void update(float deltaTime) override {
        if (gameState == GameState::MainMenu || gameState == GameState::Paused ||
            gameState == GameState::ExitConfirmation) {
            // Menu states don't need update logic
            return;
        }

        if (gameState == GameState::Playing) {
            updateGameplay(deltaTime);
        }
    }

    void updateGameplay(float deltaTime) {
        // Player 1 controls (always human)
        if (Engine::Input::Input::isKeyPressed(sf::Keyboard::Key::W)) {
            leftPaddle->moveUp(deltaTime);
        }
        if (Engine::Input::Input::isKeyPressed(sf::Keyboard::Key::S)) {
            leftPaddle->moveDown(deltaTime);
        }

        // Player 2 controls (human or AI)
        if (gameMode == GameMode::TwoPlayer) {
            if (Engine::Input::Input::isKeyPressed(sf::Keyboard::Key::Up)) {
                rightPaddle->moveUp(deltaTime);
            }
            if (Engine::Input::Input::isKeyPressed(sf::Keyboard::Key::Down)) {
                rightPaddle->moveDown(deltaTime);
            }
        } else {
            // AI controls right paddle
            if (aiController) {
                aiController->update(deltaTime, ball);
            }
        }

        // ESC handled in onEvent for pause menu

        ball->update(deltaTime);
        checkCollisions();
    }

    // Circle-Rectangle collision detection
    bool checkCircleRectCollision(const Engine::Math::Vector2& circleCenter, float radius,
                                   const Engine::Math::Vector2& rectPos, const sf::Vector2f& rectSize) {
        // Find the closest point on the rectangle to the circle center
        float closestX = Engine::Math::Vector2::clamp(circleCenter.x, rectPos.x, rectPos.x + rectSize.x);
        float closestY = Engine::Math::Vector2::clamp(circleCenter.y, rectPos.y, rectPos.y + rectSize.y);

        // Calculate distance between circle center and closest point
        Engine::Math::Vector2 closest(closestX, closestY);
        float distance = Engine::Math::Vector2::distance(circleCenter, closest);

        return distance < radius;
    }

    void checkCollisions() {
        auto ballPos = ball->getPosition();
        float ballRadius = ball->getRadius();

        // Wall collisions (top and bottom)
        if (ballPos.y - ballRadius <= 0) {
            ball->bounceY();
            // Push ball out of wall to prevent getting stuck
            ball->setPosition(ballPos.x, ballRadius);
        } else if (ballPos.y + ballRadius >= WINDOW_HEIGHT) {
            ball->bounceY();
            // Push ball out of wall to prevent getting stuck
            ball->setPosition(ballPos.x, WINDOW_HEIGHT - ballRadius);
        }

        // Paddle collisions (use circle-rectangle collision)
        auto leftPaddlePos = leftPaddle->getPosition();
        auto leftPaddleSize = leftPaddle->getSize();
        auto rightPaddlePos = rightPaddle->getPosition();
        auto rightPaddleSize = rightPaddle->getSize();

        // Left paddle collision
        if (checkCircleRectCollision(ballPos, ballRadius, leftPaddlePos, leftPaddleSize)) {
            // Only process collision if ball is moving towards paddle (prevent double bounce)
            if (ball->getVelocity().x < 0) {
                ball->handlePaddleCollision(leftPaddle->getCenterY());
                // Push ball out of paddle to prevent getting stuck
                ball->setPosition(leftPaddlePos.x + leftPaddleSize.x + ballRadius, ballPos.y);
            }
        }

        // Right paddle collision
        if (checkCircleRectCollision(ballPos, ballRadius, rightPaddlePos, rightPaddleSize)) {
            // Only process collision if ball is moving towards paddle (prevent double bounce)
            if (ball->getVelocity().x > 0) {
                ball->handlePaddleCollision(rightPaddle->getCenterY());
                // Push ball out of paddle to prevent getting stuck
                ball->setPosition(rightPaddlePos.x - ballRadius, ballPos.y);
            }
        }

        // Scoring (left and right boundaries)
        if (ballPos.x - ballRadius <= 0) {
            rightScore++;
            ball->reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        }

        if (ballPos.x + ballRadius >= WINDOW_WIDTH) {
            leftScore++;
            ball->reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        }
    }

    void render() override {
        window->clear(sf::Color::Black);

        if (gameState == GameState::MainMenu) {
            renderMainMenu();
        } else if (gameState == GameState::Playing) {
            renderGameplay();
        } else if (gameState == GameState::Paused) {
            renderGameplay(); // Draw game in background
            renderPauseMenu(); // Overlay pause menu
        } else if (gameState == GameState::ExitConfirmation) {
            if (previousState == GameState::Playing || previousState == GameState::Paused) {
                renderGameplay();
            }
            renderExitConfirmation();
        }

        window->display();
    }

    void renderMainMenu() {
        float centerX = WINDOW_WIDTH / 2;

        // Draw title with shadow effect
        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "PONG",
                                                      centerX + 2, 62, 8.0f, sf::Color(40, 40, 40));
        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "PONG",
                                                      centerX, 60, 8.0f, sf::Color::White);

        if (!selectingDifficulty) {
            // Main menu options with selection box
            float option1Y = 220;
            float option2Y = 300;
            float option3Y = 380;

            // Draw selection box
            float boxWidth = 380;
            float boxY = option1Y;
            if (selectedMenuOption == 1) {
                boxWidth = 280;
                boxY = option2Y;
            } else if (selectedMenuOption == 2) {
                boxWidth = 120;
                boxY = option3Y;
            }

            sf::RectangleShape selectionBox({boxWidth, 50});
            selectionBox.setPosition({centerX - boxWidth/2, boxY - 10});
            selectionBox.setFillColor(sf::Color::Transparent);
            selectionBox.setOutlineColor(sf::Color::Yellow);
            selectionBox.setOutlineThickness(2);
            window->getRenderWindow()->draw(selectionBox);

            Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "PLAY WITH FRIEND",
                                                          centerX, option1Y, 4.0f,
                                                          selectedMenuOption == 0 ? sf::Color::Yellow : sf::Color::White);

            Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "PLAY VS AI",
                                                          centerX, option2Y, 4.0f,
                                                          selectedMenuOption == 1 ? sf::Color::Yellow : sf::Color::White);

            Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "EXIT",
                                                          centerX, option3Y, 4.0f,
                                                          selectedMenuOption == 2 ? sf::Color::Yellow : sf::Color::White);

            // Instructions at bottom
            Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(),
                                                          "USE UP/DOWN TO SELECT",
                                                          centerX, 480, 2.5f, sf::Color(120, 120, 120));
            Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(),
                                                          "PRESS ENTER TO CONFIRM",
                                                          centerX, 510, 2.5f, sf::Color(120, 120, 120));
        } else {
            // Difficulty selection
            Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "SELECT DIFFICULTY",
                                                          centerX, 160, 4.5f, sf::Color::White);

            float easyY = 260;
            float mediumY = 340;
            float hardY = 420;

            // Draw selection box
            float boxY = easyY;
            float boxWidth = 120;
            if (selectedDifficultyOption == 1) {
                boxY = mediumY;
                boxWidth = 180;
            } else if (selectedDifficultyOption == 2) {
                boxY = hardY;
                boxWidth = 120;
            }

            sf::RectangleShape selectionBox({boxWidth, 50});
            selectionBox.setPosition({centerX - boxWidth/2, boxY - 10});
            selectionBox.setFillColor(sf::Color::Transparent);
            selectionBox.setOutlineColor(sf::Color::Yellow);
            selectionBox.setOutlineThickness(2);
            window->getRenderWindow()->draw(selectionBox);

            // Draw difficulty options
            Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "EASY",
                                                          centerX, easyY, 4.0f,
                                                          selectedDifficultyOption == 0 ? sf::Color::Yellow : sf::Color::White);

            Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "MEDIUM",
                                                          centerX, mediumY, 4.0f,
                                                          selectedDifficultyOption == 1 ? sf::Color::Yellow : sf::Color::White);

            Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "HARD",
                                                          centerX, hardY, 4.0f,
                                                          selectedDifficultyOption == 2 ? sf::Color::Yellow : sf::Color::White);

            // Instructions
            Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(),
                                                          "PRESS ENTER TO CONFIRM",
                                                          centerX, 520, 2.5f, sf::Color(120, 120, 120));
            Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(),
                                                          "ESC TO GO BACK",
                                                          centerX, 545, 2.5f, sf::Color(120, 120, 120));
        }
    }

    void renderGameplay() {
        drawCenterLine();

        leftPaddle->render(window->getRenderWindow());
        rightPaddle->render(window->getRenderWindow());
        ball->render(window->getRenderWindow());

        drawScores();

        // Draw mode indicator
        if (gameMode == GameMode::VsAI) {
            std::string diffText = "";
            if (aiDifficulty == AIDifficulty::Easy) diffText = "AI: EASY";
            else if (aiDifficulty == AIDifficulty::Medium) diffText = "AI: MEDIUM";
            else if (aiDifficulty == AIDifficulty::Hard) diffText = "AI: HARD";

            Engine::Graphics::SimpleFont::drawText(window->getRenderWindow(), diffText,
                                                  WINDOW_WIDTH - 200, 10, 2.0f, sf::Color::White);
        }

        Engine::Graphics::SimpleFont::drawText(window->getRenderWindow(), "ESC: Pause",
                                              10, 10, 2.0f, sf::Color::White);
        Engine::Graphics::SimpleFont::drawText(window->getRenderWindow(), "R: Reset",
                                              10, 35, 2.0f, sf::Color::White);
    }

    void renderPauseMenu() {
        // Semi-transparent dark overlay
        sf::RectangleShape overlay({WINDOW_WIDTH, WINDOW_HEIGHT});
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window->getRenderWindow()->draw(overlay);

        float centerX = WINDOW_WIDTH / 2;

        // Title
        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "PAUSED",
                                                      centerX, 100, 6.0f, sf::Color::White);

        // Menu options
        float resumeY = 220;
        float restartY = 290;
        float mainMenuY = 360;
        float exitY = 430;

        // Draw selection boxes
        float boxWidth = 180;
        float boxY = resumeY;
        if (selectedPauseOption == 1) {
            boxWidth = 200;
            boxY = restartY;
        } else if (selectedPauseOption == 2) {
            boxWidth = 260;
            boxY = mainMenuY;
        } else if (selectedPauseOption == 3) {
            boxWidth = 120;
            boxY = exitY;
        }

        sf::RectangleShape selectionBox({boxWidth, 50});
        selectionBox.setPosition({centerX - boxWidth/2, boxY - 10});
        selectionBox.setFillColor(sf::Color::Transparent);
        selectionBox.setOutlineColor(sf::Color::Yellow);
        selectionBox.setOutlineThickness(2);
        window->getRenderWindow()->draw(selectionBox);

        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "RESUME",
                                                      centerX, resumeY, 4.0f,
                                                      selectedPauseOption == 0 ? sf::Color::Yellow : sf::Color::White);

        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "RESTART",
                                                      centerX, restartY, 4.0f,
                                                      selectedPauseOption == 1 ? sf::Color::Yellow : sf::Color::White);

        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "MAIN MENU",
                                                      centerX, mainMenuY, 4.0f,
                                                      selectedPauseOption == 2 ? sf::Color::Yellow : sf::Color::White);

        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "EXIT",
                                                      centerX, exitY, 4.0f,
                                                      selectedPauseOption == 3 ? sf::Color::Yellow : sf::Color::White);

        // Instructions
        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(),
                                                      "USE UP/DOWN TO SELECT",
                                                      centerX, 530, 2.5f, sf::Color(150, 150, 150));
    }

    void renderExitConfirmation() {
        // Semi-transparent dark overlay
        sf::RectangleShape overlay({WINDOW_WIDTH, WINDOW_HEIGHT});
        overlay.setFillColor(sf::Color(0, 0, 0, 200));
        window->getRenderWindow()->draw(overlay);

        float centerX = WINDOW_WIDTH / 2;

        // Dialog box
        sf::RectangleShape dialogBox({500, 280});
        dialogBox.setPosition({centerX - 250, 180});
        dialogBox.setFillColor(sf::Color(20, 20, 20));
        dialogBox.setOutlineColor(sf::Color::White);
        dialogBox.setOutlineThickness(3);
        window->getRenderWindow()->draw(dialogBox);

        // Title
        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(),
                                                      "ARE YOU SURE?",
                                                      centerX, 220, 5.0f, sf::Color::White);

        // Message
        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(),
                                                      "DO YOU WANT TO EXIT THE GAME?",
                                                      centerX, 290, 2.5f, sf::Color(200, 200, 200));

        // Options
        float yesY = 360;
        float noY = 360;
        float yesX = centerX - 100;
        float noX = centerX + 100;

        // Draw selection boxes
        if (selectedExitOption == 0) {
            sf::RectangleShape selectionBox({100, 50});
            selectionBox.setPosition({yesX - 50, yesY - 10});
            selectionBox.setFillColor(sf::Color::Transparent);
            selectionBox.setOutlineColor(sf::Color(255, 100, 100));
            selectionBox.setOutlineThickness(2);
            window->getRenderWindow()->draw(selectionBox);
        } else {
            sf::RectangleShape selectionBox({80, 50});
            selectionBox.setPosition({noX - 40, noY - 10});
            selectionBox.setFillColor(sf::Color::Transparent);
            selectionBox.setOutlineColor(sf::Color(100, 255, 100));
            selectionBox.setOutlineThickness(2);
            window->getRenderWindow()->draw(selectionBox);
        }

        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "YES",
                                                      yesX, yesY, 4.0f,
                                                      selectedExitOption == 0 ? sf::Color(255, 100, 100) : sf::Color::White);

        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(), "NO",
                                                      noX, noY, 4.0f,
                                                      selectedExitOption == 1 ? sf::Color(100, 255, 100) : sf::Color::White);

        // Instructions
        Engine::Graphics::SimpleFont::drawTextCentered(window->getRenderWindow(),
                                                      "LEFT/RIGHT TO SELECT  ENTER TO CONFIRM",
                                                      centerX, 420, 2.0f, sf::Color(150, 150, 150));
    }

    void drawCenterLine() {
        for (int i = 0; i < WINDOW_HEIGHT; i += 20) {
            renderer->drawRectangle(
                sf::Vector2f(WINDOW_WIDTH / 2 - 2, static_cast<float>(i)),
                sf::Vector2f(4, 10),
                sf::Color(100, 100, 100)
            );
        }
    }

    void drawDigit(int digit, float x, float y, float size) {
        bool segments[7];

        switch(digit) {
            case 0: segments[0]=1; segments[1]=1; segments[2]=1; segments[3]=1; segments[4]=1; segments[5]=1; segments[6]=0; break;
            case 1: segments[0]=0; segments[1]=1; segments[2]=1; segments[3]=0; segments[4]=0; segments[5]=0; segments[6]=0; break;
            case 2: segments[0]=1; segments[1]=1; segments[2]=0; segments[3]=1; segments[4]=1; segments[5]=0; segments[6]=1; break;
            case 3: segments[0]=1; segments[1]=1; segments[2]=1; segments[3]=1; segments[4]=0; segments[5]=0; segments[6]=1; break;
            case 4: segments[0]=0; segments[1]=1; segments[2]=1; segments[3]=0; segments[4]=0; segments[5]=1; segments[6]=1; break;
            case 5: segments[0]=1; segments[1]=0; segments[2]=1; segments[3]=1; segments[4]=0; segments[5]=1; segments[6]=1; break;
            case 6: segments[0]=1; segments[1]=0; segments[2]=1; segments[3]=1; segments[4]=1; segments[5]=1; segments[6]=1; break;
            case 7: segments[0]=1; segments[1]=1; segments[2]=1; segments[3]=0; segments[4]=0; segments[5]=0; segments[6]=0; break;
            case 8: segments[0]=1; segments[1]=1; segments[2]=1; segments[3]=1; segments[4]=1; segments[5]=1; segments[6]=1; break;
            case 9: segments[0]=1; segments[1]=1; segments[2]=1; segments[3]=1; segments[4]=0; segments[5]=1; segments[6]=1; break;
        }

        float thickness = size * 0.15f;
        float width = size * 0.6f;
        float height = size * 0.5f;

        if (segments[0]) renderer->drawRectangle({x, y}, {width, thickness}, sf::Color::White);
        if (segments[1]) renderer->drawRectangle({x + width - thickness, y}, {thickness, height}, sf::Color::White);
        if (segments[2]) renderer->drawRectangle({x + width - thickness, y + height}, {thickness, height}, sf::Color::White);
        if (segments[3]) renderer->drawRectangle({x, y + height * 2 - thickness}, {width, thickness}, sf::Color::White);
        if (segments[4]) renderer->drawRectangle({x, y + height}, {thickness, height}, sf::Color::White);
        if (segments[5]) renderer->drawRectangle({x, y}, {thickness, height}, sf::Color::White);
        if (segments[6]) renderer->drawRectangle({x, y + height - thickness/2}, {width, thickness}, sf::Color::White);
    }

    void drawScores() {
        float leftX = WINDOW_WIDTH / 4 - 20;
        float scoreY = 40;
        float digitSize = 60;

        if (leftScore < 10) {
            drawDigit(leftScore, leftX, scoreY, digitSize);
        } else {
            drawDigit(leftScore / 10, leftX - 25, scoreY, digitSize);
            drawDigit(leftScore % 10, leftX + 25, scoreY, digitSize);
        }

        float rightX = 3 * WINDOW_WIDTH / 4 - 20;

        if (rightScore < 10) {
            drawDigit(rightScore, rightX, scoreY, digitSize);
        } else {
            drawDigit(rightScore / 10, rightX - 25, scoreY, digitSize);
            drawDigit(rightScore % 10, rightX + 25, scoreY, digitSize);
        }
    }

    void onEvent(const sf::Event& event) override {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            if (gameState == GameState::MainMenu) {
                handleMenuInput(keyPressed->code);
            } else if (gameState == GameState::Playing) {
                handleGameplayInput(keyPressed->code);
            } else if (gameState == GameState::Paused) {
                handlePauseInput(keyPressed->code);
            } else if (gameState == GameState::ExitConfirmation) {
                handleExitConfirmation(keyPressed->code);
            }
        }
    }

    void handleMenuInput(sf::Keyboard::Key key) {
        if (!selectingDifficulty) {
            if (key == sf::Keyboard::Key::Up) {
                selectedMenuOption = (selectedMenuOption - 1 + 3) % 3;
            } else if (key == sf::Keyboard::Key::Down) {
                selectedMenuOption = (selectedMenuOption + 1) % 3;
            } else if (key == sf::Keyboard::Key::Enter) {
                if (selectedMenuOption == 0) {
                    // Play with Friend
                    gameMode = GameMode::TwoPlayer;
                    startGame();
                } else if (selectedMenuOption == 1) {
                    // Play vs AI - show difficulty selection
                    selectingDifficulty = true;
                } else if (selectedMenuOption == 2) {
                    // Exit - show confirmation
                    previousState = GameState::MainMenu;
                    gameState = GameState::ExitConfirmation;
                    selectedExitOption = 1; // Default to "No"
                }
            }
        } else {
            // Difficulty selection
            if (key == sf::Keyboard::Key::Up) {
                selectedDifficultyOption = (selectedDifficultyOption - 1 + 3) % 3;
            } else if (key == sf::Keyboard::Key::Down) {
                selectedDifficultyOption = (selectedDifficultyOption + 1) % 3;
            } else if (key == sf::Keyboard::Key::Enter) {
                gameMode = GameMode::VsAI;

                switch(selectedDifficultyOption) {
                    case 0: aiDifficulty = AIDifficulty::Easy; break;
                    case 1: aiDifficulty = AIDifficulty::Medium; break;
                    case 2: aiDifficulty = AIDifficulty::Hard; break;
                }

                selectingDifficulty = false;
                startGame();
            } else if (key == sf::Keyboard::Key::Escape) {
                selectingDifficulty = false;
            }
        }
    }

    void handleGameplayInput(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Key::R) {
            leftScore = 0;
            rightScore = 0;
            ball->reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        } else if (key == sf::Keyboard::Key::Escape) {
            gameState = GameState::Paused;
            selectedPauseOption = 0; // Default to Resume
        }
    }

    void handlePauseInput(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Key::Up) {
            selectedPauseOption = (selectedPauseOption - 1 + 4) % 4;
        } else if (key == sf::Keyboard::Key::Down) {
            selectedPauseOption = (selectedPauseOption + 1) % 4;
        } else if (key == sf::Keyboard::Key::Enter) {
            if (selectedPauseOption == 0) {
                // Resume
                gameState = GameState::Playing;
            } else if (selectedPauseOption == 1) {
                // Restart
                leftScore = 0;
                rightScore = 0;
                ball->reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
                gameState = GameState::Playing;
            } else if (selectedPauseOption == 2) {
                // Main Menu
                gameState = GameState::MainMenu;
                leftScore = 0;
                rightScore = 0;
                if (aiController) {
                    delete aiController;
                    aiController = nullptr;
                }
            } else if (selectedPauseOption == 3) {
                // Exit
                previousState = GameState::Paused;
                gameState = GameState::ExitConfirmation;
                selectedExitOption = 1; // Default to "No"
            }
        } else if (key == sf::Keyboard::Key::Escape) {
            // Resume with ESC
            gameState = GameState::Playing;
        }
    }

    void handleExitConfirmation(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Key::Left || key == sf::Keyboard::Key::Right) {
            selectedExitOption = 1 - selectedExitOption; // Toggle between 0 and 1
        } else if (key == sf::Keyboard::Key::Enter) {
            if (selectedExitOption == 0) {
                // Yes - exit game
                window->close();
            } else {
                // No - go back
                gameState = previousState;
            }
        } else if (key == sf::Keyboard::Key::Escape) {
            // ESC acts as "No"
            gameState = previousState;
        }
    }

    void startGame() {
        leftScore = 0;
        rightScore = 0;
        ball->reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

        if (gameMode == GameMode::VsAI) {
            if (!aiController) {
                aiController = new AIController(rightPaddle, aiDifficulty);
            } else {
                aiController->setDifficulty(aiDifficulty);
            }
        } else {
            if (aiController) {
                delete aiController;
                aiController = nullptr;
            }
        }

        gameState = GameState::Playing;
    }
};

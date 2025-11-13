#pragma once
#include "../Entities/Paddle.h"
#include "../Entities/Ball.h"

enum class AIDifficulty {
    Easy,
    Medium,
    Hard
};

class AIController {
private:
    Paddle* paddle;
    AIDifficulty difficulty;
    float reactionDelay;
    float errorMargin;
    float maxSpeed;
    float targetY;
    float reactionTimer;

public:
    AIController(Paddle* paddle, AIDifficulty difficulty)
        : paddle(paddle), difficulty(difficulty), reactionTimer(0.0f), targetY(0.0f) {

        switch(difficulty) {
            case AIDifficulty::Easy:
                reactionDelay = 0.3f;
                errorMargin = 80.0f;
                maxSpeed = 0.6f;
                break;
            case AIDifficulty::Medium:
                reactionDelay = 0.15f;
                errorMargin = 40.0f;
                maxSpeed = 0.8f;
                break;
            case AIDifficulty::Hard:
                reactionDelay = 0.05f;
                errorMargin = 10.0f;
                maxSpeed = 1.0f;
                break;
        }
    }

    void update(float deltaTime, Ball* ball) {
        reactionTimer += deltaTime;

        // Only update target position after reaction delay
        if (reactionTimer >= reactionDelay) {
            reactionTimer = 0.0f;

            auto ballPos = ball->getPosition();
            auto ballVel = ball->getVelocity();

            // Predict where ball will be
            if (ballVel.x > 0) { // Ball moving toward AI
                targetY = ballPos.y;
            } else {
                // Return to center when ball is moving away
                targetY = 300.0f; // Center of screen
            }

            // Add some error based on difficulty
            float error = (rand() % (int)(errorMargin * 2)) - errorMargin;
            targetY += error;
        }

        float paddleCenterY = paddle->getCenterY();
        float diff = targetY - paddleCenterY;

        // Move paddle toward target with speed based on difficulty
        if (abs(diff) > 5.0f) {
            if (diff < 0) {
                paddle->moveUp(deltaTime * maxSpeed);
            } else {
                paddle->moveDown(deltaTime * maxSpeed);
            }
        }
    }

    void setDifficulty(AIDifficulty newDifficulty) {
        difficulty = newDifficulty;

        switch(difficulty) {
            case AIDifficulty::Easy:
                reactionDelay = 0.3f;
                errorMargin = 80.0f;
                maxSpeed = 0.6f;
                break;
            case AIDifficulty::Medium:
                reactionDelay = 0.15f;
                errorMargin = 40.0f;
                maxSpeed = 0.8f;
                break;
            case AIDifficulty::Hard:
                reactionDelay = 0.05f;
                errorMargin = 10.0f;
                maxSpeed = 1.0f;
                break;
        }
    }
};

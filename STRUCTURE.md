# Project Structure

This repository contains a **2D Game Engine** and games built with it.

## Directory Structure

```
GameEngine/                              ← Root folder
├── Engine/                              ← Reusable 2D Game Engine
│   ├── Core/
│   │   ├── Application.h               ← Base game application class
│   │   ├── Window.h                    ← Window management
│   │   └── Time.h                      ← Delta time tracking
│   ├── Graphics/
│   │   ├── Renderer.h                  ← 2D shape rendering
│   │   └── SimpleFont.h                ← Bitmap font system
│   ├── Input/
│   │   └── Input.h                     ← Keyboard/mouse input
│   ├── Math/
│   │   └── Vector2.h                   ← 2D vector math
│   └── ECS/
│       └── Entity.h                    ← Generic entity base class
│
├── PongGame/                            ← Pong Game Project
│   ├── src/
│   │   ├── Entities/
│   │   │   ├── GameEntity.h            ← SFML bridge entity
│   │   │   ├── Paddle.h                ← Player paddle
│   │   │   └── Ball.h                  ← Game ball
│   │   ├── AI/
│   │   │   └── AIController.h          ← AI opponent (3 difficulties)
│   │   ├── PongGame.h                  ← Main game logic & menus
│   │   └── main.cpp                    ← Entry point
│   └── PongGame.vcxproj                ← Visual Studio project
│
├── Pong/                                ← OLD structure (to be removed)
│
├── Pong.sln                             ← Visual Studio solution
└── README.md                            ← Main documentation
```

## Key Design Principles

### 1. **Engine is 100% Game-Agnostic**
The `Engine/` folder contains **ZERO** knowledge about Pong or any specific game:
- No `Paddle`, `Ball`, or game-specific code
- Pure, reusable components
- Can be used to build Snake, Breakout, Asteroids, etc.

### 2. **GameEntity Bridge Layer**
`PongGame/src/Entities/GameEntity.h` bridges the gap:
- Engine provides generic `Engine::ECS::Entity` (position, velocity)
- GameEntity adds SFML-specific features (rendering, collision, color)
- Game entities (Paddle, Ball) inherit from GameEntity

### 3. **Separation of Concerns**
```
Engine::Entity          ← Generic (position, velocity)
      ↓ inherits
GameEntity              ← SFML bridge (size, color, render, bounds)
      ↓ inherits
Paddle, Ball            ← Game-specific (Pong mechanics)
```

## Adding a New Game

To add another game (e.g., Breakout):

1. Create new folder: `BreakoutGame/`
2. Create `src/` subdirectory
3. Copy `GameEntity.h` or create your own bridge
4. Create game-specific entities: `Brick.h`, `PowerUp.h`, etc.
5. Create `BreakoutGame.h` (inherits from `Engine::Core::Application`)
6. Create `main.cpp`
7. Add `BreakoutGame.vcxproj` to solution

**The Engine folder stays untouched!**

## Build Instructions

### Current Setup (Transitioning)
- Use `Pong.sln` in Visual Studio
- Configuration: Debug/Release, Platform: x64
- SFML 3.0.2 required at `C:\Nimrita\Projects\C++\SFML-3.0.2`

### Future Setup (After Cleanup)
- Remove old `Pong/` subfolder
- Rename `Pong.sln` → `GameEngine.sln`
- Update project references to new structure

## Technologies

- **Language:** C++17
- **Graphics:** SFML 3.0.2 (static linking)
- **Build System:** MSBuild (Visual Studio 2022)
- **Architecture:** ECS-inspired entity system

## Projects

### 1. Engine (Core Library)
- Standalone 2D game engine
- No external game dependencies
- Reusable across multiple games

### 2. Pong Game
- Classic Pong with modern UX
- AI opponent (Easy/Medium/Hard)
- Professional menus and pause system
- Built entirely on the custom engine

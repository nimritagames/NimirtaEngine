# 2D Game Engine & Pong Implementation

A reusable 2D game engine built in C++ with SFML 3.0.2, featuring a complete Pong game implementation with AI opponents and professional UX.

> **📂 See [PROJECT_STRUCTURE_VISUAL.md](PROJECT_STRUCTURE_VISUAL.md) for detailed folder organization**

## 🎮 Two Projects in One Repository

### 1. **Game Engine** (`Engine/`)
Standalone, reusable 2D game engine with:
- Entity-Component System architecture
- Rendering pipeline (shapes, text, lines)
- Input handling (keyboard/mouse)
- Time management (delta time, frame-independent)
- 2D vector math library
- **Zero game-specific code** - can build any 2D game

### 2. **Pong Game** (`PongGame/`)
Feature-complete Pong implementation:
- Two-player local multiplayer
- AI opponent with 3 difficulty levels
- Professional menu system with pause functionality
- Exit confirmations and modern UX
- Retro 7-segment score display

## Project Structure

```
Pong/
├── Engine/              # General-purpose game engine
│   ├── Core/           # Core engine components
│   │   ├── Application.h    # Base application class
│   │   ├── Window.h         # Window management
│   │   └── Time.h           # Time and delta time tracking
│   ├── Graphics/       # Rendering system
│   │   └── Renderer.h       # 2D rendering utilities
│   ├── Input/          # Input handling
│   │   └── Input.h          # Keyboard and mouse input
│   ├── Math/           # Math utilities
│   │   └── Vector2.h        # 2D vector math
│   └── ECS/            # Entity Component System
│       └── Entity.h         # Base entity class
├── Game/               # Pong-specific game code
│   ├── Ball.h              # Ball entity
│   ├── Paddle.h            # Paddle entity
│   └── PongGame.h          # Main game logic
└── main.cpp            # Application entry point
```

## Engine Architecture

The engine follows a component-based architecture with the following features:

### Core Systems
- **Application**: Base class for game applications with game loop
- **Window**: SFML window wrapper with event handling
- **Time**: Delta time tracking for frame-independent movement

### Graphics
- **Renderer**: Simple 2D rendering for shapes and text

### Input
- **Input**: Keyboard and mouse input handling

### Math
- **Vector2**: 2D vector operations (addition, subtraction, magnitude, normalization)

### ECS (Entity Component System)
- **Entity**: Base class for game objects with position, velocity, and collision

## Game Features

- Two-player local multiplayer
- Ball physics with paddle collision
- Score tracking
- Increasing ball speed on paddle hits
- Center line divider

## Controls

### Player 1 (Left Paddle)
- `W` - Move up
- `S` - Move down

### Player 2 (Right Paddle)
- `Up Arrow` - Move up
- `Down Arrow` - Move down

### Other Controls
- `R` - Reset scores
- `Escape` - Exit game

## Building the Project

### Requirements
- Visual Studio 2022 (or compatible)
- SFML 3.0.2 (located at `C:\Nimrita\Projects\C++\SFML-3.0.2`)
- C++17 or later

### Build Instructions

1. Open `Pong.sln` in Visual Studio
2. Select your desired configuration (Debug/Release) and platform (x64 recommended)
3. Build the solution (F7 or Build > Build Solution)
4. Run the project (F5 or Debug > Start Debugging)

### SFML Configuration

The project is configured to use SFML static libraries. The following libraries are linked:

**Debug (x64):**
- sfml-graphics-s-d.lib
- sfml-window-s-d.lib
- sfml-system-s-d.lib
- opengl32.lib
- winmm.lib
- gdi32.lib
- freetype.lib

**Release (x64):**
- sfml-graphics-s.lib
- sfml-window-s.lib
- sfml-system-s.lib
- opengl32.lib
- winmm.lib
- gdi32.lib
- freetype.lib

## Game Rules

1. Each player controls a paddle to hit the ball
2. If the ball passes a paddle and reaches the edge, the opposing player scores
3. The ball speed increases slightly with each paddle hit
4. The ball's angle changes based on where it hits the paddle

## Engine Design Philosophy

The engine is designed to be:

- **Modular**: Separate systems for core, graphics, input, and math
- **Extensible**: Easy to add new entity types and components
- **General-purpose**: Not specifically tied to Pong, can be used for other 2D games
- **Lightweight**: Minimal abstraction over SFML for performance

## Future Enhancements

Potential improvements for the engine:
- Audio system for sound effects and music
- Scene management for menus and multiple game states
- Resource manager for textures and fonts
- Particle system for visual effects
- Physics system with more advanced collision detection
- Animation system

Potential improvements for the game:
- Single-player mode with AI opponent
- Power-ups and special abilities
- Multiple ball modes
- Customizable paddle and ball colors
- Sound effects and background music
- Pause menu

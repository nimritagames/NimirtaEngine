# 2D Physics Engine

**Philosophy: Under the hood all the power is there, but accessing that power should be easy.**

A custom 2D physics engine built from scratch following expert recommendations from Erin Catto (Box2D creator), Glenn Fiedler (Gaffer On Games), and game physics professionals.

---

## Features

### Core Physics
- ✅ **Semi-implicit Euler integration** (stable and energy-preserving)
- ✅ **Fixed timestep with accumulator** (prevents FPS-dependent behavior)
- ✅ **Impulse-based collision resolution** (instant, time-step independent)
- ✅ **Baumgarte stabilization** (prevents sinking with slop tolerance)
- ✅ **Friction simulation** (Coulomb friction model)
- ✅ **Material system** (bounciness, friction, density)

### Collision Detection
- ✅ Circle vs Circle
- ✅ Box (AABB) vs Box
- ✅ Circle vs Box
- ✅ Separating Axis Theorem (SAT)
- ✅ Closest point method

### Body Types
- **Static**: Doesn't move (walls, ground) - infinite mass
- **Dynamic**: Fully simulated (balls, players, boxes)
- **Kinematic**: Moves but not affected by forces (moving platforms)

---

## Quick Start - Super Easy API!

```cpp
#include "Engine/Physics2D/PhysicsWorld.h"

using namespace Engine::Physics2D;

// 1. Create physics world
PhysicsWorld* physics = new PhysicsWorld();
physics->setGravity(0, -9.8f);  // Earth gravity

// 2. Create a bouncy ball - ONE LINE!
Rigidbody* ball = physics->createCircle(100, 100, 10, PhysicsMaterial::Bouncy());

// 3. Create a static ground - TWO LINES!
Rigidbody* ground = physics->createBox(400, 550, 800, 50);
ground->makeStatic();

// 4. Update physics in your game loop
void update(float deltaTime) {
    physics->update(deltaTime);  // Fixed timestep handled internally!

    // Render ball (with interpolation for smoothness)
    float alpha = physics->getAccumulator() / FIXED_TIMESTEP;
    Vector2 renderPos = ball->getInterpolatedPosition(alpha);
    drawCircle(renderPos, 10);
}
```

That's it! Physics just works.

---

## Easy API Examples

### Creating Objects

```cpp
// Circle (ball, wheel, etc.)
Rigidbody* ball = physics->createCircle(x, y, radius);

// Box (walls, platforms, crates)
Rigidbody* box = physics->createBox(x, y, width, height);

// With custom material
Rigidbody* ice = physics->createCircle(100, 100, 15, PhysicsMaterial::Ice());
```

### Applying Forces

```cpp
// Apply force (continuous)
ball->applyForce(Vector2(100, 0));  // Push right

// Apply impulse (instant)
ball->applyImpulse(Vector2(0, 500));  // Jump!

// Set velocity directly
ball->setVelocity(50, 0);
```

### Body Properties

```cpp
// Make it static (walls, ground)
wall->makeStatic();

// Change mass
ball->setMass(2.0f);

// Disable gravity for this object
ball->disableGravity();

// Lock rotation
ball->setFixedRotation(true);
```

### Materials (Presets Available!)

```cpp
// Use presets
ball->setMaterial(PhysicsMaterial::Bouncy());      // High bounce
ice->setMaterial(PhysicsMaterial::Ice());          // Slippery
crate->setMaterial(PhysicsMaterial::Wood());       // Normal friction

// Or create custom
PhysicsMaterial custom(0.5f, 0.3f, 1.0f);  // friction, restitution, density
ball->setMaterial(custom);
```

### Collision Callbacks

```cpp
physics->setCollisionEnterCallback([](const CollisionInfo& info) {
    std::cout << "Collision detected!" << std::endl;
    std::cout << "Normal: " << info.normal.x << ", " << info.normal.y << std::endl;
});
```

---

## Material Presets

| Preset | Friction | Restitution | Density | Use Case |
|--------|----------|-------------|---------|----------|
| `Default()` | 0.3 | 0.0 | 1.0 | General purpose |
| `Bouncy()` | 0.3 | 0.8 | 1.0 | Rubber balls |
| `SuperBouncy()` | 0.2 | 0.95 | 0.5 | Bouncy balls |
| `Ice()` | 0.03 | 0.1 | 0.9 | Slippery surfaces |
| `Rubber()` | 0.8 | 0.7 | 1.2 | High grip + bounce |
| `Wood()` | 0.4 | 0.3 | 0.6 | Wooden objects |
| `Metal()` | 0.2 | 0.4 | 7.8 | Heavy metal |
| `Stone()` | 0.6 | 0.1 | 2.5 | Heavy, low bounce |

---

## Under The Hood (Advanced)

### Architecture

```
Physics2D/
├── PhysicsWorld.h          - Main API (what users interact with)
├── Rigidbody.h             - Physics body with easy API
├── Collider.h              - Shape classes (Circle, Box)
├── PhysicsMaterial.h       - Material presets
└── Internal/               - Powerful engine internals
    ├── PhysicsTypes.h      - Core structs
    ├── CollisionDetection.h - All collision algorithms
    ├── ImpulseResolver.h   - Collision response + Baumgarte
    └── Integration.h       - Semi-implicit Euler (future)
```

### Expert Techniques Used

1. **Fixed Timestep with Accumulator** (Glenn Fiedler)
   - Physics runs at 60 Hz regardless of frame rate
   - Prevents "spiral of death"
   - Smooth interpolation for rendering

2. **Semi-Implicit Euler Integration** (Erin Catto)
   - Update velocity FIRST, then position
   - Stable for spring systems
   - Energy-preserving (symplectic)

3. **Impulse-Based Resolution** (Erin Catto)
   - Instant velocity changes
   - No time-step dependency
   - Clean collision response

4. **Baumgarte Stabilization** (Expert consensus)
   - Positional correction with slop (0.01)
   - 20% correction per frame
   - Prevents jitter and sinking

5. **Store Inverse Mass** (Optimization)
   - Pre-compute 1/mass once
   - Used in every physics calculation

6. **Net Force as Running Total** (Winter.dev)
   - Single vector, not array
   - Automatic cleanup each frame
   - Better performance

7. **Minimum Material Values** (Envato Tuts+)
   - Use min(restitution) between objects
   - More realistic interactions

---

## Performance

- **O(n²) collision detection** (brute force)
- **Future optimization**: Spatial hashing / broad phase
- **60 Hz physics update** (configurable)
- **Lightweight**: No external physics libraries

---

## Future Features

- [ ] Spatial partitioning (grid/quadtree)
- [ ] Continuous collision detection (CCD)
- [ ] Joints and constraints
- [ ] Polygon colliders
- [ ] Sleeping/awake states
- [ ] Raycasting
- [ ] Triggers and sensors
- [ ] Swappable backends (Box2D wrapper)

---

## References & Credits

Built using expert knowledge from:
- **Erin Catto** - Box2D creator, sequential impulse solver
- **Glenn Fiedler** - "Fix Your Timestep", integration methods
- **Allen Chou** - Sequential impulse tutorials
- **Winter.dev** - Physics engine design philosophy
- **Envato Tuts+** - 2D physics engine tutorial

---

## Example: Pong with Physics

```cpp
PhysicsWorld* physics = new PhysicsWorld();

// Ball
Rigidbody* ball = physics->createCircle(400, 300, 8, PhysicsMaterial::Bouncy());

// Paddles (kinematic - moved by player, not physics)
Rigidbody* leftPaddle = physics->createBox(30, 300, 15, 100);
leftPaddle->makeKinematic();

// Walls (static)
Rigidbody* topWall = physics->createBox(400, 0, 800, 10);
topWall->makeStatic();

// Game loop
void update(float deltaTime) {
    // Move paddle
    if (isKeyPressed(W)) {
        leftPaddle->setVelocity(0, -400);
    }

    // Update physics
    physics->update(deltaTime);
}
```

Simple, powerful, clean!

---

Made with ❤️ following best practices from physics engine experts.

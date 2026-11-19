# Bilge

A game engine I'm writing from scratch in C, directly on the Win32 API, with no third-party libraries. The goal is to understand every layer between the operating system and a running game.

> Work in progress. Bilge grew out of an earlier prototype.

## What works today

- **Engine / game split.** The engine builds as a DLL (`BilgeEngine`) and the game as an executable (`BilgeGame`). The game plugs into the engine through a `GameInterface` of function pointers: `initialize`, `update`, `render`, `shutdown`.
- **Platform layer.** Window creation, the message pump and the final GDI blit on Win32, behind a small platform API (`Platform/platform.h`). The renderer and logger still call Win32 directly, which is what the Linux port has to untangle.
- **Software renderer.** Draws background fills and rectangles into a CPU-side pixel buffer, which the platform layer copies to the window. The buffer is reallocated when the window is resized.
- **Application loop.** Delta-time updates. Subsystems start in a fixed order (logging → platform → ECS → renderer → game), and the game, renderer, platform and logger shut down in reverse.
- **Logger.** Six severity levels (FATAL to TRACE) with coloured console output. DEBUG and TRACE are compiled out of release builds.
- **Level generator (in progress).** Takes a hard-coded 7×7 example tile map and learns which tile types may sit next to each other in each direction, printing the rules at startup. These rules will feed a Wave Function Collapse generator.
- **Demo.** WASD moves a rectangle around the window.

## Roadmap

- Linux Port
- Entity Component System (only a stub so far)
- Engine-owned string and vector types to replace the remaining standard-library pieces
- Wave Function Collapse level generation from the learned rules

## Layout

```
Engine/
  Core/Application/   main loop and subsystem lifecycle
  Core/ECS/           entity component system (stub)
  Core/Logger/        levelled, coloured logging
  Core/Renderer/      software renderer (CPU pixel buffer)
  Platform/           platform API + Win32 window, message pump, GDI blit
  game_interface.h    the contract between the engine and the game
Game/
  game.cpp            the demo game, built against the engine DLL
  level_generator.*   learns tile adjacency rules from an example map
```

## License

Apache 2.0, see [LICENSE](LICENSE).

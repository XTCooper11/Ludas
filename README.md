# Ludas

**Ludas** is an early-stage, lightweight C++ game engine/framework built on **SDL3**.  
Its goal is to provide a clean, understandable foundation for learning game development without an engine while remaining practical enough to build real projects.

Ludas intentionally favors **clarity over abstraction**. If something happens, you should be able to trace it in the code without pulling your hair out.

---

## Project Status

⚠️ **Early Development**

Ludas is *stable* enough to provide complete projects. Many feautures are yet to come that will allow for more complex games.

---

## Goals

- Build a **small, readable engine core** instead of a bloated framework
- Keep the codebase **approachable for learning engine internals**
- Avoid overengineering and premature abstractions
- Provide a clear path from:
  - SDL-based rendering → native graphics APIs (OpenGL / Vulkan / Metal)
- Stay **C++ first**, with minimal dependencies

This is not trying to compete with Unity, Unreal, or Godot.  
It’s trying to teach you *why those engines exist*.

---

## Current Features

### Core
- SDL3 initialization and shutdown
- Configurable startup flags (Video, Audio, Input)
- Centralized engine startup (`StartLudas`)
- Global access to `SDL_Window` and `SDL_Renderer`

### Rendering
- SDL3 renderer backend selection:
  - `auto`
  - `opengl`
  - `metal` (macOS)
  - `vulkan`
  - `direct3d11` (Windows)
  - `software`
- Runtime query of the actual renderer selected by SDL
- Texture loading via **SDL3_image**
- Color modulation, rotation, scaling, and flipping

### Objects
- Simple `Object` class with:
  - Position, size, scale, rotation
  - Texture binding
  - Visibility and activity state
- Integrated rendering and update loop (`FullUpdate`)
- Debug state output for inspection

### Timing & Physics (Basic)
- High-resolution delta time using SDL performance counters
- Basic velocity-based movement
- Gravity simulation
- Simple force application

---

## Roadmap

- Audio Support
- More Robust Physics
- Simple Lighting Systems
- Animation Support
- UI Tools
- Simple ECS

Check the latest release for the "goals" of the next release.

---

## Philosophy

Ludas follows a few simple rules:

- **Simple > Clever**
- **Explicit > Magical**
- **Readable > Abstract**
- **You should know what the engine is doing**

If a feature makes the engine harder to understand, it probably doesn’t belong yet.

---

## How to Contribute

Contributions are welcome, but expectations are clear.

### You can help by:
- Improving code clarity
- Fixing bugs
- Refactoring without changing behavior
- Adding small, focused features
- Improving documentation

### Please don’t:
- Drop massive rewrites without discussion
- Add heavy dependencies
- Turn this into a “framework of frameworks”

If you’re unsure, open an issue or discussion first or DM me on Discord "XT Cooper" for large contributions

---

## Building & Usage

**Right now**, Ludas is distributed as a **single-header implementation**. (Things will change as the project goes on)
You include it, define the implementation once, and go.
SDL3 and SDL3-image are REQUIRED.

---

## License
This framework is licensed under the **BSD 3-Clause License**.

Copyright (c) 2026, XTCooper11. All rights reserved.

### Summary for Developers
* **Commercial Use:** You can use this framework to build and sell your own games.
* **Modifications:** You can change the framework code however you like.
* **Attribution:** You must keep the copyright notice in your source code and include it in your game's credits/documentation.
* **No Endorsement:** You cannot use the name "XTCooper11" to promote your game without prior written permission.
* **No Liability:** The software is provided "as is" without any warranty.

> See the [LICENSE](./LICENSE) file for the full legal text.
---

## Final Note

Ludas exists to hold your hand and allow you to fly alone, your choice.

# Ludas

![Logo with replaced A](https://github.com/XTCooper11/Ludas/blob/main/images/Ludas-replacedA.png)

## About

Ludas is a plug-and-play **3D game development library** written in pure Python. It focuses solely on 3D, making tasks like rendering, camera movement, and mesh handling easy to implement without needing a full game engine. Built on top of `ModernGL`, `Pyglet`, and `NumPy`, Ludas gives you direct access to the 3D pipeline with simple and clean Python syntax.

The name comes from a *Latin* word meaning "you play" — because all games begin with the player.

---

## Table of Contents:

1. [Features](#features)
2. [Roadmap](#roadmap)
3. [License](#license)

![Logo separated](https://github.com/XTCooper11/Ludas/blob/main/images/logo%20and%20text%20seperated.png)

---

## Features

### Graphics (3D Only)

Ludas is fully focused on 3D. There is no 2D API. Everything is built around 3D rendering using OpenGL:

* ✅ Easy window creation
* ✅ GLSL shader support
* ✅ 3D mesh loading (.obj)
* ✅ Perspective camera system
* 🚧 Lighting (basic and advanced)
* 🚧 Scene management (planned)

### Audio

* ✅ Basic audio via `Pyglet` for background music and sound effects
* 🚧 3D positional audio support (planned)

### Physics

* 🚧 Planned support for 3D physics using `PyBullet`, including:

  * Rigid body simulation
  * Basic collision detection

### Scripting

* ✅ Write your games entirely in Python
* ✅ No external editor required — bring your own IDE

---

## Roadmap

* [ ] Scene manager and entity system
* [ ] Lighting + shadows
* [ ] Animation system
* [ ] GUI overlay (optional)
* [ ] PyPI package
* [ ] Visual editor (long-term goal)

---

![1:1 Logo](https://github.com/XTCooper11/Ludas/blob/main/images/1%3A1%20Logo.png)

## License

Ludas is open-source under the [MIT license](https://github.com/XTCooper11/Ludas/blob/main/LICENSE).

Use it, modify it, and make great 3D games.

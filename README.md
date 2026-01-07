# Ludas Framework v1.0

Ludas handles the SDL3 boilerplate. Making games with a dream is hard; Ludas makes it easier by providing an extensive Object class and simplified physics so you can focus on what matters: your game.

## What's Inside the Zip?

* include/: The headers. Contains Ludas.h and the required SDL3 headers.
* x64/: The muscle. Contains the .lib files for the compiler and .dll files for the engine.

## How to add Ludas to your Project

In your IDE find where you can add your include folders

* MSVS (MSVC): Project >> Properties > C/C++

Add the include folder its self. This will position ludas.h in the correct spot with SDL3. Ludas v1\\include

Now attach the SDL3 dll's and lib's. These are in the x64 folder as they are to build with x64 for ARM and x86 please change these out [here](https://github.com/libsdl-org/SDL/releases/tag/release-3.4.0).

The x64 (or ARM, x86) need to be placed in the output folder (bin) where your finished C++ binary will go.

## Getting Started

See the Wiki for documentation, beginner guides, examples and how to check if its installed right


#pragma once
/*  ANYONE CHECKING THE SOURCE FOR INFO
	SEE THE DOCS ON GITHUB IN THE WIKI SECTION
	USE THE LINK: https://github.com/XTCooper11/Ludas/wiki
*/
#define SDL_MAIN_HANDLED
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#ifndef LUDAS_H
#define LUDAS_H
extern SDL_Window* window;
extern SDL_Renderer* renderer;

#endif 
#ifdef LUDAS_INSTANCE

//created once when LUDAS_INSTANCE is defined.
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
#endif
void LudasOUT(std::string message) {
	std::cout << message << std::endl;
}
float GetDeltaTime() {
	static Uint64 lastTime = SDL_GetPerformanceCounter();
	Uint64 currentTime = SDL_GetPerformanceCounter();

	// Calculate seconds passed since last call
	float deltaTime = (float)(currentTime - lastTime) / (float)SDL_GetPerformanceFrequency();
	lastTime = currentTime;

	//cap deltaTime
	if (deltaTime > 0.1f) deltaTime = 0.1f;

	return deltaTime;
}
class Object {
public:
	//positions and stuff
	float xcord = 0;
	float ycord = 0;
	float w = 50;
	float h = 50;
	float scale = 1;
	float angle = 0;
	// how it will look
	SDL_Texture* texture = NULL;
	int color[4] = { 255,255,255,255 };
	SDL_FlipMode flip = SDL_FLIP_NONE;

	// State
	bool isActive = true;
	bool affectPhysics = false;
	//Physics
	float xvel = 0;     // Velocity
	float yvel = 0;
	float gravity = -9.8f;  // Downward force

	std::string GetCurrentState() {
		std::stringstream ss;

		// Set float precision to 2 decimal places for cleaner reading
		ss << std::fixed << std::setprecision(2);

		ss << "[ Debug Info ]\n\n";

		ss << "[ TRANSFORM ]\n";
		ss << "Pos: " << xcord << ", " << ycord << "\n";
		ss << "Dim: " << w << " x " << h << " (Scale: " << scale << ")\n";
		ss << "Rot: " << angle << " deg\n\n";

		ss << "[ APPEARANCE ]\n";
		ss << "Color: RGBA(" << color[0] << "," << color[1] << "," << color[2] << "," << color[3] << ")\n";
		ss << "Flip: " << (int)flip << " | TexturePtr: " << texture << "\n\n";

		ss << "[ STATUS ]\n";
		ss << "Active: " << (isActive ? "YES" : "NO") << "\n";
		ss << "Physics: " << (affectPhysics ? "ENABLED" : "DISABLED") << "\n\n";

		ss << "[ PHYSICS ]\n";
		ss << "Vel: " << xvel << "x, " << yvel << "y\n";
		ss << "Grav: " << gravity << "\n";

		return ss.str();
	}
	void SetTexturePNG(SDL_Renderer* renderer, const char* file) {
		this->texture = IMG_LoadTexture(renderer, file);

		if (this->texture != NULL) {
			// Automatically update Object dimensions to match the PNG
			float width, height;
			SDL_GetTextureSize(this->texture, &width, &height);
			this->w = width;
			this->h = height;
		}
		else {
			SDL_Log("Failed to load: %s", SDL_GetError());
		}
	}
	void UpdateState(float deltaTime) {
		if (!isActive && !affectPhysics) return;

		//Apply Gravity
		yvel += gravity * deltaTime;

		//Apply Movement
		xcord += xvel * deltaTime;
		ycord += yvel * deltaTime;
	}
	void PushObject(float xforce, float yforce) {
		if (!isActive) return;

		// We add the force directly to the velocity
		xvel += xforce;
		yvel += yforce;
	}
	void Render(SDL_Renderer* renderer) {
		if (!isActive || !texture) return;

		SDL_FRect dest = { xcord, ycord, w * scale, h * scale };
		SDL_SetTextureColorMod(texture, color[0], color[1], color[2]);
		dest.x = this->xcord - (this->w / 2.0f);
		dest.y = this->ycord - (this->h / 2.0f);
		dest.w = this->w;
		dest.h = this->h;
		// Rotate the object
		SDL_RenderTextureRotated(renderer, texture, NULL, &dest, angle, NULL, flip);
	}
	void FullUpdate(SDL_Renderer* renderer) {
		float deltaTime = GetDeltaTime();

		//Physics
		UpdateState(deltaTime);

		//Rendering
		Render(renderer);
	}
};
enum LudasFlags {
	VIDEO = 0x01,
	AUDIO = 0x02,
	INPUT = 0x04,
	LUDAS_EVERYTHING = (VIDEO | AUDIO | INPUT)
};
bool StartLudas(const char* title, int w, int h, uint32_t flags, const char* APIChoice) {
	uint32_t sdlFlags = 0;

	// Map your flags to SDL3 flags
	if (flags & VIDEO) sdlFlags |= SDL_INIT_VIDEO;
	if (flags & AUDIO) sdlFlags |= SDL_INIT_AUDIO;
	if (flags & INPUT) sdlFlags |= SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK;

	if (!SDL_Init(sdlFlags)) {
		SDL_Log("SDL_Init Failed: %s", SDL_GetError());
		return false;
	}
	// Only create window/renderer if Video was requested
	if (flags & VIDEO) {
		window = SDL_CreateWindow(title, w, h, 0);
		if (!window) {
			LudasOUT(SDL_GetError());
			SDL_Quit();
			return false;
		}

		if (APIChoice == "auto") APIChoice = NULL;
		renderer = SDL_CreateRenderer(window, APIChoice); // No SDL_Renderer* here
		if (!renderer) {
			LudasOUT(SDL_GetError());
			SDL_DestroyWindow(window);
			SDL_Quit();
			return false;
		}
	
		LudasOUT(SDL_GetRendererName(renderer));
		LudasOUT("Renderer and Window Created!");

	}

	return true;
}
bool HasQuit(SDL_Event &event) {
	if (event.type == SDL_EVENT_QUIT) {
		return true;
	}
	else {
		return false;
	}
}
void CloseALL(SDL_Renderer* renderer, SDL_Window* window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
float GetCenterOf(SDL_Window* window, const char* axis) {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	// Using strcmp to compare the string content correctly
	if (std::strcmp(axis, "x") == 0 || std::strcmp(axis, "X") == 0) {
		return w / 2.0f;
	}
	else if (std::strcmp(axis, "y") == 0 || std::strcmp(axis, "Y") == 0) {
		return h / 2.0f;
	}
	else {
		return 0;
	}
}
void TerminalFPS() {
	static float accumulator = 0.0f;
	static int frameCount = 0;

	accumulator += GetDeltaTime();
	frameCount++;

	// Update every 0.5 seconds to keep the terminal readable
	if (accumulator >= 0.5f) {
		// Clear the screen first
#if defined(SDL_PLATFORM_WINDOWS)
		system("cls");
#else
		system("clear");
#endif
		float avgFPS = (float)frameCount / accumulator;
		// Format the string
		std::stringstream ss;
		ss << " DEBUG MONITOR" << "\n" << " FPS: " << std::fixed << std::setprecision(2) << avgFPS << "\n";
		LudasOUT(ss.str());

		// Reset counters
		accumulator = 0.0f;
		frameCount = 0;
	}
}
void CapTo240FPS() {
	static Uint64 nextFrameTime = 0;
	Uint64 now = SDL_GetTicksNS();

	if (nextFrameTime == 0) nextFrameTime = now;

	Uint64 frameBudget = 1000000000 / 240; // nanoseconds for 240 FPS
	nextFrameTime += frameBudget;

	if (nextFrameTime > now) {
		SDL_DelayPrecise(nextFrameTime - now);
	}
	else {
		// We are behind schedule, don't wait!
		nextFrameTime = now;
	}
}
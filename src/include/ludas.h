#pragma once
/*  ANYONE CHECKING THE SOURCE FOR INFO
	SEE THE DOCS ON GITHUB IN THE WIKI SECTION
	USE THE LINK: https://github.com/XTCooper11/Ludas/wiki
*/
// Dev to Do: Create CheckCollision, ResolveCollision, and SetCollisionBox functions
#define SDL_MAIN_HANDLED
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>
#ifndef LUDAS_H
#define LUDAS_H
struct Camera {
	float x = 0.0f;
	float y = 0.0f;
	float zoom = 1.0f;     // 1.0 = normal, >1 zoom in, <1 zoom out
	float smooth = 8.0f;
};

extern Camera mainCamera;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

#endif 
#ifdef LUDAS_INSTANCE

//created once when LUDAS_INSTANCE is defined.
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Camera mainCamera;
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
enum ColliderBarrier : uint8_t {
	MIN_X = 0,
	MAX_X = 1,
	MIN_Y = 2,
	MAX_Y = 3
};
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
	bool hasCollider = false;
	bool isStatic = false;
	//Physics
	float xvel = 0;     // Velocity
	float yvel = 0;
	float gravity = 98.f;  // Downward force
	float collider[4];


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
	void SetTexture(SDL_Renderer* renderer, const char* file) { // AVIF,BMP,GIF,ICO,JPG,JXL,LBM,PCX,PNG,PNM,QOI,SVG,TGA,TIFF,WEBP,XCF,XPM,XV
		if (renderer == NULL || file == NULL) {
			SDL_Log("SetTexture failed: Renderer or File path is NULL");
			return;
		}

		// Clean up
		if (this->texture != NULL) {
			SDL_DestroyTexture(this->texture);
			this->texture = NULL;
		}

		//Load the new texture Format is detected automatically
		this->texture = IMG_LoadTexture(renderer, file);

		// Update dimensions if successful
		if (this->texture != NULL) {
			float width, height;
			if (SDL_GetTextureSize(this->texture, &width, &height)) {
				this->w = width;
				this->h = height;
			}
		}
		else {
			SDL_Log("Failed to load texture from %s: %s", file, SDL_GetError());
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
	void Render(SDL_Renderer* renderer, const Camera& cam) {
		if (!isActive || !texture) return;

		int winW, winH;
		SDL_GetWindowSize(window, &winW, &winH);

		float halfW = winW * 0.5f;
		float halfH = winH * 0.5f;

		SDL_FRect dest;
		dest.w = w * scale * cam.zoom;
		dest.h = h * scale * cam.zoom;

		dest.x = ((xcord - cam.x) * cam.zoom) + halfW - dest.w * 0.5f;
		dest.y = ((ycord - cam.y) * cam.zoom) + halfH - dest.h * 0.5f;

		SDL_SetTextureColorMod(texture, color[0], color[1], color[2]);
		SDL_RenderTextureRotated(renderer, texture, NULL, &dest, angle, NULL, flip);
	}


	void FullUpdate(SDL_Renderer* renderer) {
		float deltaTime = GetDeltaTime();

		//Physics
		UpdateState(deltaTime);

		//Rendering
		Render(renderer, mainCamera);
	}
	void HandleWASD(float speed) {
		const bool* keys = SDL_GetKeyboardState(NULL);

		if (keys) {
			float dx = 0;
			float dy = 0;
			if (keys[SDL_SCANCODE_W]) dy -= 1;
			if (keys[SDL_SCANCODE_S]) dy += 1;
			if (keys[SDL_SCANCODE_A]) dx -= 1;
			if (keys[SDL_SCANCODE_D]) dx += 1;

			if (dx != 0 && dy != 0) {
				float diagonalScaling = 0.7071f;
				dx *= diagonalScaling;
				dy *= diagonalScaling;
			}
			xcord += dx * speed;
			ycord += dy * speed;
		}
	}
	void HandleArrows(float speed) {
		size_t numkeys; // SDL3 uses size_t instead of int
		const bool* keys = SDL_GetKeyboardState(NULL);

		if (keys) {
			float dx = 0;
			float dy = 0;

			if (keys[SDL_SCANCODE_UP])    dy -= 1.0f;
			if (keys[SDL_SCANCODE_DOWN])  dy += 1.0f;
			if (keys[SDL_SCANCODE_LEFT])  dx -= 1.0f;
			if (keys[SDL_SCANCODE_RIGHT]) dx += 1.0f;

			if (dx != 0 && dy != 0) {
				float diagonalScaling = 0.7071f;
				dx *= diagonalScaling;
				dy *= diagonalScaling;
			}
			xcord += dx * speed;
			ycord += dy * speed;
		}
	}
	void HandleALLInput(float speed) {
		HandleWASD(speed);
		HandleArrows(speed);
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
void CapTo120FPS() {
	static Uint64 nextFrameTime = 0;
	Uint64 now = SDL_GetTicksNS();

	if (nextFrameTime == 0) nextFrameTime = now;

	Uint64 frameBudget = 1000000000 / 120; // nanoseconds for 120 FPS
	nextFrameTime += frameBudget;

	if (nextFrameTime > now) {
		SDL_DelayPrecise(nextFrameTime - now);
	}
	else {
		// We are behind schedule, don't wait!
		nextFrameTime = now;
	}
}
void CapTo60FPS() {
	static Uint64 nextFrameTime = 0;
	Uint64 now = SDL_GetTicksNS();

	if (nextFrameTime == 0) nextFrameTime = now;

	Uint64 frameBudget = 1000000000 / 60; // nanoseconds for 60 FPS
	nextFrameTime += frameBudget;

	if (nextFrameTime > now) {
		SDL_DelayPrecise(nextFrameTime - now);
	}
	else {
		// We are behind schedule, don't wait!
		nextFrameTime = now;
	}
}
inline void UpdateCollider(Object& obj) {
	// half sizes
	float hw = obj.w * 0.5f;
	float hh = obj.h * 0.5f;

	// rectangle corners before rotation
	float cornersX[4] = { obj.xcord - hw, obj.xcord + hw, obj.xcord + hw, obj.xcord - hw };
	float cornersY[4] = { obj.ycord - hh, obj.ycord - hh, obj.ycord + hh, obj.ycord + hh };

	// rotate each corner around the object's center
	float rad = obj.angle * 3.14159265f / 180.0f;
	float s = sin(rad);
	float c = cos(rad);

	for (int i = 0; i < 4; i++) {
		// translate point to origin
		float px = cornersX[i] - obj.xcord;
		float py = cornersY[i] - obj.ycord;

		// rotate
		float xnew = px * c - py * s;
		float ynew = px * s + py * c;

		// translate back
		cornersX[i] = xnew + obj.xcord;
		cornersY[i] = ynew + obj.ycord;
	}

	// find min/max
	obj.collider[MIN_X] = cornersX[0];
	obj.collider[MAX_X] = cornersX[0];
	obj.collider[MIN_Y] = cornersY[0];
	obj.collider[MAX_Y] = cornersY[0];

	for (int i = 1; i < 4; i++) {
		if (cornersX[i] < obj.collider[MIN_X]) obj.collider[MIN_X] = cornersX[i];
		if (cornersX[i] > obj.collider[MAX_X]) obj.collider[MAX_X] = cornersX[i];
		if (cornersY[i] < obj.collider[MIN_Y]) obj.collider[MIN_Y] = cornersY[i];
		if (cornersY[i] > obj.collider[MAX_Y]) obj.collider[MAX_Y] = cornersY[i];
	}
}

inline bool IsCollidingWith(const Object& a, const Object& b) { // returns a bool if 2 objects are colliding so the user can use that info
	if (a.hasCollider == false or b.hasCollider == false) return false;
	return (
		a.collider[MIN_X] < b.collider[MAX_X] &&
		a.collider[MAX_X] > b.collider[MIN_X] &&
		a.collider[MIN_Y] < b.collider[MAX_Y] &&
		a.collider[MAX_Y] > b.collider[MIN_Y]
		);
}
inline void CheckFixColliding(Object& a, Object& b) {
	// Update colliders first
	UpdateCollider(a);
	UpdateCollider(b);

	if (!IsCollidingWith(a, b)) return;

	float overlapX = 0.0f;
	float overlapY = 0.0f;

	if (a.xcord < b.xcord)
		overlapX = a.collider[MAX_X] - b.collider[MIN_X];
	else
		overlapX = b.collider[MAX_X] - a.collider[MIN_X];

	if (a.ycord < b.ycord)
		overlapY = a.collider[MAX_Y] - b.collider[MIN_Y];
	else
		overlapY = b.collider[MAX_Y] - a.collider[MIN_Y];

	// Move along the axis with the smallest overlap
	if (overlapX < overlapY) {
		float move = overlapX;
		if (!a.isStatic && !b.isStatic) {
			move *= 0.5f;
			if (a.xcord < b.xcord) { a.xcord -= move; b.xcord += move; }
			else { a.xcord += move; b.xcord -= move; }
		}
		else if (!a.isStatic) {
			if (a.xcord < b.xcord) a.xcord -= move;
			else                    a.xcord += move;
		}
		else if (!b.isStatic) {
			if (a.xcord < b.xcord) b.xcord += move;
			else                    b.xcord -= move;
		}
		// if both static, do nothing
	}
	else {
		float move = overlapY;
		if (!a.isStatic && !b.isStatic) {
			move *= 0.5f;
			if (a.ycord < b.ycord) { a.ycord -= move; b.ycord += move; }
			else { a.ycord += move; b.ycord -= move; }
		}
		else if (!a.isStatic) {
			if (a.ycord < b.ycord) a.ycord -= move;
			else                    a.ycord += move;
		}
		else if (!b.isStatic) {
			if (a.ycord < b.ycord) b.ycord += move;
			else                    b.ycord -= move;
		}
	}

	// Update colliders again, just in case
	UpdateCollider(a);
	UpdateCollider(b);
}
// camera stuff
void CameraHandleWASD(Camera& cam, float speed) {
	int numkeys;
	const bool* keys = (const bool*)SDL_GetKeyboardState(&numkeys);

	if (keys) {
		if (keys[SDL_SCANCODE_W]) cam.y -= speed;
		if (keys[SDL_SCANCODE_S]) cam.y += speed;
		if (keys[SDL_SCANCODE_A]) cam.x -= speed;
		if (keys[SDL_SCANCODE_D]) cam.x += speed;
	}
}
void CameraFollowObject(Camera& cam, const Object& obj) {
	cam.x = obj.xcord;
	cam.y = obj.ycord;
}
inline void CameraFollowSmooth(Camera& cam, const Object& target) {
	float dt = GetDeltaTime();
	cam.x += (target.xcord - cam.x) * cam.smooth * dt;
	cam.y += (target.ycord - cam.y) * cam.smooth * dt;
}
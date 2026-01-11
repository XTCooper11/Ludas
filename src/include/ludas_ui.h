#pragma once
/*  ANYONE CHECKING THE SOURCE FOR INFO
    SEE THE DOCS ON GITHUB IN THE WIKI SECTION
    USE THE LINK: https://github.com/The-Ludas-Project/The-Ludas-Framework/wiki
*/
#include "ludas.h"
#include <algorithm>

class LudasButton {
public:
    SDL_FRect rect;
    SDL_Color baseColor = { 60, 60, 60, 255 };
    SDL_Color hoverColor = { 90, 90, 90, 255 };
    SDL_Texture* textTex = nullptr;
    SDL_FRect textRect;
    bool isHovered = false;
    bool wasPressed = false;

    void Create(const char* label, float x, float y, float w, float h, TTF_Font* font) {
        rect = { x, y, w, h };
        if (textTex) SDL_DestroyTexture(textTex);
        SDL_Surface* surf = TTF_RenderText_Blended(font, label, 0, { 255, 255, 255, 255 });
        if (surf) {
            textTex = SDL_CreateTextureFromSurface(renderer, surf);
            textRect = { x + (w - surf->w) / 2.0f, y + (h - surf->h) / 2.0f, (float)surf->w, (float)surf->h };
            SDL_DestroySurface(surf);
        }
    }

    bool Clicked(SDL_Event& event) {
        float mx, my;
        SDL_GetMouseState(&mx, &my);

        SDL_FPoint mousePos = { mx, my };
        isHovered = SDL_PointInRectFloat(&mousePos, &rect);

        if (isHovered && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) wasPressed = true;
        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && wasPressed && isHovered) {
            wasPressed = false;
            return true;
        }
        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) wasPressed = false;
        return false;
    }

    void Render() {
        SDL_Color c = isHovered ? hoverColor : baseColor;
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(renderer, &rect);
        if (textTex) SDL_RenderTexture(renderer, textTex, NULL, &textRect);
    }
    ~LudasButton() { if (textTex) SDL_DestroyTexture(textTex); }
};

class LudasProgressBar {
public:
    SDL_FRect rect;
    SDL_Color bgColor = { 30, 30, 30, 255 };
    SDL_Color barColor = { 0, 200, 0, 255 };
    float progress = 0.5f; // 0.0 to 1.0

    void Create(float x, float y, float w, float h, SDL_Color color) {
        rect = { x, y, w, h };
        barColor = color;
    }

    void SetValue(float val) { progress = std::clamp(val, 0.0f, 1.0f); }

    void Render() {
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(renderer, &rect);

        SDL_FRect fill = { rect.x, rect.y, rect.w * progress, rect.h };
        SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
        SDL_RenderFillRect(renderer, &fill);
    }
};
class LudasTextArea {
public:
    SDL_Texture* tex = nullptr;
    SDL_FRect rect;

    void Create(const char* text, float x, float y, TTF_Font* font, SDL_Color color = { 255,255,255,255 }) {
        if (tex) SDL_DestroyTexture(tex);

        // Render the static text to a texture once
        SDL_Surface* surf = TTF_RenderText_Blended(font, text, 0, color);
        if (surf) {
            tex = SDL_CreateTextureFromSurface(renderer, surf);
            rect = { x, y, (float)surf->w, (float)surf->h };
            SDL_DestroySurface(surf);
        }
    }

    void Render() {
        if (tex) SDL_RenderTexture(renderer, tex, NULL, &rect);
    }

    ~LudasTextArea() { if (tex) SDL_DestroyTexture(tex); }
};

class LudasInputArea {
public:
    SDL_FRect rect;
    std::string text = "";
    SDL_Texture* textTex = nullptr;
    SDL_FRect textRect;
    TTF_Font* fontRef = nullptr;
    bool active = false;

    void Create(float x, float y, float w, float h, TTF_Font* font) {
        rect = { x, y, w, h };
        fontRef = font;
    }

    void Update(SDL_Event& event) {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            float mx, my;
            SDL_GetMouseState(&mx, &my);
            SDL_FPoint mousePos = { mx, my };

            bool wasActive = active;
            active = SDL_PointInRectFloat(&mousePos, &rect);

            if (active && !wasActive) {
                SDL_StartTextInput(window); // Turn on the keyboard buffer
            }
            else if (!active && wasActive) {
                SDL_StopTextInput(window);  // Turn it off to save resources
            }
        }

        // Handle Typing
        if (active) {
            if (event.type == SDL_EVENT_TEXT_INPUT) {
                text += event.text.text;
                Refresh();
            }
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_BACKSPACE && !text.empty()) {
                    text.pop_back();
                    Refresh();
                }
                if (event.key.key == SDLK_RETURN || event.key.key == SDLK_KP_ENTER) {
                    active = false;
                    SDL_StopTextInput(window);
                }
            }
        }
    }

    void Refresh() {
        if (textTex) {
            SDL_DestroyTexture(textTex);
            textTex = nullptr;
        }

        if (text.empty()) return;

        SDL_Surface* surf = TTF_RenderText_Blended(fontRef, text.c_str(), 0, { 255, 255, 255, 255 });
        if (surf) {
            textTex = SDL_CreateTextureFromSurface(renderer, surf);

            float tw = (float)surf->w;
            float th = (float)surf->h;

            // If text is wider than box, we shift it left
            float padding = 10.0f;
            float availableWidth = rect.w - (padding * 2);

            float xOffset = 0;
            if (tw > availableWidth) {
                xOffset = availableWidth - tw; // Scroll to the end
            }

            textRect = { rect.x + padding + xOffset, rect.y + (rect.h - th) / 2.0f, tw, th };
            SDL_DestroySurface(surf);
        }
    }

    void Render() {
        // Background and Border
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, active ? 0 : 100, 200, 255, 255);
        SDL_RenderRect(renderer, &rect);

        // Clipping
        SDL_Rect clip = { (int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h };
        SDL_SetRenderClipRect(renderer, &clip);

        // Text
        if (textTex && !text.empty()) {
            SDL_RenderTexture(renderer, textTex, NULL, &textRect);
        }

        // blinking cursor
        if (active) {
            if ((SDL_GetTicks() / 500) % 2 == 0) {
                float cursorX = text.empty() ? rect.x + 10 : textRect.x + textRect.w + 2;
                if (cursorX < rect.x + rect.w - 5) {
                    SDL_FRect cursorRect = { cursorX, rect.y + 5, 2, rect.h - 10 };
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &cursorRect);
                }
            }
        }

        SDL_SetRenderClipRect(renderer, NULL);
    }

    ~LudasInputArea() { if (textTex) SDL_DestroyTexture(textTex); }
};

class LudasSlider {
public:
    SDL_FRect rect;
    SDL_FRect handle;
    SDL_Color bgColor = { 40, 40, 40, 255 };
    SDL_Color barColor = { 100, 100, 100, 255 };
    SDL_Color handleColor = { 0, 150, 255, 255 };

    float value = 0.5f; // 0.0 to 1.0
    bool isDragging = false;

    void Create(float x, float y, float w, float h, float startValue = 0.5f) {
        rect = { x, y, w, h };
        value = std::clamp(startValue, 0.0f, 1.0f);
        UpdateHandle();
    }

    void Update(SDL_Event& event) {
        float mx, my;
        uint32_t mouseState = SDL_GetMouseState(&mx, &my);
        SDL_FPoint mPos = { mx, my };

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (SDL_PointInRectFloat(&mPos, &rect)) {
                isDragging = true;
            }
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            isDragging = false;
        }

        if (isDragging) {
            // Calculate value
            value = (mx - rect.x) / rect.w;
            value = std::clamp(value, 0.0f, 1.0f);
            UpdateHandle();
        }
    }

    void UpdateHandle() {
        float handleW = 15.0f;
        // Center the handle on the current value
        handle = { rect.x + (rect.w * value) - (handleW / 2.0f), rect.y - 5, handleW, rect.h + 10 };
    }

    void Render() {
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(renderer, &rect);

        SDL_FRect fill = { rect.x, rect.y, rect.w * value, rect.h };
        SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
        SDL_RenderFillRect(renderer, &fill);

        SDL_SetRenderDrawColor(renderer, handleColor.r, handleColor.g, handleColor.b, handleColor.a);
        SDL_RenderFillRect(renderer, &handle);
    }
};
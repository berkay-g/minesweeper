#pragma once

#define SDL_MAIN_HANDLED
#include "SDL3/SDL.h"

// #define IMGUI

#ifdef IMGUI
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#endif

#include "Color.h"
#include "util.h"

#include <string>

class App
{
public:
    App(const char *window_title, int window_width, int window_height, Uint32 window_flags = SDL_WINDOW_RESIZABLE, Uint32 renderer_flags = SDL_RENDERER_ACCELERATED, Uint32 sdl_init_flags = SDL_INIT_VIDEO);
    ~App();

#ifdef IMGUI
    ImGuiIO &ImguiInit();
    ImGuiIO &ImguiNewFrame();
#endif

    void ImguiRender();

    int SetWindowMinimumSize(int min_width, int min_height);
    void SetWindowWidthHeight(int w, int h);

    const int GetWindowWidth() { return window_width; }
    const int GetWindowHeight() { return window_height; }
    void GetScaleFactors(float &x, float &y, float &factor);

    SDL_Window *GetSDLWindow() { return window; }
    SDL_Renderer *GetSDLRenderer() { return renderer; }

    int SetRenderDrawColor(Color color);

    const bool GetIsMouseHidden() { return isMouseHidden; }
    void HideMouseCursor();
    void ShowMouseCursor();

    void DrawString(const std::string &str, const SDL_FRect &dimensions);
    void SetStringTextureColorMode(const Color& color);
#ifdef IMGUI
    void DrawString(const std::string &str, const ImVec2 &position, float size, const ImVec4 &color, int discriminator);
#endif

    void Setup();
    void Update(SDL_Event &event, bool &quit, float deltaTime);
    void Draw();

    const bool IsMouseInsideRect(float mouseX, float mouseY, const SDL_FRect &rect);

private:
    SDL_Window *window{};
    SDL_Renderer *renderer{};
    const char *window_title;
    int initial_window_width, initial_window_height;
    int window_width, window_height;
    float scale_factor_x{}, scale_factor_y{}, scale_factor{};
    bool isMouseHidden;

    SDL_Texture *font;

public:
    struct Texture
    {
        SDL_Texture *texture;
        Texture(const char* filename, SDL_Renderer* renderer, SDL_PixelFormatEnum pixelFormat = SDL_PIXELFORMAT_ARGB8888);
        Texture(const unsigned char* imageData, int imageDataSize, SDL_Renderer* renderer, SDL_PixelFormatEnum pixelFormat = SDL_PIXELFORMAT_ARGB8888);
        ~Texture();
    };
};
#include "App.h"

#include "SDL2_framerate.h"

int EventFilter(void *userdata, SDL_Event *event)
{
    if (event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        // Handle window resize event here
        App *pThis = reinterpret_cast<App *>(userdata);
        pThis->SetWindowWidthHeight(event->window.data1, event->window.data2);
        pThis->Draw();

        // SDL_Log("Window resized: %dx%d\n", event->window.data1, event->window.data2);
    }

    return 1;
}

int main(int argc, char *argv[])
{
    App app("Minesweeper", 825, 525, SDL_INIT_VIDEO, 0, argv[1]);
    app.SetWindowMinimumSize(825, 525);
    SDL_SetEventFilter(EventFilter, &app);

    // ImGuiIO &io = app.ImguiInit();

    app.Setup();

    FPSmanager fps;
    SDL_initFramerate(&fps);
    SDL_setFramerate(&fps, 60);

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    float deltaTime = 0;

    SDL_Event event;
    while (!app.quit)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = ((NOW - LAST) / (float)SDL_GetPerformanceFrequency());

        app.Update(event, deltaTime);
        app.Draw();
        SDL_framerateDelay(&fps);
    }

    return 0;
}
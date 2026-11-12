#include <SDL3/SDL.h>
#include <iostream>

#include "display.h"

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        std::cerr << "Falha ao inicializar SDL3: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "CHIP-8 Emulador",
        640,
        320,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::cerr << "Falha ao criar janela SDL3: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cerr << "Falha ao criar renderer SDL3: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Display display;

    bool running = true;
    SDL_Event event;
    const Uint32 ms_per_frame = 16; // ~60 Hz
    Uint32 last_tick = SDL_GetTicks();
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        display.renderDisplay(renderer);

        Uint32 current_tick = SDL_GetTicks();
        Uint32 elapsed = current_tick - last_tick;
        if (elapsed < ms_per_frame) {
            SDL_Delay(ms_per_frame - elapsed);
        }
        last_tick = SDL_GetTicks();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
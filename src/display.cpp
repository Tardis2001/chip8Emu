#include "display.h"

Display::Display() {
    clearDisplay();
}

void Display::clearDisplay() {
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        pixels[i] = false;
    }
}

void Display::renderDisplay(SDL_Renderer* renderer) {
    if (!renderer) {
        return;
    }

    // Fundo preto (todos os pixels desligados)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Desenha pixels ligados em branco
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (pixels[y * WIDTH + x]) {
                // Cada pixel do CHIP-8 é renderizado como um quadrado de escala 10×
                SDL_FRect rect = {
                    static_cast<float>(x * 10),
                    static_cast<float>(y * 10),
                    10.0f,
                    10.0f
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

bool Display::getPixel(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return false;
    }
    return pixels[y * WIDTH + x];
}

void Display::setPixel(int x, int y, bool on) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return;
    }
    pixels[y * WIDTH + x] = on;
}

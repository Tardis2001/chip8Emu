#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include <SDL3/SDL.h>

// Display do CHIP-8: 64×32 pixels monocromáticos.
// true  = pixel ligado (branco)
// false = pixel desligado (preto)
class Display {
public:
    static constexpr int WIDTH  = 64;
    static constexpr int HEIGHT = 32;

    Display();

    // Coloca todos os pixels como desligados (preto)
    void clearDisplay();

    // Desenha cada pixel do buffer na janela SDL_Renderer
    void renderDisplay(SDL_Renderer* renderer);

    // Acessa o estado de um pixel (para uso interno/testes)
    bool getPixel(int x, int y) const;

    // Define o estado de um pixel
    void setPixel(int x, int y, bool on);

private:
    bool pixels[WIDTH * HEIGHT];
};

#endif // CHIP8_DISPLAY_H

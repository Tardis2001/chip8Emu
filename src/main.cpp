#include <SDL3/SDL.h>
#include <iostream>

#include "display.h"
#include "memory.h"
#include "rom-loader.h"
#include "cpu.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: chip8 <caminho-da-rom.ch8>" << std::endl;
        return 1;
    }

    const std::string romPath = argv[1];

    // Inicializa a memória do emulador (inclui o conjunto de fontes embutido)
    chip8::Memory memory;

    // Carrega a ROM do disco
    chip8::RomLoader romLoader;
    std::vector<uint8_t> romData;
    if (!romLoader.loadRom(romPath, romData)) {
        std::cerr << "Falha ao carregar ROM: " << romPath << std::endl;
        return 1;
    }

    // Grava a ROM na memória a partir de 0x200
    if (!memory.loadRom(romData.data(), static_cast<int>(romData.size()))) {
        std::cerr << "Falha ao gravar ROM na memória: " << romPath << std::endl;
        return 1;
    }

    Display display;

    // Cria a CPU conectada à memória e ao display
    chip8::Cpu cpu(memory, display);

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

        // Executa um ciclo de CPU por frame
        if (!cpu.runCycle()) {
            std::cerr << "Erro ao executar ciclo de CPU" << std::endl;
            running = false;
        }

        // Renderiza o display sempre (o draw flag indica que algo mudou)
        display.renderDisplay(renderer);

        // Limpa o draw flag apos a renderizacao
        cpu.clearDrawFlag();

        Uint32 current_tick = SDL_GetTicks();
        Uint32 elapsed = current_tick - last_tick;
        if (elapsed < ms_per_frame) {
            SDL_Delay(ms_per_frame - elapsed);
        }
        last_tick = current_tick;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

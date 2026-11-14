#include <SDL3/SDL.h>
#include <atomic>
#include <cmath>
#include <iostream>
#include <vector>

#include "display.h"
#include "memory.h"
#include "rom-loader.h"
#include "cpu.h"

namespace {
// Dados compartilhados entre a thread principal e a callback de áudio.
struct AudioData {
    std::atomic<bool> soundActive{false};
    double phase = 0.0;
};

// Mapeia um scancode SDL para o indice CHIP-8 (0x0-0xF). Retorna -1 se nao mapeado.
int scancodeToChip8Key(SDL_Scancode sc) {
    switch (sc) {
        case SDL_SCANCODE_1: return 0x1;
        case SDL_SCANCODE_2: return 0x2;
        case SDL_SCANCODE_3: return 0x3;
        case SDL_SCANCODE_4: return 0xC;
        case SDL_SCANCODE_Q: return 0x4;
        case SDL_SCANCODE_W: return 0x5;
        case SDL_SCANCODE_E: return 0x6;
        case SDL_SCANCODE_R: return 0xD;
        case SDL_SCANCODE_A: return 0x7;
        case SDL_SCANCODE_S: return 0x8;
        case SDL_SCANCODE_D: return 0x9;
        case SDL_SCANCODE_F: return 0xE;
        case SDL_SCANCODE_Z: return 0xA;
        case SDL_SCANCODE_X: return 0x0;
        case SDL_SCANCODE_C: return 0xB;
        case SDL_SCANCODE_V: return 0xF;
        default: return -1;
    }
}

// Callback de áudio: gera onda quadrada (440 Hz) enquanto soundActive for true,
// silêncio caso contrário. Chamada pela thread de áudio do SDL3.
void SDLCALL audioCallback(void* userdata, SDL_AudioStream* stream, int additional_amount, int /*total_amount*/) {
    AudioData* ad = static_cast<AudioData*>(userdata);
    if (additional_amount <= 0) {
        return;
    }

    const int numSamples = additional_amount / static_cast<int>(sizeof(int16_t));
    std::vector<int16_t> buffer(numSamples, 0);

    const bool active = ad->soundActive.load();
    const double phaseInc = 2.0 * 3.14159265358979323846 * 440.0 / 44100.0;

    for (int i = 0; i < numSamples; ++i) {
        if (active) {
            buffer[i] = (std::sin(ad->phase) >= 0.0) ? 8000 : -8000;
        } else {
            buffer[i] = 0;
        }
        ad->phase += phaseInc;
        if (ad->phase > 2.0 * 3.14159265358979323846) {
            ad->phase -= 2.0 * 3.14159265358979323846;
        }
    }

    SDL_PutAudioStreamData(stream, buffer.data(), additional_amount);
}
} // namespace

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
        std::cerr << "Falha ao gravar ROM na memoria: " << romPath << std::endl;
        return 1;
    }

    Display display;

    // Cria a CPU conectada à memória e ao display
    chip8::Cpu cpu(memory, display);

    // Inicializa SDL (vídeo + eventos + áudio)
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO)) {
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

    // Inicialização de áudio (degradação graciosa se indisponível)
    AudioData audioData;
    SDL_AudioStream* audioStream = nullptr;
    SDL_AudioSpec audioSpec{};
    audioSpec.format = SDL_AUDIO_S16;
    audioSpec.channels = 1;
    audioSpec.freq = 44100;

    audioStream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &audioSpec,
        audioCallback,
        &audioData
    );
    if (!audioStream) {
        std::cerr << "Aviso: nao foi possivel inicializar audio: " << SDL_GetError() << std::endl;
    } else {
        if (!SDL_ResumeAudioStreamDevice(audioStream)) {
            std::cerr << "Aviso: nao foi possivel retomar dispositivo de audio: " << SDL_GetError() << std::endl;
        }
    }

    bool running = true;
    SDL_Event event;
    const Uint32 ms_per_frame = 16; // ~60 Hz
    Uint32 last_tick = SDL_GetTicks();
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                int chip8Key = scancodeToChip8Key(event.key.scancode);
                if (chip8Key >= 0) {
                    cpu.setKey(static_cast<uint8_t>(chip8Key), event.type == SDL_EVENT_KEY_DOWN);
                }
            }
        }

        // Executa um ciclo de CPU por frame
        if (!cpu.runCycle()) {
            std::cerr << "Erro ao executar ciclo de CPU" << std::endl;
            running = false;
            continue;
        }

        // Atualiza o flag de som com base no sound timer atual
        audioData.soundActive.store(cpu.soundTimer > 0);

        // Decrementa os timers a ~60 Hz (uma vez por frame)
        cpu.tickTimers();

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

    if (audioStream) {
        SDL_DestroyAudioStream(audioStream);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
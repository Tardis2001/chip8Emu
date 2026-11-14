#include "cpu.h"
#include "display.h"

namespace chip8 {

Cpu::Cpu(Memory& mem, Display& disp) : memory(mem), display(disp) {
    reset();
}

void Cpu::reset() {
    // Zera todos os registradores V0–VF
    for (int reg = 0; reg < NUM_REGS; ++reg) {
        v[reg] = 0;
    }

    // Registrador indice I zera
    i = 0;

    // Timers iniciam em zero
    delayTimer = 0;
    soundTimer = 0;

    // Teclado inicia com todas as teclas soltas
    for (int k = 0; k < 16; ++k) {
        keys[k] = 0;
    }

    // Program counter inicia em 0x200 (inicio dos programas CHIP-8)
    pc = PC_START;

    // Pilha vazia: stack pointer aponta para o topo (posicao 0)
    for (int level = 0; level < STACK_SIZE; ++level) {
        stack[level] = 0;
    }
    sp = 0;

    // Draw flag inicia desligado
    drawFlag = false;
}

void Cpu::clearDrawFlag() {
    drawFlag = false;
}

void Cpu::tickTimers() {
    if (delayTimer > 0) {
        --delayTimer;
    }
    if (soundTimer > 0) {
        --soundTimer;
    }
}

void Cpu::setKey(uint8_t key, bool pressed) {
    if (key < 16) {
        keys[key] = pressed ? 1 : 0;
    }
}

bool Cpu::fetch(uint16_t& opcode) {
    uint8_t high = 0;
    uint8_t low = 0;

    // Lê os dois bytes do opcode no endereco atual do PC
    if (!memory.read(pc, high)) {
        return false;
    }
    if (!memory.read(static_cast<uint16_t>(pc + 1), low)) {
        return false;
    }

    opcode = (static_cast<uint16_t>(high) << 8) | static_cast<uint16_t>(low);
    return true;
}

bool Cpu::execute(uint16_t opcode) {
    uint16_t nnn = opcode & 0x0FFF;   // endereco de 12 bits
    uint8_t  x   = (opcode & 0x0F00) >> 8;   // registrador Vx
    uint8_t  y   = (opcode & 0x00F0) >> 4;   // registrador Vy
    uint8_t  n   = opcode & 0x000F;   // nibble inferior (altura do sprite)
    uint8_t  kk  = opcode & 0x00FF;   // byte imediato

    // Decodifica pelo primeiro nibble
    switch (opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00EE) {
                // 0x00EE - retornar de subrotina: desempilha PC
                if (sp == 0) {
                    return false; // pilha vazia, nada para retornar
                }
                --sp;
                pc = stack[sp];
                return true;
            }
            // Outros opcodes 0x0xxx nao implementados neste escopo
            pc += 2;
            return true;

        case 0x1000:
            // 0x1NNN - pular para endereco NNN
            pc = nnn;
            return true;

        case 0x2000:
            // 0x2NNN - chamar subrotina: empilha endereco de retorno (pc + 2), PC = NNN
            if (sp >= STACK_SIZE) {
                return false; // pilha cheia
            }
            stack[sp] = static_cast<uint16_t>(pc + 2);
            ++sp;
            pc = nnn;
            return true;

        case 0x6000:
            // 0x6XNN - Vx = NN
            v[x] = kk;
            pc += 2;
            return true;

        case 0x7000:
            // 0x7XNN - Vx += NN (modulo 256, sem carry para VF)
            v[x] = static_cast<uint8_t>(v[x] + kk);
            pc += 2;
            return true;

        case 0xA000:
            // 0xANNN - I = NNN
            i = nnn;
            pc += 2;
            return true;

        case 0xD000:
            // 0xDXYN - desenhar sprite de N bytes a partir de I em (Vx, Vy)
            {
                uint8_t height = n;
                bool collision = false;

                // Captura as coordenadas antes de qualquer alteracao em VF
                uint8_t coordX = v[x];
                uint8_t coordY = v[y];

                for (uint8_t row = 0; row < height; ++row) {
                    uint8_t spriteByte = 0;
                    if (!memory.read(static_cast<uint16_t>(i + row), spriteByte)) {
                        return false;
                    }

                    for (uint8_t col = 0; col < 8; ++col) {
                        bool spritePixel = (spriteByte & (0x80 >> col)) != 0;
                        if (!spritePixel) {
                            continue;
                        }

                        int screenX = (coordX + col) % 64;
                        int screenY = (coordY + row) % 32;

                        bool current = display.getPixel(screenX, screenY);
                        if (current) {
                            collision = true;
                        }
                        display.setPixel(screenX, screenY, current ^ spritePixel);
                    }
                }

                // VF = 1 se houve colisao, 0 caso contrario
                v[0xF] = collision ? 1 : 0;

                // Draw flag: sempre verdadeiro apos um opcode de desenho,
                // para que o loop de renderizacao redesenhe o display.
                drawFlag = true;

                pc += 2;
                return true;
            }

        case 0xE000:
            // 0xEX** - opcodes de teclado
            switch (kk) {
                case 0x9E:
                    // 0xEX9E - pular se tecla Vx pressionada
                    if (v[x] < 16 && keys[v[x]]) {
                        pc += 4; // avanca 2 instrucoes alem do normal
                    } else {
                        pc += 2;
                    }
                    return true;
                case 0xA1:
                    // 0xEXA1 - pular se tecla Vx nao pressionada
                    if (v[x] < 16 && !keys[v[x]]) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    return true;
                default:
                    // Outros sub-opcodes 0xEX* nao implementados
                    pc += 2;
                    return true;
            }

        case 0xF000:
            // 0xFX** - opcodes de timer (e outros 0xFX* nao implementados)
            switch (kk) {
                case 0x07:
                    // 0xFX07 - Vx = delay timer
                    v[x] = delayTimer;
                    pc += 2;
                    return true;
                case 0x15:
                    // 0xFX15 - delay timer = Vx
                    delayTimer = v[x];
                    pc += 2;
                    return true;
                case 0x18:
                    // 0xFX18 - sound timer = Vx
                    soundTimer = v[x];
                    pc += 2;
                    return true;
                case 0x0A:
                    // 0xFX0A - aguardar tecla: bloqueia ate uma tecla ser pressionada
                    {
                        bool keyPressed = false;
                        uint8_t pressedKey = 0;
                        for (int k = 0; k < 16; ++k) {
                            if (keys[k]) {
                                keyPressed = true;
                                pressedKey = static_cast<uint8_t>(k);
                                break;
                            }
                        }
                        if (!keyPressed) {
                            // Nenhuma tecla pressionada: nao avanca o PC (bloqueia)
                            return true;
                        }
                        v[x] = pressedKey;
                        pc += 2;
                        return true;
                    }
                default:
                    // Outros sub-opcodes 0xFX* nao implementados neste escopo
                    pc += 2;
                    return true;
            }

        default:
            // Opcodes nao implementados: ignorar e continuar
            pc += 2;
            return true;
    }
}

bool Cpu::runCycle() {
    uint16_t opcode = 0;

    // 1. Busca: ler opcode da memoria no endereco do PC
    if (!fetch(opcode)) {
        return false;
    }

    // 2. Decodificacao e execucao (execute() gerencia o PC)
    if (!execute(opcode)) {
        return false;
    }

    return true;
}

} // namespace chip8

#include "cpu.h"
#include "display.h"

#include <cstdlib>

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
            if (opcode == 0x00E0) {
                // 0x00E0 - limpar display
                display.clearDisplay();
                pc += 2;
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

        case 0x3000:
            // 0x3XNN - pular se Vx == NN
            if (v[x] == kk) {
                pc += 4;
            } else {
                pc += 2;
            }
            return true;

        case 0x4000:
            // 0x4XNN - pular se Vx != NN
            if (v[x] != kk) {
                pc += 4;
            } else {
                pc += 2;
            }
            return true;

        case 0x5000:
            // 0x5XY0 - pular se Vx == Vy (apenas quando n == 0)
            if (n == 0 && v[x] == v[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
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

        case 0x8000:
            // 0x8XY** - operacoes logicas/aritmeticas entre registradores
            switch (n) {
                case 0x0:
                    // 0x8XY0 - Vx = Vy
                    v[x] = v[y];
                    pc += 2;
                    return true;
                case 0x1:
                    // 0x8XY1 - Vx |= Vy (OR bitwise)
                    v[x] = static_cast<uint8_t>(v[x] | v[y]);
                    pc += 2;
                    return true;
                case 0x2:
                    // 0x8XY2 - Vx &= Vy (AND bitwise)
                    v[x] = static_cast<uint8_t>(v[x] & v[y]);
                    pc += 2;
                    return true;
                case 0x3:
                    // 0x8XY3 - Vx ^= Vy (XOR bitwise)
                    v[x] = static_cast<uint8_t>(v[x] ^ v[y]);
                    pc += 2;
                    return true;
                case 0x4:
                    // 0x8XY4 - Vx += Vy com carry para VF
                    {
                        uint16_t result = static_cast<uint16_t>(v[x]) + static_cast<uint16_t>(v[y]);
                        v[0xF] = (result > 0xFF) ? 1 : 0;
                        v[x] = static_cast<uint8_t>(result & 0xFF);
                        pc += 2;
                        return true;
                    }
                case 0x5:
                    // 0x8XY5 - Vx -= Vy com emprestimo para VF
                    {
                        v[0xF] = (v[x] >= v[y]) ? 1 : 0;
                        v[x] = static_cast<uint8_t>(v[x] - v[y]);
                        pc += 2;
                        return true;
                    }
                case 0x6:
                    // 0x8XY6 - Vx >>= 1, VF = bit menos significativo original de Vx
                    v[0xF] = v[x] & 0x1;
                    v[x] = static_cast<uint8_t>(v[x] >> 1);
                    pc += 2;
                    return true;
                case 0x7:
                    // 0x8XY7 - Vx = Vy - Vx com emprestimo para VF
                    {
                        v[0xF] = (v[y] >= v[x]) ? 1 : 0;
                        v[x] = static_cast<uint8_t>(v[y] - v[x]);
                        pc += 2;
                        return true;
                    }
                case 0xE:
                    // 0x8XYE - Vx <<= 1, VF = bit mais significativo original de Vx
                    v[0xF] = (v[x] & 0x80) ? 1 : 0;
                    v[x] = static_cast<uint8_t>(v[x] << 1);
                    pc += 2;
                    return true;
                default:
                    // Outros sub-opcodes 0x8XY* nao implementados
                    pc += 2;
                    return true;
            }

        case 0x9000:
            // 0x9XY0 - pular se Vx != Vy (apenas quando n == 0)
            if (n == 0 && v[x] != v[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
            return true;

        case 0xA000:
            // 0xANNN - I = NNN
            i = nnn;
            pc += 2;
            return true;

        case 0xB000:
            // 0xBNNN - pular para endereco NNN + V0
            pc = static_cast<uint16_t>(nnn + v[0]);
            return true;

        case 0xC000:
            // 0xCXNN - Vx = valor aleatorio & NN
            v[x] = static_cast<uint8_t>((std::rand() % 256) & kk);
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
                case 0x1E:
                    // 0xFX1E - I += Vx
                    i = static_cast<uint16_t>(i + v[x]);
                    pc += 2;
                    return true;
                case 0x29:
                    // 0xFX29 - I = endereco do sprite do digito Vx (cada sprite ocupa 5 bytes)
                    i = static_cast<uint16_t>(v[x] * 5);
                    pc += 2;
                    return true;
                case 0x33:
                    // 0xFX33 - armazenar BCD de Vx em I, I+1, I+2
                    {
                        uint8_t value = v[x];
                        memory.write(i, static_cast<uint8_t>(value / 100));
                        memory.write(static_cast<uint16_t>(i + 1), static_cast<uint8_t>((value / 10) % 10));
                        memory.write(static_cast<uint16_t>(i + 2), static_cast<uint8_t>(value % 10));
                        pc += 2;
                        return true;
                    }
                case 0x55:
                    // 0xFX55 - armazenar V0..Vx na memoria a partir de I
                    for (uint8_t k = 0; k <= x; ++k) {
                        memory.write(static_cast<uint16_t>(i + k), v[k]);
                    }
                    pc += 2;
                    return true;
                case 0x65:
                    // 0xFX65 - carregar V0..Vx da memoria a partir de I
                    for (uint8_t k = 0; k <= x; ++k) {
                        memory.read(static_cast<uint16_t>(i + k), v[k]);
                    }
                    pc += 2;
                    return true;
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

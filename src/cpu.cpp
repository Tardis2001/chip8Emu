#include "cpu.h"

namespace chip8 {

Cpu::Cpu(Memory& mem) : memory(mem) {
    reset();
}

void Cpu::reset() {
    // Zera todos os registradores V0–VF
    for (int reg = 0; reg < NUM_REGS; ++reg) {
        v[reg] = 0;
    }

    // Registrador indice I zera
    i = 0;

    // Program counter inicia em 0x200 (inicio dos programas CHIP-8)
    pc = PC_START;

    // Pilha vazia: stack pointer aponta para o topo (posicao 0)
    for (int level = 0; level < STACK_SIZE; ++level) {
        stack[level] = 0;
    }
    sp = 0;
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
    // Neste escopo, nenhum opcode e implementado ainda.
    // O ciclo de execucao e valido: busca e decodificacao funcionam,
    // mas a execucao de opcodes especificos fica para uma mudança posterior.
    (void)opcode;
    return true;
}

bool Cpu::runCycle() {
    uint16_t opcode = 0;

    // 1. Busca: ler opcode da memoria no endereco do PC
    if (!fetch(opcode)) {
        return false;
    }

    // 2. Decodificacao e execucao
    if (!execute(opcode)) {
        return false;
    }

    // 3. PC avanca 2 posicoes para o proximo opcode
    pc = static_cast<uint16_t>(pc + 2);

    return true;
}

} // namespace chip8

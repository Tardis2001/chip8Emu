#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include <cstdint>

#include "memory.h"

namespace chip8 {

// CPU do CHIP-8: registradores, estado de execucao e ciclo fetch-decode-execute.
// Integra-se com a camada de memoria ja existente (chip8-memory).
class Cpu {
public:
    static constexpr int NUM_REGS = 16;   // V0–VF
    static constexpr int STACK_SIZE = 16; // pilha de 16 niveis
    static constexpr uint16_t PC_START = 0x200;

    // Construtor recebe a referencia para a memoria do emulador
    Cpu(Memory& mem);

    // Registradores de proposito geral V0–VF
    uint8_t v[NUM_REGS];

    // Registrador indice I (endereço de 12 bits)
    uint16_t i;

    // Program counter: endereco da proxima instrucao
    uint16_t pc;

    // Pilha de endereços de retorno e stack pointer
    uint16_t stack[STACK_SIZE];
    uint8_t sp;

    // Referencia a memoria do emulador (fornecida externamente)
    Memory& memory;

    // Busca um opcode de 2 bytes no endereco atual do PC.
    // Retorna false se nao conseguir ler da memoria.
    bool fetch(uint16_t& opcode);

    // Decodifica e executa o opcode. Retorna true se executado com sucesso.
    bool execute(uint16_t opcode);

    // Executa um ciclo completo: busca, decodifica e executa.
    bool runCycle();

private:
    // Inicializa registradores, PC, pilha e stack pointer.
    void reset();
};

} // namespace chip8

#endif // CHIP8_CPU_H

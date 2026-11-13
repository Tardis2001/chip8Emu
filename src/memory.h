#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <cstdint>

namespace chip8 {

// Memória de 4 KB do CHIP-8 (0x000–0xFFF).
// Fornece leitura e escrita de byte com validação de endereço.
// O conjunto de fontes fica embutido em 0x000–0xFF;
// programas são carregados a partir de 0x200.
class Memory {
public:
    static constexpr int SIZE = 4096;          // 0x1000
    static constexpr int PROGRAM_START = 0x200;// onde ROMs são carregadas

    Memory();

    // Lê um byte do endereço. Retorna false se o endereço for inválido.
    bool read(uint16_t address, uint8_t& value) const;

    // Escreve um byte no endereço. Retorna false se o endereço for inválido.
    bool write(uint16_t address, uint8_t value);

    // Acesso direto sem validação (para uso interno da CPU).
    uint8_t& at(uint16_t address);
    const uint8_t& at(uint16_t address) const;

    // Carrega uma ROM a partir de 0x200. Retorna false se não couber.
    bool loadRom(const uint8_t* data, int size);

private:
    uint8_t memory[SIZE];
};

} // namespace chip8

#endif // CHIP8_MEMORY_H

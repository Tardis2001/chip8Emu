#include "memory.h"

#include <cstring>

namespace chip8 {

// Conjunto de fontes do CHIP-8: 16 dígitos (0–F), 5 bytes cada = 80 bytes.
// Cada fonte tem 5 px de altura. Carregado em 0x000–0xFF.
static constexpr uint8_t fontSet[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Memory::Memory() {
    std::memset(memory, 0, SIZE);
    // Copia o conjunto de fontes para 0x000–0xFF (80 bytes).
    std::memcpy(memory, fontSet, sizeof(fontSet));
}

bool Memory::read(uint16_t address, uint8_t& value) const {
    if (address >= SIZE) {
        return false;
    }
    value = memory[address];
    return true;
}

bool Memory::write(uint16_t address, uint8_t value) {
    if (address >= SIZE) {
        return false;
    }
    memory[address] = value;
    return true;
}

uint8_t& Memory::at(uint16_t address) {
    return memory[address];
}

const uint8_t& Memory::at(uint16_t address) const {
    return memory[address];
}

bool Memory::loadRom(const uint8_t* data, int size) {
    if (data == nullptr || size < 0) {
        return false;
    }
    int available = SIZE - PROGRAM_START;
    if (size > available) {
        return false;
    }
    std::memcpy(memory + PROGRAM_START, data, static_cast<size_t>(size));
    return true;
}

} // namespace chip8

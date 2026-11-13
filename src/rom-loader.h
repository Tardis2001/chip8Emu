#ifndef CHIP8_ROM_LOADER_H
#define CHIP8_ROM_LOADER_H

#include <cstdint>
#include <string>
#include <vector>

namespace chip8 {

// Carregador de ROM do CHIP-8.
// Le um arquivo .ch8 ou .c8 do disco e grava seu conteúdo na memória
// a partir do endereço 0x200, reutilizando a camada de memória existente.
class RomLoader {
public:
    // Carrega uma ROM a partir do caminho do arquivo.
    // Retorna true se o carregamento foi bem-sucedido, false em caso de erro.
    // O parâmetro outSize recebe o número de bytes lidos (ou 0 em caso de erro).
    bool loadRom(const std::string& path, std::vector<uint8_t>& outData);

private:
    // Lê todos os bytes de um arquivo. Retorna false se não conseguir abrir ou ler.
    bool readFile(const std::string& path, std::vector<uint8_t>& data) const;
};

} // namespace chip8

#endif // CHIP8_ROM_LOADER_H

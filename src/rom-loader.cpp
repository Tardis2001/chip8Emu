#include "rom-loader.h"

#include <fstream>
#include <iostream>

namespace chip8 {

bool RomLoader::readFile(const std::string& path, std::vector<uint8_t>& data) const {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Falha ao abrir arquivo ROM: " << path << std::endl;
        return false;
    }

    // Lê todo o conteúdo do arquivo
    data.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    if (file.bad()) {
        std::cerr << "Falha ao ler arquivo ROM: " << path << std::endl;
        return false;
    }

    return true;
}

bool RomLoader::loadRom(const std::string& path, std::vector<uint8_t>& outData) {
    if (path.empty()) {
        std::cerr << "Caminho de ROM vazio fornecido." << std::endl;
        return false;
    }

    if (!readFile(path, outData)) {
        return false;
    }

    if (outData.empty()) {
        std::cerr << "Arquivo ROM está vazio: " << path << std::endl;
        return false;
    }

    return true;
}

} // namespace chip8

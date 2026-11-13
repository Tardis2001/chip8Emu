# Proposal

## Why

O emulador CHIP-8 atualmente só possui a camada de vídeo (SDL3 + display 64×32). Sem memória, não há onde carregar ROMs, armazenar o conjunto de fontes ou manter o estado da CPU. Esta mudança introduz a memória do emulador para que, em etapas seguintes, a CPU possa buscar e executar opcodes.

## What Changes

- Nova memória de 4 KB (4096 bytes) do CHIP-8
- Conjunto de fontes embutido carregado nos endereços 0x000–0x1FF
- Carregador de ROM que copia o arquivo `.ch8`/`.c8` a partir do endereço 0x200
- Interface de leitura e escrita de byte para uso da CPU e de outros componentes

## Capabilities

### New Capabilities

- `chip8-memory`: memória de 4 KB, fontes embutidas em 0x000–0x1FF e carregamento de ROM a partir de 0x200; expõe operações de ler/escrever byte

### Modified Capabilities

(nenhum)

## Impact

- `src/` receberá novos arquivos de memória (ex.: `memory.h` / `memory.cpp`)
- `main.cpp` precisará aceitar o caminho da ROM via linha de comando em uma etapa posterior
- Nenhuma alteração na spec existente `sdl3-display`

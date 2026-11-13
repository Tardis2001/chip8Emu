# Proposal

## Why

O emulador CHIP-8 já possui memória e display, mas não executa nenhuma instrução. Sem registradores da CPU e um ciclo de busca-decode-execucao, nao eh possivel carregar e rodar nenhuma ROM. Esta mudança introduz a CPU como o nucleo executivo do emulador.

## What Changes

- Registradores da CPU: V0–VF, indice I, program counter PC, pilha (16 niveis) e stack pointer
- Ciclo fetch-decode-execucao de opcodes
- Suporte a main.cpp receber caminho da ROM via linha de comando (argv)
- Integracao da CPU com a memoria existente (chip8-memory) e o display existente (sdl3-display)

## Capabilities

### New Capabilities

- `cpu`: registradores, estado de execucao e ciclo fetch-decode-execucao de opcodes CHIP-8; expoe interface para ler/escrever registradores e executar um ciclo de CPU

### Modified Capabilities

(nenhum)

## Impact

- `src/` recebera novos arquivos de CPU (ex.: `cpu.h` / `cpu.cpp`)
- `main.cpp` precisara aceitar caminho da ROM via argv e acionar o ciclo de CPU
- A CPU dependera da capacidade `chip8-memory` para buscar opcodes e gravar resultados
- Nenhuma alteracao na spec existente `sdl3-display`

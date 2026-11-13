# Proposal

## Why

O emulador CHIP-8 já possui memória, display e CPU (registradores + ciclo fetch-decode-execute), mas não consegue carregar ROMs do disco nem executar instruções específicas. Sem suporte a caminho de ROM via linha de comando e sem opcodes implementados, nenhuma ROM pode ser executada de verdade. Esta mudança conecta a memória e a CPU ao mundo externo, permitindo carregar e executar programas CHIP-8.

## What Changes

- `main.cpp` aceita caminho de arquivo ROM (.ch8 ou .c8) via linha de comando (argv)
- Carregador de ROM que lê um arquivo do disco e grava seu conteúdo na memória a partir de 0x200
- Implementação de opcodes CHIP-8 básicos na CPU: set (0x6XNN), add (0x7XNN), set I (0xANNN), jump (0x1NNN), call/return (0x2NNN/0x00EE), desenho (0xDXYN)
- Draw flag para controlar quando o display deve ser redesenhado
- Integração da CPU com o display existente (sdl3-display) para renderização de sprites

## Capabilities

### New Capabilities

- `rom-loader`: leitura de arquivos ROM (.ch8/.c8) do disco e gravação na memória a partir de 0x200, integrada com a capacidade `chip8-memory`

### Modified Capabilities

- `cpu`: implementação de opcodes CHIP-8 específicos (set, add, set I, jump, call, return, desenho) e draw flag; expande o ciclo fetch-decode-execute existente com comportamento real por opcode

## Impact

- `main.cpp` será modificado para aceitar argv e carregar ROM antes de iniciar o loop de emulação
- `src/` receberá novos arquivos: carregador de ROM e implementação de opcodes
- `CMakeLists.txt` precisará incluir os novos arquivos-fonte
- A CPU dependerá de `chip8-memory` (já existente) para ler opcodes e gravar resultados
- O opcode de desenho (0xDXYN) dependerá de `sdl3-display` (já existente) para renderizar pixels
- Nenhuma alteração nas specs existentes `chip8-memory` ou `sdl3-display`

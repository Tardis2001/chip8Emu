# Proposal

## Why

O emulador ainda não captura entrada do usuário. Programas CHIP-8 que dependem de teclas (jogos como Pong e Tetris) não funcionam: o jogador não consegue controlar o jogo. Além disso, três opcodes que consultam o teclado (0xEX9E, 0xEXA1, 0xFX0A) atualmente caem no caso default (no-op), então a lógica de entrada dos programas é ignorada.

## What Changes

- **Nova capability `chip8-teclado`**: estado de 16 teclas (0x0–0xF), captura de eventos de teclado via SDL3, e mapeamento padrão CHIP-8 (teclas 1 2 3 4 / Q W E R / A S D F / Z X C V).
- **Capability `cpu` modificada**: adição dos requisitos para os opcodes 0xEX9E (pular se tecla pressionada), 0xEXA1 (pular se tecla não pressionada) e 0xFX0A (aguardar tecla e armazenar em Vx).
- **main.cpp**: processar eventos SDL de teclado (KEY_DOWN/KEY_UP), atualizar o estado das 16 teclas, e repassar à CPU. O opcode 0xFX0A bloqueia a execução até uma tecla ser pressionada.

## Capabilities

### New Capabilities

- `chip8-teclado`: estado e captura de entrada do teclado CHIP-8 (16 teclas, mapeamento padrão, consulta de estado).

### Modified Capabilities

- `cpu`: adição dos requisitos para os opcodes 0xEX9E, 0xEXA1 e 0xFX0A (consulta e espera de teclas).

## Impact

- **Código afetado**: `src/cpu.h`, `src/cpu.cpp` (estado de teclas e opcodes), `src/main.cpp` (eventos SDL de teclado).
- **Dependências**: SDL3 (já em uso para vídeo); nenhum novo pacote.
- **Fora de escopo**: mapeamento customizável de teclas, suporte a gamepad, e os demais opcodes CHIP-8 (~22 restantes) ficam para changes futuros.
# Proposal

## Why

O emulador CHIP-8 ja implementa 13 opcodes (basicos, timers, teclado). Faltam cerca de 22 opcodes do conjunto padrao CHIP-8, incluindo instrucoes logicas/aritmeticas (0x8XY*), pula condicional (0x3XNN, 0x4XNN, 0x5XY0, 0x9XY0), CLS (0x00E0), salto com deslocamento (0xBNNN), aleatorio (0xCXNN), e manipulacao de memoria/indice (0xFX1E, 0xFX29, 0xFX33, 0xFX55, 0xFX65). Sem eles, programas CHIP-8 mais complexos nao funcionam corretamente.

## What Changes

- **Nova capability `chip8-opcodes`**: requisitos para todos os opcodes CHIP-8 ainda nao implementados, agrupados por categoria (display, fluxo, logica/aritmetica, memoria/indice, aleatorio).
- **Nenhuma capacidade modificada**: os requisitos existentes em `cpu` permanecem inalterados; os novos requisitos ficam na nova capability `chip8-opcodes`.

## Capabilities

### New Capabilities

- `chip8-opcodes`: requisitos de comportamento para os opcodes CHIP-8 restantes (CLS, pula condicional, logica/aritmetica 0x8XY*, salto 0xBNNN, aleatorio 0xCXNN, memoria/indice 0xFX*).

### Modified Capabilities

- (nenhum)

## Impact

- **Codigo afetado**: `src/cpu.h`, `src/cpu.cpp` (novos cases no dispatch de `execute()`), possivelmente `src/display.h`/`src/display.cpp` (CLS).
- **Dependencias**: nenhuma nova; reutiliza `Memory`, `Display` e `keys[]` ja existentes.
- **Fora de escopo**: teclado ja feito, timers ja feitos, ajuste de velocidade de CPU (varios ciclos por frame) fica para change futuro.
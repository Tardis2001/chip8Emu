# Proposal

## Why

O emulador CHIP-8 ainda não possui sistema de temporização. Programas CHIP-8 usam o **delay timer** para controlar o ritmo de execução (animações, velocidade de jogo) e o **sound timer** para emitir um beep. Sem eles, jogos rodam em velocidade não controlada e não há feedback sonoro. Além disso, três opcodes que leem/escrevem esses timers (0xFX07, 0xFX15, 0xFX18) atualmente caem no caso default (no-op), então programas que os usam não funcionam corretamente.

## What Changes

- **Nova capability `chip8-timers`**: estado do delay timer e do sound timer (8 bits cada), decremento a 60 Hz até zero, e emissão de beep enquanto o sound timer for maior que zero.
- **Capability `cpu` modificada**: adição dos requisitos para os opcodes 0xFX07 (Vx = delay timer), 0xFX15 (delay timer = Vx) e 0xFX18 (sound timer = Vx).
- **main.cpp**: integração do decremento dos timers no loop de frames (~60 Hz) e inicialização de áudio (SDL) para o beep.

## Capabilities

### New Capabilities

- `chip8-timers`: estado e comportamento dos timers de delay e som do CHIP-8 (valores 8 bits, decremento a 60 Hz até zero, beep enquanto sound timer > 0).

### Modified Capabilities

- `cpu`: adição dos requisitos para os opcodes 0xFX07, 0xFX15 e 0xFX18 (leitura/escrita dos timers).

## Impact

- **Código afetado**: `src/cpu.h`, `src/cpu.cpp` (novos registradores de timer e opcodes), `src/main.cpp` (decremento a 60 Hz, inicialização de áudio).
- **Dependências**: SDL3 já está em uso para vídeo; áudio via `SDL_INIT_AUDIO` pode ser adicionado.
- **Fora de escopo**: teclado, sound timer além do beep básico, e os demais opcodes CHIP-8 (~25 restantes) ficam para changes futuros.
# Design

## Context

O emulador já possui CPU com registradores V0–VF, I, PC, pilha, fetch-decode-execute, e opcodes básicos (0x6XNN, 0x7XNN, 0xANNN, 0x1NNN, 0x2NNN, 0x00EE, 0xDXYN). O loop principal (`main.cpp`) roda a ~60 FPS executando um ciclo de CPU por frame e renderizando o display. Não existe estado de timer nem áudio. Ver `proposal.md` para motivação.

## Goals / Non-Goals

**Goals:**
- Adicionar estado de delay timer e sound timer (8 bits) à CPU.
- Decrementar ambos os timers a 60 Hz (uma vez por frame) até zero.
- Implementar opcodes 0xFX07, 0xFX15, 0xFX18.
- Emitir beep enquanto sound timer > 0 via SDL3 áudio.

**Non-Goals:**
- Teclado e opcodes de tecla (0xEX9E, 0xEXA1, 0xFX0A) — change futuro.
- Demais opcodes CHIP-8 (~25) — changes futuros.
- Ajuste de velocidade de CPU (ciclos por frame) — fora do escopo.
- Formas de onda/áudio avançado além do beep básico.

## Decisions

### 1. Estado dos timers na CPU

**Decisão:** Adicionar campos `uint8_t delayTimer` e `uint8_t soundTimer` à classe `Cpu`, públicos como `v[]`, junto com método `void tickTimers()`.

**Rationale:** Timers são parte do estado da CPU CHIP-8 e são acessados pelos opcodes; mantê-los na CPU é consistente com os registradores existentes e simplifica o acesso. Alternativa (classe separada de timer) rejeitada por adicionar indireção sem benefício neste escopo.

### 2. Decremento a 60 Hz

**Decisão:** Chamar `cpu.tickTimers()` uma vez por frame no loop principal, após executar o ciclo de CPU. Como o loop já visa ~60 FPS (16 ms/frame), isso corresponde ao decremento de 60 Hz.

**Rationale:** Reutiliza o timing existente do loop; evita thread/timer separado. Alternativa (timer SDL separado) rejeitada por complexidade desproporcional.

### 3. Áudio (beep)

**Decisão:** Inicializar SDL com `SDL_INIT_AUDIO`. Abrir um dispositivo de áudio (stream) com callback que gera onda quadrada (440 Hz) quando um flag atômico `soundActive` for verdadeiro, e silêncio caso contrário. O flag é atualizado a cada frame pelo loop principal com base em `cpu.soundTimer > 0`.

**Rationale:** Callback em thread de áudio evita latência; flag atômico resolve a comunicação main↔audio sem locks. Alternativa (SDL_QueueAudio manual) rejeitada por dificuldade de sincronizar play/stop com o timer.

### 4. Opcodes 0xFX*

**Decisão:** Adicionar cases `0xF000` no switch de `execute()`, decodificando o sub-opcode por `kk` (0x07, 0x15, 0x18). Outros sub-opcodes 0xFX* caem no default (no-op) por ora.

**Rationale:** Consistente com a estrutura existente de dispatch por nibble alto; mantém mudanças localizadas.

## Risks / Trade-offs

- [Som pode não estar disponível no ambiente] → Verificar retorno de `SDL_Init(SDL_INIT_AUDIO)`; se falhar, continuar sem áudio (degradação graciosa) e logar aviso.
- [Acesso concorrente a soundTimer] → Apenas o flag atômico é compartilhado; o timer em si é lido/escrito apenas na thread principal.
- [Um ciclo de CPU por frame é muito lento para CHIP-8 real] → Fora do escopo; timers ainda decrementam corretamente a 60 Hz.

## Migration Plan

N/A — adição de funcionalidade, sem quebra de compatibilidade. Build existente permanece válido.

## Open Questions

Nenhuma.
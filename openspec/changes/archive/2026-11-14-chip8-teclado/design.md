# Design

## Context

O emulador possui CPU com registradores, timers (delay/sound), e opcodes básicos + timers. O loop principal (`main.cpp`) processa apenas `SDL_EVENT_QUIT` e executa um ciclo de CPU por frame. Não existe estado de teclado nem leitura de entrada. Ver `proposal.md` para motivação.

## Goals / Non-Goals

**Goals:**
- Adicionar estado de 16 teclas (0x0–0xF) à CPU.
- Implementar opcodes 0xEX9E, 0xEXA1, 0xFX0A.
- Capturar eventos de teclado SDL3 (KEY_DOWN/KEY_UP) e mapear para índices CHIP-8.
- Implementar bloqueio em 0xFX0A até uma tecla ser pressionada.

**Non-Goals:**
- Mapeamento customizável de teclas.
- Suporte a gamepad.
- Demais opcodes CHIP-8 (~22 restantes) — changes futuros.
- Detecção de edge (pressionado "agora" vs. segurado) — escopo mínimo usa estado atual.

## Decisions

### 1. Estado das teclas na CPU

**Decisão:** Adicionar campo `uint8_t keys[16]` à classe `Cpu`, público como `v[]`, inicializado em zero no `reset()`, mais método `void setKey(uint8_t key, bool pressed)`.

**Rationale:** Consistente com registradores e timers; opcodes acessam diretamente. Alternativa (classe separada de input) rejeitada por indireção desnecessária.

### 2. Mapeamento de teclas

**Decisão:** Mapeamento fixo padrão CHIP-8 usando scancodes SDL3:
1→0x1, 2→0x2, 3→0x3, 4→0xC, Q→0x4, W→0x5, E→0x6, R→0xD, A→0x7, S→0x8, D→0x9, F→0xE, Z→0xA, X→0x0, C→0xB, V→0xF.

**Rationale:** Layout padrão amplamente usado; scancodes são independentes de layout de teclado (posição física).

### 3. Bloqueio em 0xFX0A

**Decisão:** Quando 0xFX0A é executado e nenhuma tecla está pressionada, não avançar o PC (retornar sucesso sem avanço), mantendo a execução na mesma instrução até uma tecla ser pressionada. Quando pressionada, armazenar o índice em Vx e avançar PC em 2.

**Rationale:** Simples e alinhado ao comportamento CHIP-8. O loop principal reexecuta a instrução a cada frame. Alternativa (flag de "esperando") rejeitada por complexidade; o PC travado é suficiente.

### 4. Captura de eventos

**Decisão:** Em `main.cpp`, processar `SDL_EVENT_KEY_DOWN` e `SDL_EVENT_KEY_UP`, converter scancode para índice CHIP-8 via função de mapeamento, e chamar `cpu.setKey(idx, pressed)`. Eventos não mapeados são ignorados.

**Rationale:** Mantém a lógica de entrada no loop principal (onde os eventos SDL já são polled), consistente com o padrão existente.

### 5. Opcodes 0xEX*

**Decisão:** Adicionar case `0xE000` no switch de `execute()`, decodificando por `kk` (0x9E, 0xA1). Demais sub-opcodes 0xEX* caem no default (no-op).

**Rationale:** Consistente com a estrutura existente de dispatch por nibble alto.

## Risks / Trade-offs

- [0xFX0A com tecla segurada] → Se uma tecla permanecer pressionada, o programa pode avançar imediatamente em execuções subsequentes de 0xFX0A. Aceitável para o escopo; detecção de edge pode ser adicionada depois.
- [Velocidade de CPU] → Um ciclo por frame torna o bloqueio em 0xFX0A responsivo apenas a 60 Hz. Fora do escopo.

## Migration Plan

N/A — adição de funcionalidade, sem quebra de compatibilidade.

## Open Questions

Nenhuma.
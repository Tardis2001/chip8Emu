# Tasks

## 1. Estado do teclado e opcodes na CPU

- [x] 1.1 Adicionar campo `keys[16]` (uint8_t) e método `setKey()` à classe `Cpu` em `cpu.h` e verificar que a declaração compila
- [x] 1.2 Inicializar `keys` com 0 em `reset()` e verificar que todas as teclas começam não pressionadas
- [x] 1.3 Adicionar case `0xE000` no dispatch de `execute()` decodificando sub-opcode por `kk` e verificar que o build compila
- [x] 1.4 Implementar opcode `0xEX9E` (pular se tecla Vx pressionada) e verificar que avança 4 bytes quando pressionada e 2 quando não
- [x] 1.5 Implementar opcode `0xEXA1` (pular se tecla Vx não pressionada) e verificar que avança 4 bytes quando não pressionada e 2 quando pressionada
- [x] 1.6 Implementar opcode `0xFX0A` (aguardar tecla, armazenar em Vx) e verificar que bloqueia sem tecla e armazena/avança quando pressionada

## 2. Captura de entrada no loop principal

- [x] 2.1 Implementar mapeamento de scancode SDL para índice CHIP-8 e processar `SDL_EVENT_KEY_DOWN`/`SDL_EVENT_KEY_UP` chamando `cpu.setKey()`, verificando que teclas mapeadas atualizam o estado
- [x] 2.2 Ignorar eventos de teclas não mapeadas e verificar que não alteram o estado das teclas CHIP-8

## 3. Build e verificação de integração

- [x] 3.1 Rebuild completo e executar com ROM de teste (ex.: Pong), verificando que a janela abre e não trava
- [x] 3.2 Verificar que pressionar teclas mapeadas atualiza o estado observável (ex.: programa que lê tecla responde)

## Workflow follow-up

- Archive the change after the project's review requirements are satisfied.
- Verify the archived result.
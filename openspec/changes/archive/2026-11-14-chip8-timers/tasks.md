# Tasks

## 1. Estado dos timers e opcodes na CPU

- [x] 1.1 Adicionar campos `delayTimer` e `soundTimer` (uint8_t) e método `tickTimers()` à classe `Cpu` em `cpu.h` e verificar que a declaração compila
- [x] 1.2 Inicializar `delayTimer` e `soundTimer` com 0 em `reset()` e verificar que ambos começam em zero
- [x] 1.3 Implementar `tickTimers()` decrementando cada timer (se > 0) em 1 e verificar que decrementa até zero e permanece em zero
- [x] 1.4 Adicionar case `0xF000` no dispatch de `execute()` decodificando sub-opcode por `kk` e verificar que o build compila
- [x] 1.5 Implementar opcode `0xFX07` (Vx = delayTimer) e verificar que Vx recebe o valor do delay timer
- [x] 1.6 Implementar opcode `0xFX15` (delayTimer = Vx) e verificar que o delay timer recebe o valor de Vx
- [x] 1.7 Implementar opcode `0xFX18` (soundTimer = Vx) e verificar que o sound timer recebe o valor de Vx

## 2. Integração no loop principal (decremento e áudio)

- [x] 2.1 Inicializar SDL com `SDL_INIT_AUDIO` e abrir dispositivo/stream de áudio com callback de onda quadrada (440 Hz) controlado por flag atômico, verificando que a inicialização de áudio tem sucesso (ou degradação graciosa com aviso)
- [x] 2.2 Atualizar o flag `soundActive` a cada frame com base em `cpu.soundTimer > 0` e verificar que reflete o estado do timer
- [x] 2.3 Chamar `cpu.tickTimers()` uma vez por frame no loop principal e verificar que os timers decrementam a ~60 Hz

## 3. Build e verificação de integração

- [x] 3.1 Rebuild completo e executar com ROM de teste, verificando que a janela abre e não trava
- [x] 3.2 Verificar que uma ROM que usa delay timer (ex.: animação) executa sem erro e os timers decrementam observavelmente

## Workflow follow-up

- Archive the change after the project's review requirements are satisfied.
- Verify the archived result.
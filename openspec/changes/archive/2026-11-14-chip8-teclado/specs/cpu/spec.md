# Spec Delta

## ADDED Requirements

### Requirement: Opcode 0xEX9E - pular se tecla pressionada

O sistema SHALL implementar o opcode 0xEX9E, que avanca o program counter em 2 instrucoes adicionais se a tecla indicada pelo registrador Vx estiver pressionada.

#### Scenario: Pula quando tecla pressionada

- **WHEN** o opcode 0xEX9E e executado e a tecla Vx esta pressionada
- **THEN** o program counter avanca 4 bytes (2 instrucoes alem do normal)

#### Scenario: Nao pula quando tecla nao pressionada

- **WHEN** o opcode 0xEX9E e executado e a tecla Vx nao esta pressionada
- **THEN** o program counter avanca 2 bytes (apenas a instrucao atual)

### Requirement: Opcode 0xEXA1 - pular se tecla nao pressionada

O sistema SHALL implementar o opcode 0xEXA1, que avanca o program counter em 2 instrucoes adicionais se a tecla indicada pelo registrador Vx nao estiver pressionada.

#### Scenario: Pula quando tecla nao pressionada

- **WHEN** o opcode 0xEXA1 e executado e a tecla Vx nao esta pressionada
- **THEN** o program counter avanca 4 bytes (2 instrucoes alem do normal)

#### Scenario: Nao pula quando tecla pressionada

- **WHEN** o opcode 0xEXA1 e executado e a tecla Vx esta pressionada
- **THEN** o program counter avanca 2 bytes (apenas a instrucao atual)

### Requirement: Opcode 0xFX0A - aguardar tecla

O sistema SHALL implementar o opcode 0xFX0A, que bloqueia a execucao ate que uma tecla seja pressionada, armazenando o indice da tecla pressionada no registrador Vx e avancando o program counter.

#### Scenario: Aguarda e armazena tecla pressionada

- **WHEN** o opcode 0xFX0A e executado e uma tecla e pressionada
- **THEN** o registrador Vx contem o indice da tecla pressionada e o program counter avanca 2 bytes

#### Scenario: Bloqueia quando nenhuma tecla pressionada

- **WHEN** o opcode 0xFX0A e executado e nenhuma tecla esta pressionada
- **THEN** o program counter nao avanca e a execucao permanece na mesma instrucao ate uma tecla ser pressionada
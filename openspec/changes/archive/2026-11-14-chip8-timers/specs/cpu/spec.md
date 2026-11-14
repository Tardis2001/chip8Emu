# Spec Delta

## ADDED Requirements

### Requirement: Opcode 0xFX07 - ler delay timer

O sistema SHALL implementar o opcode 0xFX07, que define o registrador Vx com o valor atual do delay timer.

#### Scenario: Vx recebe valor do delay timer

- **WHEN** o opcode 0xFX07 é executado
- **THEN** o registrador Vx contém o valor atual do delay timer

### Requirement: Opcode 0xFX15 - definir delay timer

O sistema SHALL implementar o opcode 0xFX15, que define o delay timer com o valor do registrador Vx.

#### Scenario: Delay timer recebe valor de Vx

- **WHEN** o opcode 0xFX15 é executado
- **THEN** o delay timer contém o valor do registrador Vx

### Requirement: Opcode 0xFX18 - definir sound timer

O sistema SHALL implementar o opcode 0xFX18, que define o sound timer com o valor do registrador Vx.

#### Scenario: Sound timer recebe valor de Vx

- **WHEN** o opcode 0xFX18 é executado
- **THEN** o sound timer contém o valor do registrador Vx
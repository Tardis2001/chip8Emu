# Spec Delta

## Purpose

Fornece os timers de delay e som do CHIP-8, com decremento a 60 Hz e emissao de beep enquanto o sound timer for maior que zero, permitindo que programas controlem o ritmo de execucao e emitam som.

## ADDED Requirements

### Requirement: Delay timer de 8 bits

O sistema SHALL fornecer um delay timer de 8 bits (0x00–0xFF) utilizado para controlar o ritmo de execucao dos programas CHIP-8.

#### Scenario: Delay timer inicializa em zero

- **WHEN** o emulador é inicializado
- **THEN** o delay timer contém 0x00

#### Scenario: Delay timer pode ser lido e escrito

- **WHEN** um valor é escrito no delay timer e depois lido
- **THEN** o valor lido é igual ao valor escrito

### Requirement: Sound timer de 8 bits

O sistema SHALL fornecer um sound timer de 8 bits (0x00–0xFF) utilizado para controlar a emissao de som nos programas CHIP-8.

#### Scenario: Sound timer inicializa em zero

- **WHEN** o emulador é inicializado
- **THEN** o sound timer contém 0x00

#### Scenario: Sound timer pode ser lido e escrito

- **WHEN** um valor é escrito no sound timer e depois lido
- **THEN** o valor lido é igual ao valor escrito

### Requirement: Decremento do delay timer a 60 Hz

O sistema SHALL decrementar o delay timer em 1 a cada tick de 60 Hz, até atingir zero, onde permanece.

#### Scenario: Decrementa quando maior que zero

- **WHEN** o delay timer é maior que zero e ocorre um tick de 60 Hz
- **THEN** o delay timer é decrementado em 1

#### Scenario: Permanece em zero

- **WHEN** o delay timer é zero e ocorre um tick de 60 Hz
- **THEN** o delay timer permanece em zero

### Requirement: Decremento do sound timer a 60 Hz

O sistema SHALL decrementar o sound timer em 1 a cada tick de 60 Hz, até atingir zero, onde permanece.

#### Scenario: Decrementa quando maior que zero

- **WHEN** o sound timer é maior que zero e ocorre um tick de 60 Hz
- **THEN** o sound timer é decrementado em 1

#### Scenario: Permanece em zero

- **WHEN** o sound timer é zero e ocorre um tick de 60 Hz
- **THEN** o sound timer permanece em zero

### Requirement: Beep enquanto sound timer maior que zero

O sistema SHALL emitir um beep (som) enquanto o sound timer for maior que zero, e cessar o som quando o sound timer atingir zero.

#### Scenario: Beep ativo enquanto timer maior que zero

- **WHEN** o sound timer é maior que zero
- **THEN** um beep é emitido

#### Scenario: Sem beep quando timer zero

- **WHEN** o sound timer é zero
- **THEN** nenhum beep é emitido
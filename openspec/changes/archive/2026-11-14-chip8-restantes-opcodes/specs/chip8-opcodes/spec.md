# Spec Delta

## Purpose

Fornece os requisitos de comportamento para os opcodes CHIP-8 restantes (display, fluxo, logica/aritmetica, memoria/indice e aleatorio), completando o conjunto de instrucoes do emulador.

## ADDED Requirements

### Requirement: Opcode 0x00E0 - limpar display

O sistema SHALL implementar o opcode 0x00E0, que limpa todos os pixels do display (define como desligado/preto).

#### Scenario: Limpar display

- **WHEN** o opcode 0x00E0 e executado
- **THEN** todos os 2048 pixels do display sao definidos como desligados

### Requirement: Opcode 0x3XNN - pular se Vx igual a NN

O sistema SHALL implementar o opcode 0x3XNN, que avanca o program counter em 2 instrucoes adicionais se Vx for igual a NN.

#### Scenario: Pula quando Vx igual a NN

- **WHEN** o opcode 0x3XNN e executado e Vx contem o valor NN
- **THEN** o program counter avanca 4 bytes

#### Scenario: Nao pula quando Vx diferente de NN

- **WHEN** o opcode 0x3XNN e executado e Vx contem valor diferente de NN
- **THEN** o program counter avanca 2 bytes

### Requirement: Opcode 0x4XNN - pular se Vx diferente de NN

O sistema SHALL implementar o opcode 0x4XNN, que avanca o program counter em 2 instrucoes adicionais se Vx for diferente de NN.

#### Scenario: Pula quando Vx diferente de NN

- **WHEN** o opcode 0x4XNN e executado e Vx contem valor diferente de NN
- **THEN** o program counter avanca 4 bytes

#### Scenario: Nao pula quando Vx igual a NN

- **WHEN** o opcode 0x4XNN e executado e Vx contem o valor NN
- **THEN** o program counter avanca 2 bytes

### Requirement: Opcode 0x5XY0 - pular se Vx igual a Vy

O sistema SHALL implementar o opcode 0x5XY0, que avanca o program counter em 2 instrucoes adicionais se Vx for igual a Vy.

#### Scenario: Pula quando Vx igual a Vy

- **WHEN** o opcode 0x5XY0 e executado e Vx contem o mesmo valor que Vy
- **THEN** o program counter avanca 4 bytes

#### Scenario: Nao pula quando Vx diferente de Vy

- **WHEN** o opcode 0x5XY0 e executado e Vx contem valor diferente de Vy
- **THEN** o program counter avanca 2 bytes

### Requirement: Opcode 0x9XY0 - pular se Vx diferente de Vy

O sistema SHALL implementar o opcode 0x9XY0, que avanca o program counter em 2 instrucoes adicionais se Vx for diferente de Vy.

#### Scenario: Pula quando Vx diferente de Vy

- **WHEN** o opcode 0x9XY0 e executado e Vx contem valor diferente de Vy
- **THEN** o program counter avanca 4 bytes

#### Scenario: Nao pula quando Vx igual a Vy

- **WHEN** o opcode 0x9XY0 e executado e Vx contem o mesmo valor que Vy
- **THEN** o program counter avanca 2 bytes

### Requirement: Opcode 0x8XY0 - Vx recebe Vy

O sistema SHALL implementar o opcode 0x8XY0, que define Vx com o valor de Vy.

#### Scenario: Copiar valor de Vy para Vx

- **WHEN** o opcode 0x8XY0 e executado
- **THEN** Vx contem o mesmo valor que Vy

### Requirement: Opcode 0x8XY1 - Vx OR Vy

O sistema SHALL implementar o opcode 0x8XY1, que define Vx como o resultado de Vx OR Vy (operacao bitwise).

#### Scenario: Operacao OR entre registradores

- **WHEN** o opcode 0x8XY1 e executado
- **THEN** Vx contem o resultado de Vx OR Vy

### Requirement: Opcode 0x8XY2 - Vx AND Vy

O sistema SHALL implementar o opcode 0x8XY2, que define Vx como o resultado de Vx AND Vy (operacao bitwise).

#### Scenario: Operacao AND entre registradores

- **WHEN** o opcode 0x8XY2 e executado
- **THEN** Vx contem o resultado de Vx AND Vy

### Requirement: Opcode 0x8XY3 - Vx XOR Vy

O sistema SHALL implementar o opcode 0x8XY3, que define Vx como o resultado de Vx XOR Vy (operacao bitwise).

#### Scenario: Operacao XOR entre registradores

- **WHEN** o opcode 0x8XY3 e executado
- **THEN** Vx contem o resultado de Vx XOR Vy

### Requirement: Opcode 0x8XY4 - Vx adiciona Vy com carry

O sistema SHALL implementar o opcode 0x8XY4, que soma Vy a Vx, armazena o resultado em Vx e define VF como 1 se houver carry (resultado > 255) ou 0 caso contrario.

#### Scenario: Soma sem carry

- **WHEN** o opcode 0x8XY4 e executado e a soma de Vx e Vy nao excede 255
- **THEN** Vx contem a soma e VF contem 0

#### Scenario: Soma com carry

- **WHEN** o opcode 0x8XY4 e executado e a soma de Vx e Vy excede 255
- **THEN** Vx contem o resultado modulo 256 e VF contem 1

### Requirement: Opcode 0x8XY5 - Vx subtrai Vy com emprestimo

O sistema SHALL implementar o opcode 0x8XY5, que subtrai Vy de Vx, armazena o resultado em Vx e define VF como 0 se houver emprestimo (Vx < Vy) ou 1 caso contrario.

#### Scenario: Subtracao sem emprestimo

- **WHEN** o opcode 0x8XY5 e executado e Vx maior ou igual a Vy
- **THEN** Vx contem a diferenca e VF contem 1

#### Scenario: Subtracao com emprestimo

- **WHEN** o opcode 0x8XY5 e executado e Vx menor que Vy
- **THEN** Vx contem o resultado modulo 256 e VF contem 0

### Requirement: Opcode 0x8XY6 - Vx desloca para a direita

O sistema SHALL implementar o opcode 0x8XY6, que desloca Vx um bit para a direita e define VF com o bit menos significativo original de Vx.

#### Scenario: Deslocamento direita com bit par

- **WHEN** o opcode 0x8XY6 e executado e Vx tem bit menos significativo igual a 0
- **THEN** Vx e deslocado para a direita e VF contem 0

#### Scenario: Deslocamento direita com bit impar

- **WHEN** o opcode 0x8XY6 e executado e Vx tem bit menos significativo igual a 1
- **THEN** Vx e deslocado para a direita e VF contem 1

### Requirement: Opcode 0x8XY7 - Vx recebe Vy menos Vx

O sistema SHALL implementar o opcode 0x8XY7, que define Vx como Vy menos Vx e define VF como 0 se houver emprestimo (Vy < Vx) ou 1 caso contrario.

#### Scenario: Subtracao inversa sem emprestimo

- **WHEN** o opcode 0x8XY7 e executado e Vy maior ou igual a Vx
- **THEN** Vx contem Vy menos Vx e VF contem 1

#### Scenario: Subtracao inversa com emprestimo

- **WHEN** o opcode 0x8XY7 e executado e Vy menor que Vx
- **THEN** Vx contem o resultado modulo 256 e VF contem 0

### Requirement: Opcode 0x8XYE - Vx desloca para a esquerda

O sistema SHALL implementar o opcode 0x8XYE, que desloca Vx um bit para a esquerda e define VF com o bit mais significativo original de Vx.

#### Scenario: Deslocamento esquerda sem overflow

- **WHEN** o opcode 0x8XYE e executado e Vx tem bit mais significativo igual a 0
- **THEN** Vx e deslocado para a esquerda e VF contem 0

#### Scenario: Deslocamento esquerda com overflow

- **WHEN** o opcode 0x8XYE e executado e Vx tem bit mais significativo igual a 1
- **THEN** Vx e deslocado para a esquerda e VF contem 1

### Requirement: Opcode 0xBNNN - pular para endereco mais V0

O sistema SHALL implementar o opcode 0xBNNN, que define o program counter como NNN mais o valor de V0.

#### Scenario: Salto com deslocamento por V0

- **WHEN** o opcode 0xBNNN e executado
- **THEN** o program counter contem NNN mais o valor de V0

### Requirement: Opcode 0xCXNN - Vx recebe aleatorio mascarado

O sistema SHALL implementar o opcode 0xCXNN, que define Vx com o resultado de um valor aleatorio AND NN.

#### Scenario: Valor aleatorio mascarado

- **WHEN** o opcode 0xCXNN e executado
- **THEN** Vx contem um valor aleatorio AND NN (apenas bits onde NN tem 1)

### Requirement: Opcode 0xFX1E - I adiciona Vx

O sistema SHALL implementar o opcode 0xFX1E, que adiciona o valor de Vx ao registrador indice I.

#### Scenario: Adicionar Vx a I

- **WHEN** o opcode 0xFX1E e executado
- **THEN** I contem o valor anterior de I mais o valor de Vx

### Requirement: Opcode 0xFX29 - I aponta para sprite do digito Vx

O sistema SHALL implementar o opcode 0xFX29, que define I como o endereco inicial do sprite do digito hexadecimal armazenado em Vx no conjunto de fontes (enderecos 0x000 a 0xFF).

#### scenario: I aponta para sprite do digito

- **WHEN** o opcode 0xFX29 e executado com Vx contendo um digito valido
- **THEN** I contem o endereco do sprite desse digito no conjunto de fontes

### Requirement: Opcode 0xFX33 - armazenar BCD de Vx

O sistema SHALL implementar o opcode 0xFX33, que armazena a representacao BCD (decimal codificado em binario) de Vx nos enderecos I, I+1 e I+2 (centenas, dezenas, unidades).

#### Scenario: Armazenar BCD de Vx

- **WHEN** o opcode 0xFX33 e executado com Vx contendo um valor de 0 a 255
- **THEN** os enderecos I, I+1 e I+2 contem respectivamente os digitos de centenas, dezenas e unidades de Vx

### Requirement: Opcode 0xFX55 - armazenar registradores V0 ate Vx

O sistema SHALL implementar o opcode 0xFX55, que armazena os valores dos registradores V0 ate Vx (inclusive) na memoria comecando no endereco I.

#### Scenario: Armazenar registradores na memoria

- **WHEN** o opcode 0xFX55 e executado
- **THEN** os valores de V0 ate Vx sao armazenados sequencialmente a partir do endereco I

### Requirement: Opcode 0xFX65 - carregar registradores V0 ate Vx

O sistema SHALL implementar o opcode 0xFX65, que carrega os valores dos registradores V0 ate Vx (inclusive) a partir da memoria comecando no endereco I.

#### Scenario: Carregar registradores da memoria

- **WHEN** o opcode 0xFX65 e executado
- **THEN** os registradores V0 ate Vx sao carregados sequencialmente a partir do endereco I
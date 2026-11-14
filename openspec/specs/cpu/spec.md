# cpu Specification

## Purpose

Fornece os registradores e o ciclo de execucao da CPU CHIP-8, permitindo buscar, decodificar e executar opcodes a partir da memoria ja existente.

## Requirements

### Requirement: Registradores V0–VF

O sistema SHALL fornecer 16 registradores de proposito geral, nomeados V0 ate VF, cada um armazenando um byte (0x00–0xFF).

#### Scenario: Ler e escrever um registrador Vx

- **WHEN** um valor e escrito em um registrador Vx e depois lido no mesmo registrador
- **THEN** o valor lido e igual ao valor escrito

#### Scenario: Registradores inicializam em zero

- **WHEN** a CPU e inicializada
- **THEN** todos os 16 registradores V0–VF contem 0x00

### Requirement: Registrador indice I

O sistema SHALL fornecer um registrador indice I capaz de armazenar um endereço de 12 bits (0x000–0xFFF).

#### Scenario: Ler e escrever o registrador I

- **WHEN** um endereco valido e escrito no registrador I e depois lido
- **THEN** o valor lido e igual ao endereco escrito

### Requirement: Program counter PC

O sistema SHALL fornecer um program counter PC que armazena o endereco da proxima instrucao a ser executada, iniciando em 0x200.

#### Scenario: PC inicializa em 0x200

- **WHEN** a CPU e inicializada apos carregar uma ROM
- **THEN** o program counter contem 0x200

#### Scenario: PC avapos buscar um opcode

- **WHEN** a CPU busca um opcode de 2 bytes no endereco atual do PC
- **THEN** o PC avanca 2 posicoes para o proximo opcode

### Requirement: Pilha e stack pointer

O sistema SHALL fornecer uma pilha de 16 niveis e um stack pointer para chamadas de subrotina e retorno.

#### Scenario: Chamada empilha endereco de retorno

- **WHEN** a CPU executa um opcode de chamada de subrotina
- **THEN** o endereco de retorno e empilhado e o stack pointer e incrementado

#### Scenario: Retorno desempilha endereco

- **WHEN** a CPU executa um opcode de retorno de subrotina
- **THEN** o endereco e desempilhado, o stack pointer e decrementado e o PC salta para esse endereco

### Requirement: Ciclo fetch-decode-execute

O sistema SHALL executar um ciclo de CPU que busca um opcode da memoria, decodifica-o e executa a acao correspondente.

#### Scenario: Executar um ciclo de CPU sem erros

- **WHEN** uma ROM valida esta carregada na memoria a partir de 0x200
- **THEN** a CPU busca, decodifica e executa o primeiro opcode sem falha

### Requirement: Integracao com memoria existente

O sistema SHALL buscar opcodes e ler/escrever dados utilizando a capacidade de memoria ja existente (chip8-memory).

#### Scenario: CPU le opcode da memoria

- **WHEN** a CPU solicita a leitura de um byte em um endereco valido da memoria
- **THEN** o byte retornado vem da memoria ja existente

### Requirement: Opcode 0x6XNN - definir registrador Vx

O sistema SHALL implementar o opcode 0x6XNN, que define o registrador Vx com o valor byte NN.

#### Scenario: Definir Vx com valor imediato

- **WHEN** o opcode 0x6XNN é executado com NN igual a um valor byte
- **THEN** o registrador Vx contem o valor NN

### Requirement: Opcode 0x7XNN - adicionar ao registrador Vx

O sistema SHALL implementar o opcode 0x7XNN, que adiciona o valor byte NN ao registrador Vx (sem carry para VF).

#### Scenario: Adicionar valor imediato a Vx

- **WHEN** o opcode 0x7XNN é executado com NN igual a um valor byte
- **THEN** o registrador Vx contem o resultado da adicao modulo 256

### Requirement: Opcode 0xANNN - definir registrador indice I

O sistema SHALL implementar o opcode 0xANNN, que define o registrador indice I com o endereço NNN.

#### Scenario: Definir I com endereco imediato

- **WHEN** o opcode 0xANNN é executado com NNN igual a um endereco de 12 bits
- **THEN** o registrador I contem o valor NNN

### Requirement: Opcode 0x1NNN - pular para endereco

O sistema SHALL implementar o opcode 0x1NNN, que define o program counter PC como o endereço NNN.

#### Scenario: Pular para endereco imediato

- **WHEN** o opcode 0x1NNN é executado com NNN igual a um endereco valido
- **THEN** o program counter PC contem o valor NNN

### Requirement: Opcode 0x2NNN - chamar subrotina

O sistema SHALL implementar o opcode 0x2NNN, que chama uma subrotina no endereço NNN, empilhando o endereco de retorno atual.

#### Scenario: Chamar subrotina empilha retorno

- **WHEN** o opcode 0x2NNN é executado
- **THEN** o endereco atual do PC e empilhado e o PC e definido como NNN

### Requirement: Opcode 0x00EE - retornar de subrotina

O sistema SHALL implementar o opcode 0x00EE, que retorna de uma subrotina, desempilhando o endereco de retorno para o PC.

#### Scenario: Retornar de subrotina desempilha PC

- **WHEN** o opcode 0x00EE é executado apos uma chamada de subrotina
- **THEN** o PC e definido com o endereco desempilhado e o stack pointer e decrementado

### Requirement: Opcode 0xDXYN - desenhar sprite

O sistema SHALL implementar o opcode 0xDXYN, que desenha um sprite de N bytes na posicao (Vx, Vy) do display, usando o registrador I como endereco inicial dos dados do sprite, e define VF como 1 se houver colisao de pixels ou 0 caso contrario.

#### Scenario: Desenhar sprite no display

- **WHEN** o opcode 0xDXYN é executado com coordenadas validas e dados de sprite na memoria
- **THEN** o sprite eh desenhado no display na posicao (Vx, Vy) e VF reflete se ocorreu colisao

### Requirement: Draw flag

O sistema SHALL fornecer um draw flag que indica quando o display deve ser redesenhado apos a execucao de um opcode de desenho.

#### Scenario: Draw flag definido apos desenho

- **WHEN** o opcode 0xDXYN e executado
- **THEN** o draw flag eh definido como verdadeiro

#### Scenario: Draw flag lido e limpo

- **WHEN** o draw flag eh lido pelo loop de renderizacao
- **THEN** o draw flag eh limpo (definido como falso) apos a leitura

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
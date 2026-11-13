# Spec Delta

## Purpose

Fornece os registradores e o ciclo de execucao da CPU CHIP-8, permitindo buscar, decodificar e executar opcodes a partir da memoria ja existente.

## ADDED Requirements

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

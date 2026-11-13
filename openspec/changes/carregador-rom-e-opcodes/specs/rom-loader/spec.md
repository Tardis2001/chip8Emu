# Spec Delta

## Purpose

Fornece o carregamento de arquivos ROM (.ch8 ou .c8) do disco para a memória do emulador CHIP-8, a partir do endereço 0x200, permitindo que programas externos sejam executados.

## ADDED Requirements

### Requirement: Carregamento de ROM a partir de arquivo

O sistema SHALL ler um arquivo ROM (.ch8 ou .c8) especificado via linha de comando e gravar seu conteúdo na memória começando no endereço 0x200.

#### Scenario: Arquivo ROM válido é carregado na memória

- **WHEN** um caminho de arquivo ROM válido é fornecido via linha de comando
- **THEN** o conteúdo do arquivo é gravado na memória começando no endereço 0x200

#### Scenario: Arquivo ROM inexistente gera erro

- **WHEN** um caminho de arquivo ROM que não existe é fornecido
- **THEN** o sistema informa que o arquivo não pôde ser carregado

#### Scenario: ROM muito grande não excede os limites da memória

- **WHEN** um arquivo ROM maior que o espaço disponível de 0x200 a 0xFFF é fornecido
- **THEN** o sistema rejeita o carregamento e não grava dados parciais na memória

### Requirement: Integração com a memória existente

O sistema SHALL utilizar a capacidade de memória já existente (chip8-memory) para gravar os bytes da ROM carregada.

#### Scenario: Bytes da ROM são gravados via memória existente

- **WHEN** o carregador de ROM grava um byte na memória
- **THEN** o byte é armazenado utilizando a interface de escrita da memória existente

# chip8-memory Specification

## Purpose

Fornece a memória de 4 KB do emulador CHIP-8, incluindo o conjunto de fontes embutido e o carregamento de ROM, para que a CPU possa buscar e executar instruções.

## Requirements

### Requirement: Memória de 4 KB endereçável

O sistema SHALL fornecer 4 KB de memória endereçável nos endereços 0x000 a 0xFFF.

#### Scenario: Ler endereço válido retorna byte armazenado

- **WHEN** um byte é escrito em um endereço válido (0x000–0xFFF) e depois lido no mesmo endereço
- **THEN** o valor lido é igual ao valor escrito

#### Scenario: Ler endereço fora do intervalo é rejeitado

- **WHEN** uma leitura é solicitada em um endereço fora do intervalo 0x000–0xFFF
- **THEN** a operação não retorna um byte de memória válido

### Requirement: Conjunto de fontes embutido

O sistema SHALL carregar o conjunto de fontes do CHIP-8 nos endereços 0x000 a 0xFF.

#### Scenario: Fontes estão disponíveis após inicialização

- **WHEN** a memória é inicializada
- **THEN** os bytes do conjunto de fontes estão presentes nos endereços 0x000–0xFF

### Requirement: Carregamento de ROM a partir de 0x200

O sistema SHALL carregar o conteúdo de um arquivo ROM (.ch8 ou .c8) começando no endereço 0x200.

#### Scenario: ROM é carregada começando em 0x200

- **WHEN** um arquivo ROM é fornecido ao carregador
- **THEN** o primeiro byte da ROM fica no endereço 0x200 e os bytes seguintes ocupam endereços contíguos

### Requirement: Leitura de byte

O sistema SHALL permitir ler um byte de qualquer endereço válido da memória.

#### Scenario: Ler byte de endereço na região de programa

- **WHEN** uma ROM foi carregada em 0x200 e a CPU solicita a leitura de 0x200
- **THEN** o byte do primeiro opcode da ROM é retornado

### Requirement: Escrita de byte

O sistema SHALL permitir escrever um byte em qualquer endereço válido da região de RAM da memória.

#### Scenario: Escrever e ler de volta um byte

- **WHEN** um valor é escrito em um endereço de RAM e depois lido
- **THEN** o valor lido corresponde ao valor escrito

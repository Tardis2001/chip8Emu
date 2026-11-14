# Spec Delta

## Purpose

Fornece o estado e a captura de entrada do teclado CHIP-8 (16 teclas, indices 0x0 a 0xF), com mapeamento padrao de teclas fisicas, permitindo que programas CHIP-8 leiam a entrada do usuario.

## ADDED Requirements

### Requirement: Estado de 16 teclas

O sistema SHALL fornecer o estado de 16 teclas (indices 0x0 a 0xF), onde cada tecla pode estar pressionada ou nao pressionada.

#### Scenario: Teclas inicializam nao pressionadas

- **WHEN** o emulador e inicializado
- **THEN** todas as 16 teclas estao no estado nao pressionado

#### Scenario: Estado de uma tecla pode ser consultado

- **WHEN** uma tecla e pressionada e depois consultada
- **THEN** a consulta retorna que a tecla esta pressionada

### Requirement: Atualizacao de estado de tecla

O sistema SHALL permitir atualizar o estado de uma tecla (pressionada ou solta) a partir de eventos de entrada.

#### Scenario: Pressionar uma tecla

- **WHEN** um evento de tecla pressionada e recebido para uma tecla mapeada
- **THEN** o estado dessa tecla passa a pressionado

#### Scenario: Soltar uma tecla

- **WHEN** um evento de tecla solta e recebido para uma tecla mapeada
- **THEN** o estado dessa tecla passa a nao pressionado

### Requirement: Mapeamento padrao de teclas

O sistema SHALL mapear as teclas fisicas do teclado para os indices CHIP-8 0x0 a 0xF seguindo o layout padrao: 1 2 3 4 / Q W E R / A S D F / Z X C V.

#### Scenario: Tecla fisica mapeada para indice CHIP-8

- **WHEN** uma tecla fisica do layout padrao e pressionada
- **THEN** o indice CHIP-8 correspondente tem seu estado atualizado

#### Scenario: Tecla nao mapeada e ignorada

- **WHEN** uma tecla fisica fora do layout padrao e pressionada
- **THEN** nenhum estado de tecla CHIP-8 e alterado
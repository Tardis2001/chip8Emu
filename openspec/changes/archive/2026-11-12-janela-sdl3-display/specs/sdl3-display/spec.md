# Spec Delta

## Purpose

Gerencia a janela SDL3, o renderer e o buffer de display de 64×32 pixels do emulador CHIP-8, fornecendo saída gráfica mínima para renderizar o estado da tela.

## ADDED Requirements

### Requirement: Janela SDL3 criada
O sistema SHALL criar uma janela SDL3 com título e dimensões definidas ao iniciar o emulador.

#### Scenario: Janela aberta com sucesso
- **WHEN** o emulador é iniciado
- **THEN** uma janela SDL3 é exibida com o título configurado e dimensões esperadas

#### Scenario: Janela fecha corretamente
- **WHEN** o usuário aciona o botão de fechar da janela
- **THEN** o SDL3 recebe um evento de encerramento e o programa finaliza

### Requirement: Display em preto renderizado
O sistema SHALL renderizar um display de 64×32 pixels com todos os pixels desligados (cor preta).

#### Scenario: Display inicializa em preto
- **WHEN** o loop de renderização é iniciado
- **THEN** todos os 2048 pixels do display são desenhados como preto

#### Scenario: Display é renderizado a cada frame
- **WHEN** o loop principal executa um novo frame de renderização
- **THEN** o display preto é redesenhado antes do próximo frame

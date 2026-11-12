# Proposal

## Why

O emulador CHIP-8 atualmente não exibe nada na tela: `main.cpp` é um stub que inicializa o SDL3 e retorna 0. Antes de executar qualquer opcode, precisamos de uma janela SDL3 e um display preto como base para renderizar os gráficos do CHIP-8. Esta feature estabelece a infraestrutura gráfica mínima sobre a qual tudo o resto será construído.

## What Changes

- Inicializar SDL3 (vídeo e eventos) no ponto de entrada do emulador
- Criar uma janela SDL3 com tamanho adequado para o display CHIP-8 (64×32 pixels, com escala)
- Criar um renderer SDL3 associado à janela
- Implementar um buffer de display de 64×32 pixels (ligado/desligado)
- Renderizar o display como preto (todos os pixels desligados) a cada frame
- Loop principal que processa eventos SDL3, renderiza o display e controla o framerate (~60 Hz)
- Limpeza adequada dos recursos SDL3 ao encerrar

## Capabilities

### New Capabilities

- `sdl3-display`: Gerencia a janela SDL3, o renderer e o buffer de display de 64×32 pixels. É responsável por criar a janela, desenhar o display preto e manter o loop de renderização. Cada comportamento durável relacionado à exibição (desenhar sprites, limpar tela, escalar pixels) será capturado como requisito nesta capability.

### Modified Capabilities

- (nenhum)

## Impact

- `src/main.cpp`: deixará de ser um stub e conterá o loop principal do emulador
- `CMakeLists.txt`: já vincula SDL3, SDL3_ttf e SDL3_image; nenhuma alteração de dependência esperada nesta feature
- Novos arquivos-fonte sob `src/` para encapsular display e janela (a ser detalhado no design)

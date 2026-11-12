# Design

## Context

O emulador CHIP-8 está em estágio inicial: `src/main.cpp` inclui o SDL3 mas não faz nada além de retornar 0. O `CMakeLists.txt` já vincula SDL3, SDL3_ttf e SDL3_image via vcpkg, usando C++20. Não há buffer de display, janela, renderer nem loop de eventos implementados. Veja `proposal.md` para a motivação desta change.

## Goals / Non-Goals

**Goals:**
- Inicializar SDL3 e criar uma janela com renderer
- Implementar um buffer de display de 64×32 pixels
- Renderizar o display como preto a cada frame
- Manter um loop principal com eventos SDL3 e controle de framerate (~60 Hz)
- Limpar recursos SDL3 ao encerrar

**Non-Goals:**
- Emulação de opcodes CHIP-8 (CPU, memória, registradores)
- Desenho de sprites no display (será em uma change posterior)
- Entrada de teclado / mapeamento do teclado hexadecimal
- Áudio / timers (delay e sound)
- Carregamento de ROMs

## Decisions

### 1. Tamanho da janela e escala de pixels
**Decisão:** Janela de 640×320 pixels (escala 10× de cada pixel do CHIP-8).
**Rationale:** O display nativo do CHIP-8 é 64×32. Uma escala 10× torna os pixels visíveis o suficiente para jogar sem ocupar tela inteira. É simples de calcular (multiplicar coordenadas por 10) e não exige redimensionamento dinâmico nesta fase.
**Alternativas:**
- Janela 64×32 (1×): pixels muito pequenos para jogar confortavelmente.
- Tela cheia: introduz complexidade de redimensionamento e múltiplos monitores desnecessários nesta fase.

### 2. Representação do buffer de display
**Decisão:** Buffer bidimensional de booleanos (64×32), onde `true` = pixel ligado (branco), `false` = pixel desligado (preto).
**Rationale:** O CHIP-8 é monocromático. Um booleano por pixel é a representação mais simples e suficiente para esta feature. Estruturas mais complexas (bits, cores) podem ser introduzidas depois sem mudar a interface do display.
**Alternativas:**
- Array de bytes ou inteiros: mais memória sem benefício nesta fase.
- Textura SDL direta: adicionaria dependência de API SDL para algo que ainda não precisa de aceleração de GPU.

### 3. Renderização via SDL_Renderer
**Decisão:** Usar `SDL_Renderer` com `SDL_RenderDrawPoint` (ou retângulos preenchidos) para desenhar cada pixel do buffer.
**Rationale:** O SDL3 oferece `SDL_Renderer` como abstração de renderização 2D, suficiente para desenhar pixels individuais ou retângulos. Não requer shaders nem conhecimento de gráficos de baixo nível.
**Alternativas:**
- `SDL_Surface` + software rendering: mais verboso e menos adequado para renderização frame-a-frame.
- OpenGL/Vulkan via SDL: excessivo para um display monocromático 64×32.

### 4. Loop principal e framerate
**Decisão:** Loop que processa eventos SDL3, executa ciclos de CPU (quantidade a definir na change da CPU), atualiza timers (a definir), renderiza o display e limita o framerate a ~60 Hz com `SDL_Delay`.
**Rationale:** 60 Hz é a taxa de atualização típica do CHIP-8 e corresponde à taxa de decremento dos timers. `SDL_Delay` evita consumir 100% de CPU.
**Alternativas:**
- Loop sem limite de framerate: consumiria CPU desnecessariamente e tornaria o emulador imprevisível em diferentes máquinas.
- Temporizador de alta precisão (`SDL_GetPerformanceCounter`): pode ser adicionado depois se houver necessidade de sincronização mais exata.

## Risks / Trade-offs

- [Desempenho com muitos pixels] → Desenhar 2048 pixels individualmente por frame pode ser lento em máquinas muito modestas. Mitigar agrupando pixels adjacentes em retângulos se necessário.
- [Eventos SDL3 bloqueando o loop] → Se o processamento de eventos demorar, o framerate pode cair. Mitigar processando apenas eventos pendentes (`SDL_PollEvent` em loop) sem bloquear.
- [Janela não redimensionável nesta fase] → Usuários não podem alterar o tamanho. Mitigar adicionando redimensionamento em uma change posterior se necessário.

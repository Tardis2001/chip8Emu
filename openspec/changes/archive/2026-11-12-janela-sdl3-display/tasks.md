# Tasks

## 1. Setup — janela SDL3 e renderer

- [x] 1.1 Criar arquivo `src/display.h` e `src/display.cpp` com declarações e implementação iniciais, verificando que ambos compilam como parte do alvo `chip8` no CMake
- [x] 1.2 Inicializar SDL3 (subsistema de vídeo e eventos) no ponto de entrada, verificando que `SDL_Init` retorna sucesso e que a inicialização falha com mensagem de erro se o SDL3 não estiver disponível
- [x] 1.3 Criar janela SDL3 com título "CHIP-8 Emulador" e dimensões 640×320 pixels, verificando que a janela aparece na tela e tem as dimensões esperadas
- [x] 1.4 Criar renderer SDL3 associado à janela, verificando que o renderer é criado sem erro e que a cor de fundo é definível
- [x] 1.5 Implementar função de encerramento que destrói renderer, janela e faz `SDL_Quit`, verificando que o programa fecha sem vazamentos de recurso detectáveis pelo SDL3

## 2. Buffer de display e renderização preta

- [x] 2.1 Implementar buffer de display de 64×32 pixels (array de booleanos), inicializado como todos desligados (preto), verificando que todos os pixels começam como `false`
- [x] 2.2 Implementar função `clearDisplay()` que coloca todos os pixels como desligados, verificando que após chamá-la todos os pixels do buffer são `false`
- [x] 2.3 Implementar função `renderDisplay()` que desenha cada pixel do buffer na janela SDL3 (pixel desligado = preto, ligado = branco), verificando que o display renderizado é inteiramente preto quando todos os pixels estão desligados
- [x] 2.4 Integrar a renderização ao ponto de entrada, verificando que ao iniciar o emulador a janela exibe um display completamente preto

## 3. Loop principal e controle de framerate

- [x] 3.1 Implementar loop principal que processa eventos SDL3 (`SDL_PollEvent`) sem bloquear, verificando que eventos de fechamento de janela são detectados
- [x] 3.2 Implementar controle de framerate a ~60 Hz usando `SDL_Delay`, verificando que o loop não consome 100% de CPU e que a janela permanece responsiva
- [x] 3.3 Executar o loop: inicializar SDL3 → criar janela/renderer → renderizar display preto → processar eventos → encerrar, verificando que o emulador inicia, exibe o display preto, responde ao fechamento da janela e sai limpo

## Workflow follow-up

- Quando a implementação estiver completa, revisar a change com o usuário antes de prosseguir para `/opsx-apply`.

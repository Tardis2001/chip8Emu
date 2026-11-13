# Design

## Context

O emulador ja possui memoria (chip8-memory), display (sdl3-display) e CPU (registradores + ciclo fetch-decode-execute base). O que falta e a ligacao com o mundo externo: carregar ROMs do disco e executar opcodes específicos. Veja a motivacao em `proposal.md` - Why.

## Goals / Non-Goals

**Goals:**
- `main.cpp` aceita caminho de arquivo ROM (.ch8/.c8) via argv
- Carregador de ROM que lê arquivo do disco e grava na memoria a partir de 0x200
- Opcodes CHIP-8 basicos: 0x6XNN (set), 0x7XNN (add), 0xANNN (set I), 0x1NNN (jump), 0x2NNN (call), 0x00EE (return), 0xDXYN (draw)
- Draw flag para controle de renderizacao condicional
- Integracao do opcode de desenho com o display existente (sdl3-display)

**Non-Goals:**
- Implementacao de todos os ~35 opcodes CHIP-8 (apenas um subconjunto basico neste escopo)
- Timers (delay/sound)
- Entrada de teclado
- Som
- Super CHIP ou resolucoes alternativas

## Decisions

- **Estrutura de opcodes: tabela de dispatch baseada no nibble mais significativo** — cada família de opcode (0x0, 0x1, ..., 0xF) tem um manipulador dedicado. Alternativa: switch gigante com todos os opcodes; rejeitada porque fica dificil de manter e estender.
- **main.cpp carrega ROM antes de iniciar o loop de emulacao** — o caminho da ROM e lido de argv, o conteudo e carregado na memoria, depois o loop principal inicia normalmente. Alternativa: carregar dinamicamente durante a execucao; rejeitada porque complica o ciclo de vida sem beneficio neste escopo.
- **Draw flag como booleano na CPU** — definido como true pelo opcode 0xDXYN, lido e limpo pelo loop de renderizacao. Alternativa: redesenhar a cada frame independentemente; rejeitada porque e ineficiente e nao reflete o comportamento real do CHIP-8.
- **Opcode de desenho renderiza diretamente no Display existente** — o opcode 0xDXYN usa a interface `setPixel` do `Display` para alterar pixels e define o draw flag. Nao ha camada de renderizacao separada nesta mudança.

## Risks / Trade-offs

- [Apenas um subconjunto de opcodes implementado] -> muitas ROMs nao funcionarao completamente; e um escopo intencionalmente limitado para entregar valor incremental.
- [Desenho direto no Display sem buffering] -> piscar pode ocorrer se o display for lido durante o desenho; mitigado pelo draw flag que controla quando redesenhar.
- [Nenhuma validacao de argumento de linha de comando além da existencia do arquivo] -> arquivos invalidos (nao-ROM) podem ser carregados sem erro; mitigado pela verificacao de tamanho no carregador.

## Open Questions

- Quais opcodes adicionais devem ser implementados na mudança seguinte (logica, input, operacoes com I)? -> decidir apos validar este subconjunto basico.
- O draw flag deve ser parte da CPU ou do Display? -> deciso temporal: esta mudança coloca na CPU, mas pode ser movido futuramente.

# Design

## Context

O emulador ja possui memoria (chip8-memory, spec existente) e display (sdl3-display, spec existente). A CPU e o proxima camada: sem registradores nem ciclo de execucao, nao e possivel rodar nenhuma ROM. Veja a motivacao em `proposal.md` - Why.

## Goals / Non-Goals

**Goals:**
- Registradores V0–VF, indice I, program counter PC, pilha e stack pointer
- Ciclo fetch-decode-execute que busca opcodes da memoria
- Integracao com a memoria existente para leitura de opcodes e dados
- Interface simples para ser acionada por main.cpp em uma etapa posterior

**Non-Goals:**
- Implementacao de todos os ~35 opcodes CHIP-8 neste escopo (apenas a infraestrutura de execucao)
- Timers (delay/sound)
- Entrada de teclado
- Som
- Renderizacao no display (ja existe)

## Decisions

- **Estrutura: classe `cpu` em namespace `chip8`** — espelha o design ja adotado por `Memory` e `Display`. Alternativa: funcoes soltas; rejeitada porque o estado da CPU (registradores, PC, pilha) precisa ser encapsulado.
- **PC inicia em 0x200** — segue a convenção CHIP-8 (programas sao carregados a partir de 0x200 pela memoria ja existente).
- **Pilha fixa de 16 niveis** — e o limite definido pela especificacao original do CHIP-8.
- **Dependencia de `Memory` via inclusao de `memory.h`** — reusa a camada ja implementada em vez de duplicar logica de enderecamento.

## Risks / Trade-offs

- [Apenas infraestrutura de execucao, nenhum opcode funcional ainda] -> a CPU buscara e decodificara, mas nao havera comportamento visivel ate que opcodes sejam implementados em uma mudança posterior.
- [Nenhum opcode implementado neste escopo] -> proposital: o escopo e limitado a registradores e ciclo de execucao para manter a mudança focada e validavel.

## Open Questions

- Quais opcodes devem ser priorizados na mudança seguinte (aritmetica, desenho, input)? -> decidir antes de iniciar a implementacao de opcodes.

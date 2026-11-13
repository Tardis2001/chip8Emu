# Design

## Context

O emulador CHIP-8 já possui a camada de vídeo (`sdl3-display`, spec existente em `openspec/specs/sdl3-display/spec.md`) com janela SDL3 e display 64×32. A memória é a próxima camada: sem ela não há onde carregar ROMs nem manter estado da CPU. Veja a motivação em `proposal.md` - Why.

## Goals / Non-Goals

**Goals:**
- Memória de 4 KB (0x000–0xFFF) com leitura e escrita de byte
- Conjunto de fontes embutido em 0x000–0xFF
- Carregamento de ROM a partir de 0x200
- Interface simples o suficiente para ser usada pela CPU em uma etapa posterior

**Non-Goals:**
- Banco de páginas ou memória expandida além de 4 KB
- Proteção de escrita em regiões específicas (ex.: fontes somente-leitura)
- Implementação da CPU, opcodes, timers ou entrada

## Decisions

- **Estrutura: array de 4096 `uint8_t`** — simples, direto e reflete o modelo de memória do CHIP-8 original. Alternativa: `std::vector<uint8_t>` com alocação dinâmica; rejeitada porque o tamanho é fixo e conhecido em tempo de compilação.
- **Endereçamento a partir de 0x200 para ROMs** — segue a convenção CHIP-8 (0x000–0x1FF reservado para intérprete/fontes). Não há desvio dessa convenção neste escopo.
- **Fontes embutidas como array estático const** — os 16 dígitos hexadecimais (0–F) em 5 bytes cada, totalizando 80 bytes. Não será carregado de arquivo externo.

## Risks / Trade-offs

- [Fonte de apenas 5 px de altura pode ficar difícil de ler em displays modernos] → não é problema desta camada; cabe à renderização/escalonamento do display (já existente) ou a uma versão futura SUPER CHIP.
- [Sem proteção contra escrita em área de fontes] → o CHIP-8 original permite sobrescrever a área do intérprete; mantemos esse comportamento por simplicidade e fidelidade.

## Open Questions

(nenhum)

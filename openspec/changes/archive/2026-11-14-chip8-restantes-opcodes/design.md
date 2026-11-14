# Design

## Context

O emulador ja possui CPU com registradores, timers, teclado, e 13 opcodes implementados (0x00EE, 0x1NNN, 0x2NNN, 0x6XNN, 0x7XNN, 0xANNN, 0xDXYN, 0xFX07, 0xFX15, 0xFX18, 0xEX9E, 0xEXA1, 0xFX0A). O loop principal roda um ciclo de CPU por frame. O display ja possui metodo clearDisplay(). Ver proposal.md para motivacao.

## Goals / Non-Goals

**Goals:**
- Implementar os opcodes CHIP-8 restantes: 0x00E0 (CLS), 0x3XNN, 0x4XNN, 0x5XY0, 0x9XY0, 0x8XY* (0 a 7, E), 0xBNNN, 0xCXNN, 0xFX1E, 0xFX29, 0xFX33, 0xFX55, 0xFX65.
- Manter consistencia com a estrutura existente de dispatch por nibble alto em execute().
- Manter semantica correta de VF (carry/emprestimo/deslocamento) para opcodes 0x8XY*.

**Non-Goals:**
- Ajuste de velocidade de CPU (varios ciclos por frame) — change futuro.
- Opcodes ja implementados nao sao alterados.
- Novas dependencias externas.

## Decisions

### 1. Dispatch por sub-opcode 0x0000, 0x8000, 0xE000, 0xF000

**Decisao:** Adicionar cases dentro dos blocos switch existentes:
- 0x0000: adicionar 0x00E0 (CLS) junto ao 0x00EE existente.
- 0x8000: novo switch aninhado decodificando por kk (0x0, 0x1, ..., 0x7, 0xE).
- 0xE000: ja existe; adicionar 0x3XNN e 0x4XNN nao se encaixa aqui (sao 0x3000/0x4000, nibble alto diferente) — adicionar cases 0x3000 e 0x4000 no switch principal.
- 0xF000: ja existe; adicionar 0xFX1E, 0xFX29, 0xFX33, 0xFX55, 0xFX65 ao switch aninhado por kk.
- Novos cases 0x5000, 0x9000, 0xB000, 0xC000 no switch principal.

**Rationale:** Minimiza mudancas na estrutura existente; cada opcode fica no bloco correspondente ao seu nibble alto.

### 2. CLS (0x00E0)

**Decisao:** Chamar display.clearDisplay() e avancar PC em 2.

**Rationale:** Reutiliza metodo existente da classe Display; nao eh necessario draw flag pois CLS nao e um opcode de desenho de sprite.

### 3. Opcodes 0x8XY* (logica/aritmetica)

**Decisao:** Cada sub-opcode decodificado por kk dentro do case 0x8000. VF eh atualizado apenas para 0x8XY4 (carry), 0x8XY5 (emprestimo), 0x8XY6 (LSB), 0x8XY7 (emprestimo inverso), 0x8XYE (MSB). Para 0x8XY0/0x8XY1/0x8XY2/0x8XY3, VF nao eh alterado.

**Rationale:** Segue a especificacao CHIP-8 padrao; VF eh afetado apenas onde especificado.

### 4. Opcodes 0xFX* (memoria/indice)

**Decisao:** Adicionar ao switch aninhado existente em 0xF000:
- 0xFX1E: i += v[x] (com wraparound de 12 bits se necessario)
- 0xFX29: i = v[x] * 5 (cada sprite de fonte ocupa 5 bytes, começando em 0x000)
- 0xFX33: armazenar BCD de v[x] em memory[i], memory[i+1], memory[i+2]
- 0xFX55: armazenar v[0]..v[x] em memory[i]..memory[i+x]
- 0xFX65: carregar memory[i]..memory[i+x] em v[0]..v[x]

**Rationale:** Consistente com a implementacao existente de 0xFX07/0xFX15/0xFX18/0xFX0A.

### 5. Opcodes de salto e condicao

**Decisao:**
- 0x3XNN/0x4XNN: cases 0x3000 e 0x4000 no switch principal, comparando v[x] com kk.
- 0x5XY0/0x9XY0: cases 0x5000 e 0x9000 no switch principal, comparando v[x] com v[y] (apenas quando n == 0).
- 0xBNNN: case 0xB000, pc = nnn + v[0].
- 0xCXNN: case 0xC000, v[x] = rand() % 256 & kk.

**Rationale:** Segue o padrao de dispatch existente; cada opcode e um case independente.

## Risks / Trade-offs

- [0xFX55/0xFX65 alteram I apos execucao em algumas interpretacoes] -> Escolhemos nao alterar I (comportamento mais comum em emuladores modernos); documentado como decisao.
- [Geracao de numeros aleatorios] -> Usar std::rand() ou equivalente simples; criptograficamente seguro nao eh necessario.
- [0x00E0 nao define draw flag] -> Proposital: CLS limpa o display mas o loop renderiza todo frame de qualquer forma.

## Migration Plan

N/A — adicao de funcionalidade, sem quebra de compatibilidade. Build existente permanece valido.

## Open Questions

Nenhuma.
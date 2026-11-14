# Tasks

## 1. Opcodes de display e fluxo basico

- [x] 1.1 Implementar opcode 0x00E0 (CLS) chamando display.clearDisplay() e verificar que o display fica todo desligado
- [x] 1.2 Implementar opcode 0x3XNN (pular se Vx == NN) e verificar que avanca 4 bytes quando igual e 2 quando diferente
- [x] 1.3 Implementar opcode 0x4XNN (pular se Vx != NN) e verificar que avanca 4 bytes quando diferente e 2 quando igual
- [x] 1.4 Implementar opcode 0x5XY0 (pular se Vx == Vy) e verificar que avanca 4 bytes quando igual e 2 quando diferente
- [x] 1.5 Implementar opcode 0x9XY0 (pular se Vx != Vy) e verificar que avanca 4 bytes quando diferente e 2 quando igual
- [x] 1.6 Implementar opcode 0xBNNN (saltar para NNN + V0) e verificar que PC = NNN + V0
- [x] 1.7 Implementar opcode 0xCXNN (Vx = aleatorio & NN) e verificar que Vx recebe valor mascarado pelo byte imediato

## 2. Opcodes logico/aritmeticos 0x8XY*

- [x] 2.1 Adicionar case 0x8000 no dispatch decodificando sub-opcode por kk e verificar que o build compila
- [x] 2.2 Implementar opcode 0x8XY0 (Vx = Vy) e verificar que Vx recebe o valor de Vy
- [x] 2.3 Implementar opcode 0x8XY1 (Vx |= Vy) e verificar operacao bitwise OR
- [x] 2.4 Implementar opcode 0x8XY2 (Vx &= Vy) e verificar operacao bitwise AND
- [x] 2.5 Implementar opcode 0x8XY3 (Vx ^= Vy) e verificar operacao bitwise XOR
- [x] 2.6 Implementar opcode 0x8XY4 (Vx += Vy com carry para VF) e verificar carry em overflow e sem carry
- [x] 2.7 Implementar opcode 0x8XY5 (Vx -= Vy com emprestimo para VF) e verificar emprestimo em underflow
- [x] 2.8 Implementar opcode 0x8XY6 (Vx >>= 1, VF = LSB) e verificacao do bit deslocado
- [x] 2.9 Implementar opcode 0x8XY7 (Vx = Vy - Vx com emprestimo para VF) e verificacao de underflow
- [x] 2.10 Implementar opcode 0x8XYE (Vx <<= 1, VF = MSB) e verificacao do bit mais significativo

## 3. Opcodes de memoria e indice 0xFX*

- [x] 3.1 Implementar opcode 0xFX1E (I += Vx) e verificar que I acumula o valor de Vx
- [x] 3.2 Implementar opcode 0xFX29 (I = endereco do sprite do digito Vx) e verificar que I aponta para o conjunto de fontes
- [x] 3.3 Implementar opcode 0xFX33 (armazenar BCD de Vx em I, I+1, I+2) e verificacao dos digitos centenas/dezenas/unidades
- [x] 3.4 Implementar opcode 0xFX55 (armazenar V0..Vx na memoria a partir de I) e verificacao de escrita sequencial
- [x] 3.5 Implementar opcode 0xFX65 (carregar V0..Vx da memoria a partir de I) e verificacao de leitura sequencial

## 4. Build e verificacao de integracao

- [x] 4.1 Rebuild completo e executar com ROMs de teste, verificando que a janela abre e nao trava
- [x] 4.2 Verificar que ROMs usando os novos opcodes executam sem erro

## Workflow follow-up

- Archive the change after the project's review requirements are satisfied.
- Verify the archived result.
# Tasks

## 1. main.cpp com argv e carregador de ROM

- [x] 1.1 Modificar main.cpp para aceitar caminho de ROM via argv e verificar que o arquivo esta presente
- [x] 1.2 Implementar leitura de arquivo .ch8/.c8 do disco e gravacao na memoria a partir de 0x200, verificando que o primeiro byte fica em 0x200
- [x] 1.3 Adicionar tratamento de erro para arquivo inexistente ou ROM muito grande, verificando mensagens de erro adequadas

## 2. Opcodes CHIP-8 basicos

- [x] 2.1 Implementar opcode 0x6XNN (set Vx = NN) e verificar que Vx recebe o valor correto
- [x] 2.2 Implementar opcode 0x7XNN (add Vx += NN) e verificar que Vx soma corretamente modulo 256
- [x] 2.3 Implementar opcode 0xANNN (set I = NNN) e verificar que I recebe o endereco correto
- [x] 2.4 Implementar opcode 0x1NNN (jump to NNN) e verificar que PC salta para NNN
- [x] 2.5 Implementar opcode 0x2NNN (call subroutine) e verificar que endereco de retorno e empilhado
- [x] 2.6 Implementar opcode 0x00EE (return from subroutine) e verificar que PC e restaurado da pilha

## 3. Opcode de desenho e draw flag

- [x] 3.1 Implementar opcode 0xDXYN (draw sprite) e verificar que pixels sao desenhados no display
- [x] 3.2 Implementar draw flag (definido por desenho, lido e limpo pelo loop de renderizacao) e verificar funcionamento
- [x] 3.3 Integrar opcode de desenho com o display existente (sdl3-display) via setPixel

## 4. Build e verificacao de integracao

- [x] 4.1 Adicionar novos arquivos-fonte ao CMakeLists.txt e verificar que o projeto compila e linka
- [x] 4.2 Fazer rebuild completo e executar com uma ROM de teste para verificar que a janela abre e nao travac

## Workflow follow-up

- Revisar os artifacts de planning com o usuario antes de iniciar a implementacao
- Quando aprovado, iniciar o apply workflow com /opsx-apply

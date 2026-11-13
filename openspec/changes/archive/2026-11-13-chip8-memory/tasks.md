# Tasks

## 1. Setup e build

- [x] 1.1 Criar memory.h com interface de leitura e escrita de byte e verificar que o arquivo está presente
- [x] 1.2 Criar memory.cpp com array de 4096 bytes, adicionar ao CMakeLists.txt e verificar que o projeto compila sem erros

## 2. Memória: leitura, escrita e fontes

- [x] 2.1 Implementar leitura de byte com validação de endereço (0x000–0xFFF) e verificar que ler um endereço válido retorna o byte previamente escrito
- [x] 2.2 Implementar escrita de byte com validação de endereço e verificar que escrita seguida de leitura no mesmo endereço retorna o mesmo valor
- [x] 2.3 Embutir o conjunto de fontes do CHIP-8 nos endereços 0x000–0xFF e verificar que os bytes estão presentes após a inicialização

## 3. Carregador de ROM

- [x] 3.1 Implementar carregador que lê um arquivo .ch8 ou .c8 e grava seu conteúdo a partir de 0x200, verificando que o primeiro byte da ROM fica no endereço 0x200

## 4. Verificação de integração

- [x] 4.1 Fazer rebuild completo e executar o emulador sem ROM para verificar que a janela abre e o programa não trava

## Workflow follow-up

- Revisar os artifacts de planning com o usuário antes de iniciar a implementação
- Quando aprovado, iniciar o apply workflow com `/opsx-apply`

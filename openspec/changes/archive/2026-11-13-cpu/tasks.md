# Tasks

## 1. Registradores e estado da CPU

- [x] 1.1 Criar cpu.h com declaracao da classe cpu, registradores V0–VF, indice I, PC, pilha e stack pointer, verificando que o arquivo esta presente
- [x] 1.2 Criar cpu.cpp com inicializacao dos registradores em zero, PC em 0x200 e pilha vazia, verificando que o projeto compila sem erros

## 2. Ciclo fetch-decode-execute

- [x] 2.1 Implementar metodo de buscar um opcode da memoria (via Memory) e verificar que ele retorna os 2 bytes no endereco do PC
- [x] 2.2 Implementar metodo de decodificar o opcode e verificar que o PC avanca 2 posicoes apos a busca
- [x] 2.3 Implementar metodo executarCiclo() que busca, decodifica e executa sem falhar, verificando que roda sem travar com uma ROM carregada

## 3. Integracao com main.cpp

- [x] 3.1 Adicionar cpu.cpp ao CMakeLists.txt e verificar que o projeto compila e linka corretamente
- [x] 3.2 Fazer rebuild completo e executar o emulador sem ROM para verificar que a janela abre e o programa nao trava

## Workflow follow-up

- Revisar os artifacts de planning com o usuario antes de iniciar a implementacao
- Quando aprovado, iniciar o apply workflow com /opsx-apply

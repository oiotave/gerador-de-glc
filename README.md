# Gerador de Gramáticas Livres de Contexto (GLCs)

## Introdução
A seguinte implementação foi apresentada como projeto final da disciplina de Linguagens Formais e Computabilidade, na Universidade Federal da Paraíba (UFPB). Escrito na linguagem C, o código tem como objetivo construir cadeias de caracteres válidas dentro de uma [gramática livre de contexto](https://en.wikipedia.org/wiki/Context-free_grammar), passada pelo usuário sob uma formatação específica em arquivo de texto.

## Funcionalidades
O código fornece dois modos de produção de cadeias: no **modo detalhado**, o usuário escolhe as derivações possíveis desde o começo até alcançar uma cadeia de caracteres válida; e no **modo rápido**, o algoritmo sempre gera uma nova cadeia válida diretamente, a medida que o usuário solicita.

### Divisão
A implementação possui um fluxo de execução principal, em ```main.c```, na qual se encontram as funções tanto para o modo detalhado quanto para o modo rápido. Além dela, há duas bibliotecas auxiliares:
1. ```structs.h```: responsável por armazenar estruturas essenciais da gramática e tratá-las;
2. ```erros.h```: responsável pelo debugging e pela checagem de erros na gramática passada.

### Execução
Para a compilação do código, recomenda-se o uso de GCC e também o seguinte comando de compilação:

```gcc -o app structs.c erros.c main.c```

## Autores
O seguinte projeto foi desenvolvido por [Davi de Lacerda Teixeira](https://github.com/DavideLacerdaT) e por [João Victor Fernandes da Silveira](https://github.com/oiotave).

## Notas
O projeto contém uma pasta com duas gramáticas de teste em ```texto.txt``` e ```texto2.txt```, que podem ser usados para melhor compreensão da formatação exigida e do próprio funcionamento do código.

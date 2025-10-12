# Gerador de cadeias para Gramáticas Livres de Contexto

A seguinte implementação foi apresentada como projeto final da disciplina de Linguagens Formais e Computabilidade, na Universidade Federal da Paraíba. Seu objetivo é criar uma rotina capaz de gerar cadeias binárias de uma [gramática formal](https://en.wikipedia.org/wiki/Context-free_grammar) genérica passada em arquivo de texto pelo usuário.

## Autores

* [Davi de Lacerda Teixeira](https://github.com/DavideLacerdaT)
* [João Victor Fernandes da Silveira](https://github.com/oiotave)

## Especificação

Esse projeto desenvolve um gerador de cadeias para Gramáticas Livres de Contexto. Ou seja, dado uma GLC, o programa deve ser capaz de gerar cadeias (a pedido do usuário) que fazem parte da linguagem associada à gramática. A gramática deve ser fornecida ao programar por meio de um arquivo no seguinte formato:


```plaintext
variaveis:S,A,B     # Lista de símbolos não-terminais (variáveis)
inicial:S           # Indica o não-terminal inicial
terminais:a,b,c,d   # Lista de símbolos terminais

producoes
S: aA               # Cada produção deve ser colocada em uma linha diferente
S: bB
A: epsilon
...
```

Note que só é permitido a utilização de símbolos terminais e não-terminais com um único caractere, e que a palavra reservada “epsilon” deve ser interpretada como cadeia vazia. Além disso, é obrigatória a utilização do formato de arquivo especificado acima. O gerador deve possuir 2 modos de funcionamento: **rápido** e **detalhado**.

### Modo rápido

No modo rápido, à medida em que o usuário solicita, o programa gera uma nova cadeia e mostra a derivação mais à esquerda que foi utilizada para gerar a mesma. Note que, caso a gramática seja ambígua, é possível que uma mesma cadeia possa ser gerada por múltiplas derivações mais à esquerda. Entretanto, o programa não deve mostrar a mesma derivação mais de uma vez, o que implica que a geração da cadeia não pode ser feita de forma aleatória.

### Modo detalhado
Já no modo detalhado, o programa funciona quase como um jogo para realizar uma única derivação. Essa derivação inicia pelo não-terminal inicial e, a cada etapa, o programa mostra ao usuário quais produções podem ser utilizadas para a próxima substituição e o deixa escolher uma delas. Note que, a próxima substituição deve ser sempre a do não-terminal mais à esquerda.

## Estrutura do projeto

Além da função principal "main", que contém as rotinas para os modos rápido e detalhado o projeto possui duas bibliotecas auxiliares:

* `structs.h`: responsável por armazenar estruturas essenciais da gramática e tratá-las
* `erros.h`: responsável pela checagem de erros na gramática passada

## Instruções de compilação e execução

1. Clone o repositório localmente em sua máquina

```bash
git clone "URL_DO_REPOSITÓRIO"
```

2. Com a linha de comando no diretório `src`, compile o código

```bash
gcc structs.c erros.c main.c -o app 
```
3. Execute o código após a compilação

```bash
./app
```

**Observação**: O projeto contém uma pasta com duas gramáticas de teste em "texto.txt" e "texto2.txt"


# Gerador de Gramáticas Livres de Contexto (GLC)

Projeto final da disciplina de Linguagens Formais e Computabilidade, do 3º período da Universidade Federal da Paraíba

### Autores:

* [Davi de Lacerda Teixeira](https://github.com/DavideLacerdaT)
* [João Victor Fernandes da Silveira](https://github.com/oiotave)

Seu objetivo é executar a geração de cadeias em uma gramática livre de contexto, fornecendo dois modos de produção:

* Detalhado: usuário escolhe as derivações até a finalização da cadeia
* Rápido: gera sempre uma cadeia toda vez que o usuário solicita

Para a compilação do código, sobretudo se for por meio do compilador GCC, recomenda-se o seguinte comando:

    gcc -o app structs.c erros.c main.c

O projeto contém uma pasta com duas gramáticas de teste em "texto.txt" e "texto2.txt"

#ifndef ESTRUTURAS_H_INCLUDED
#define ESTRUTURAS_H_INCLUDED

// Lista para guardar todas as variaveis da gramatica (maximo de 20)
struct tVariaveis {
    char vetor[20];
    int controlador; // Controlador para indicar o numero efetivo de variaveis da gramatica
};

typedef struct tVariaveis Variaveis;

// Lista para guardar todos os simbolos terminais da gramatica (maximo de 20)
struct tTerminais {
    char vetor[20];
    int controlador; // Controlador para indicar o numero efetivo de terminais da gramatica
};

typedef struct tTerminais Terminais;

// Lista encadeada para guardar cada uma das producoes da gramatica
struct tProducoes {
    char producao[20];          // Armazena a producao
    char variavel;              // Armazena a variavel que gera a producao
    int estadoArmadilha;        // Flag que determina se a producao leva a loops
    struct tProducoes *proximo; // Ponteiro para o proximo elemento
};

typedef struct tProducoes Producoes;

// Struct que reune todos os tipos acima
struct tGramatica {
    Variaveis variaveis;
    Terminais terminais;
    Producoes *producoes;
    char inicial; // Armazena a variavel inicial da gramatica
};

typedef struct tGramatica Gramatica;

// Inicializa os campos da gramatica
void construtor(Gramatica*);

// Adiciona as producoes da gramatica como lista encadeada
void adicionar_producao(Gramatica*, char*);

// Libera a memoria alocada para finalizar o programa
void limpar_producao(Gramatica*);

#endif // ESTRUTURAS_H_INCLUDED

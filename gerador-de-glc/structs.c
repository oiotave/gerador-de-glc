#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

// Inicializa a gramatica
void construtor(Gramatica *gramatica) {
    // Vetores inicializados como vazios
    gramatica->variaveis.vetor[0] = '\0';
    gramatica->terminais.vetor[0] = '\0';
    gramatica->inicial = '\0';

    // Contadores e ponteiros inicializados como nulos
    gramatica->variaveis.controlador = -1;
    gramatica->terminais.controlador = -1;
    gramatica->producoes = 0;
}

// Adiciona as producoes da gramatica como lista encadeada
void adicionar_producao(Gramatica *gramatica, char *linha) {
    // Cria um novo espaco para guardar a producao
    Producoes *resultado = (Producoes*) malloc(sizeof(Producoes));
    if(!resultado) {
        printf("\033[31m\nErro na alocacao de memoria!\033[0m\n");
        return;
    }

    // Le a linha no formato: "S: producao"
    sscanf(linha, "%c: %[^\n]", &resultado->variavel, resultado->producao);

    // Poe a proxima producao como 0, por padrao
    resultado->proximo = 0;

    // Adiciona a producao como novo ultimo elemento na lista encadeada
    if(!gramatica->producoes)
        gramatica->producoes = resultado;

    else {
        Producoes *aux = gramatica->producoes;
        while(aux->proximo != 0)
            aux = aux->proximo;

        aux->proximo = resultado;
    }
}

// Libera a memoria alocada para finalizar o programa
void limpar_producao(Gramatica *gramatica) {
    Producoes *aux, *temp;

    aux = gramatica->producoes;

    // Libera o espaco de memoria alocado para guardar as producoes
    while(aux) {
        temp = aux;
        aux = aux->proximo;

        free(temp);
    }
}

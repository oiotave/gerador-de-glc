#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

void construtor(Gramatica *gramatica) {
    gramatica->variaveis.vetor[0] = '\0';
    gramatica->terminais.vetor[0] = '\0';
    gramatica->inicial = '\0';
    gramatica->variaveis.controlador = -1;
    gramatica->terminais.controlador = -1;
    gramatica->producoes = 0;
}

void adicionar_producao(Gramatica *gramatica, char *linha) {
    Producoes *resultado = (Producoes*) malloc(sizeof(Producoes));
    if(!resultado) {
        printf("\033[31m\nErro na alocacao de memoria!\033[0m\n");
        return;
    }
    sscanf(linha, "%c: %[^\n]", &resultado->variavel, resultado->producao);
    resultado->proximo = 0;

    // Adiciona a producao como novo ultimo elemento na lista encadeada
    if(!gramatica->producoes) gramatica->producoes = resultado;

    else {
        Producoes *aux = gramatica->producoes;
        while(aux->proximo != 0) aux = aux->proximo;
        aux->proximo = resultado;
    }
}

void limpar_producao(Gramatica *gramatica) {
    Producoes *aux, *temp;
    aux = gramatica->producoes;

    while(aux) {
        temp = aux;
        aux = aux->proximo;
        free(temp);
    }
}


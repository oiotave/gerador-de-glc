#include <stdio.h>
#include <string.h>
#include "structs.h"
#include "erros.h"

// Verifica se algum parametro deixou de ser passado no arquivo
int verifica_arquivo(Gramatica *gramatica) {
    int flag = 0;   // Flag para guardar situacao de erro: '0' se nao houver, '1' se houver

    // Verifica se o vetor das variaveis esta vazio.
    if(gramatica->variaveis.vetor[0] == '\0') {
        printf("\033[31m\nNao ha variaveis para serem lidas!\033[0m\n");
        flag = 1;
    }
    // Verifica se ha variavel inicial.
    if(gramatica->inicial == '\0') {
        printf("\033[31m\nNao ha variavel inicial!\033[0m\n");
        flag = 1;
    }
    // Verifica se o vetor dos terminais esta vazio.
    if(gramatica->terminais.vetor[0] == '\0') {
        printf("\033[31m\nNao ha terminais para serem lidos!\033[0m\n");
        flag = 1;
    }
    // Verifica se ha producoes lidas.
    if(!gramatica->producoes) {
        printf("\033[31m\nNao ha producaes para serem lidas!\033[0m\n");
        flag = 1;
    }
    return flag;
}

// Verifica casos de loops simples
int verifica_loops(Gramatica *gramatica) {
    Producoes *aux = gramatica->producoes;      // Variavel usada para percorrer todas as producoes
    char* var = gramatica->variaveis.vetor;     // Variavel usada para pecorrer todos os nao-terminais
    int contador = 0, num_derivacoes = 0;       // Contadores para verificacao de erros

    // Verifica se nenhuma das producoes e capaz de terminar a cadeia
    while(aux) {
        // A cada nova producao, 'num_derivacoes' e incrementado
        num_derivacoes++;

        // Se na producao atual houver pelo menos um nao-terminal, incrementa-se o contador
        for(int i = 0; i <= gramatica->variaveis.controlador; i++) {
            if(strchr(aux->producao, var[i]))
                contador++;
        }
        aux = aux->proximo;
    }

    /*  
     *  Caso o numero de producoes totais coincida com o numero de producoes que contem um nao-terminal,
     *  isso significa que toda producao sempre vai gerar uma derivacao com ao menos um simbolo nao-terminal
     *  Portanto, a cadeia nunca podera ser encerrada
     */
    if(num_derivacoes == contador) return 1;

    // Verifica se uma variavel leva apenas a derivacoes dela propria
    for(int i = 0; i <= gramatica->variaveis.controlador; i++) {
        contador = 0;
        num_derivacoes = 0;

        // Para cada nao-terminal sao vistas as producoes que partem dele
        for(aux = gramatica->producoes; aux; aux = aux->proximo) {
            // Se a producao deriva do nao-terminal atual, incrementa-se 'num_derivacoes'
            if(var[i] == aux->variavel) {
                num_derivacoes++;

                // Se na producao gerada por esse nao-terminal, ele deriva para ele proprio, incrementa-se o contador
                if(strchr(aux->producao, var[i])) contador++;
            }
        }
        /*  
         *  Caso ambos os contadores coincidam, significa que, para aquele simbolo nao-terminal, todas as producoes
         *  que partem dele derivam nele proprio e, portanto, nunca se podera encerrar a cadeia
         */
        if((num_derivacoes == contador) && num_derivacoes != 0) return 1;
    }
    return 0;
}

// Verifica casos de armadilha mais complexas
int verifica_armadilha(Gramatica *gramatica) {
    int contador = 0, contadorArmadilha = 0;
    char resultado[2];

    resultado[0] = '\0';

    Producoes *aux;

    // Incrementa o contador toda vez que lê uma produção e uma variável.
    for(aux = gramatica->producoes; aux; aux = aux->proximo) {
        contador++;
        for(int i = 0; i < (int) strlen(aux->producao); i++) {
            for(int j = 0; j <= gramatica->variaveis.controlador; j++) {
                if(aux->producao[i] == gramatica->variaveis.vetor[j]) contador++;
            }
        }
    }
    // Se o contador for maior que 20, ignora a verificação de produções armadilhas.
    if(contador > 20) {
        for(aux = gramatica->producoes; aux; aux = aux->proximo)
            aux->estadoArmadilha = 0;
    }
    else {
        // Envia cada producao para a funcao estado_armadilha.
        for(aux = gramatica->producoes; aux; aux = aux->proximo) {
            int armadilha = 1;
            int *ponteiroArmadilha;

            ponteiroArmadilha = &armadilha;

            estado_armadilha(gramatica, aux->variavel, aux->producao, resultado, contador, ponteiroArmadilha);

            if(armadilha == 1) {
                aux->estadoArmadilha = 1;
                contadorArmadilha++;
            }
            else aux->estadoArmadilha = 0;
        }
        // Conta quantas produções tem
        contador = 0;
        for(aux = gramatica->producoes; aux; aux = aux->proximo)
            contador++;

        // Compara a quantidade de produções com a quantidade de armadilhas, e encerra o programa se forem iguais
        if(contador == contadorArmadilha) return 1;
        
        if(contador == contadorArmadilha) {
            printf("Producoes invalidas.");
            return 1;
        }
    }
    // Retorna '0' se tudo ocorrer bem.
    return 0;
}

// Verifica potenciais erros na escrita do arquivo
int verifica_producoes(Gramatica *gramatica) {
    Producoes *aux = gramatica->producoes;

    int controle;

    // Verifica se ha uma producao que parte de uma variavel nao declarada
    for(aux = gramatica->producoes; aux; aux = aux->proximo) {
        controle = 0;

        // Para cada producao, verifica se a variavel que a deriva foi declarada
        for(int i = 0; i <= gramatica->variaveis.controlador; i++) {
            // Se a variavel foi declarada, incrementa o controle
            if(gramatica->variaveis.vetor[i] == aux->variavel) controle++;
        }
        // Caso a variavel de origem da derivacao nao coincida com nenhum, ela nao foi declarada
        if(!controle) return 1;
    }

    // Verifica se em alguma producao ha alguma variavel invalida
    for(aux = gramatica->producoes; aux; aux = aux->proximo) {
        // Se a derivacao for "epsilon", verifica a proxima
        if(!strcmp(aux->producao, "epsilon")) continue;

        // Verifica cada caractere da derivacao atual e checa se todos os caracteres sao validos
        for(int i = 0; i < (int) strlen(aux->producao); i++) {
            if(!strchr(gramatica->terminais.vetor, aux->producao[i])) {
                // Caso o caractere nao seja nem variavel nem terminal declarado, ele nao foi declarado anteriormente
                if(!strchr(gramatica->variaveis.vetor, aux->producao[i])) return 1;
            }
        }
    }
    return 0; // Retorna '0' caso nenhum erro tenha sido detectado
}

// Verifica a existencia de estados armadilha (funcionamento equivalente ao do modo rapido)
void estado_armadilha(Gramatica *gramatica, char simbolo, char *producao, char *resultado, int iteracoes, int *armadilha) {
    // Se passar o limite de iteracoes, nao permite que a funcao prossiga
    if(!iteracoes) return;

    // Flag para indicar se a producao e invalida ('0' para nao, '1' para sim)
    int invalida = 0;

    Producoes *aux;

    // Cria cópias das variáveis recebidas
    char copiaProducao[20];
    strcpy(copiaProducao, producao);

    char copiaResultado[200];
    strcpy(copiaResultado, resultado);

    aux = gramatica->producoes;
    while(aux) {
        if (aux->variavel == simbolo) {
            if((strcmp(aux->producao, "epsilon") == 0) && (strcmp(copiaProducao, "epsilon") == 0)) {
                (*armadilha) = 0;
                return;
            }
            if((strcmp(aux->producao, copiaProducao) == 0)) {
                if(copiaResultado[0] == '\0')
                    strcpy(copiaResultado, copiaProducao);
                else {
                    for(int i = 0; i < (int) strlen(copiaResultado); i++) {
                        if((copiaResultado[i] == simbolo)) {
                            if((int)strlen(copiaProducao) > 1) {
                                for(int j = (int) strlen(copiaResultado); j >= i; j--)
                                    copiaResultado[j + ((int)strlen(copiaProducao) - 1)] = copiaResultado[j];
                            }
                            for(int j = 0; j < (int) strlen(copiaProducao); j++)
                                copiaResultado[i + j] = copiaProducao[j];

                            break;
                        }
                    }
                }
                break;
            }
        }
        aux = aux->proximo;
    }

    for(int i = 0; i < (int) strlen(copiaResultado); i++) {
        for(int j = 0; j <= gramatica->variaveis.controlador; j++) {
            if(copiaResultado[i] == gramatica->variaveis.vetor[j]) {
                simbolo = copiaResultado[i];
                invalida = 1;
                break;
            }
        }
        if(invalida) break;
    }
    if(!invalida) {
        (*armadilha) = 0;
        return;
    }

    aux = gramatica->producoes;
    while(aux) {
        if(aux->variavel == simbolo)
            estado_armadilha(gramatica, simbolo, aux->producao, copiaResultado, iteracoes - 1, armadilha);

        aux = aux->proximo;
    }
}

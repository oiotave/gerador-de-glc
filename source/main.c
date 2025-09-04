#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "erros.h"

int flag = 0; // Flag usada na recursao do modo rapido para indicar cadeias finalizadas

// Funcoes principais de execucao dos modos
int  ler_arquivo(Gramatica*);
void modo_rapido(Gramatica*, char, char*, char*, char*, int);
void modo_detalhado(Gramatica*);

// Funcoes visuais auxiliares
void clear();
void menu_abertura();
void avisos();

int main(void) {
    clear();

    // Strings de inicializacao do modo rapido,
    char placeHolder[2], string[5];

    // Variaveis inteiras usadas na funcao
    int iteracoes, opcao;

    // Inicializa a gramatica do usuario
    Gramatica gramatica;
    construtor(&gramatica);
    menu_abertura();
    avisos();

    int aux = ler_arquivo(&gramatica);

    // Loop de leitura do arquivo passado pelo usuario
    while(aux) {
        if(aux == -1) {
            clear();
            return 0;
        }
        // Caso haja algum erro, limpa a memoria alocada e pede nova leitura
        limpar_producao(&gramatica);
        aux = ler_arquivo(&gramatica);
        
        if(!aux) break;
    }

    // Sequencia de verificacao de erros
    if(verifica_arquivo(&gramatica)) {
        limpar_producao(&gramatica);
        return 0;
    }
    if(verifica_producoes(&gramatica)) {
        printf("\033[31m\nExiste algum erro de sintaxe nas producoes!\033[0m\n");
        limpar_producao(&gramatica);
        return 0;
    }
    if(verifica_loops(&gramatica) || verifica_armadilha(&gramatica)) {
        printf("\033[31m\nA gramatica passada potencialmente gera loops!\033[0m\n");
        limpar_producao(&gramatica);
        return 0;
    }
    // Permite ao usuario escolher como proceder no codigo
    do {
        printf("\033[33m\n(1) - Modo rapido\033[0m\n");
        printf("\033[33m\n(2) - Modo detalhado\033[0m\n");
        printf("\033[33m\n(0) - Sair do Programa\033[0m\n\n");
        printf("Digite uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                flag = 0; // Reinicia o valor da flag de recursao

                printf("\nDigite a quantidade maxima de iteracoes: ");
                scanf("%d", &iteracoes);
                printf("\n");

                placeHolder[0] = '\0';

                sprintf(string, "%c", gramatica.inicial);
                Producoes *aux = gramatica.producoes;

                while(aux) {
                    if(aux->variavel == gramatica.inicial)
                        modo_rapido(&gramatica, gramatica.inicial, aux->producao, placeHolder, string, iteracoes);
                    aux = aux->proximo;
                }
                printf("Todas as producoes foram mostradas!\n");

                break;

            case 2:
                modo_detalhado(&gramatica);
                break;

            case 0:
                clear();
                limpar_producao(&gramatica);
                break;

            default:
                printf("\033[31m\nOpcao nao suportada!\033[0m\n\n");
                break;
        }
    }
    while(opcao);
    return 0;
}

int ler_arquivo(Gramatica *gramatica) {
    // Flag para indicar se o programa deve comecar a ler as producoes
    int modoLerProducoes = 0;

    char linha[100], caminho_arquivo[1000];
    FILE *arquivo;

    // Loop de leitura do arquivo
    while(1) {
        printf("\nDigite o caminho do arquivo: ");
        fgets(caminho_arquivo, 1000, stdin);

        // Remove o '\n' ao final da string do diretorio
        caminho_arquivo[strcspn(caminho_arquivo, "\n")] = 0;

        arquivo = fopen(caminho_arquivo, "r");

        // Repete a leitura caso o diretorio passado tenha algum problema
        if(!arquivo) {
            int opt;

            printf("\033[31m\nArquivo nao encontrado ou com problemas!\033[0m\n");
            printf("\nDigite '1' para tentar de novo ou '0' para sair: ");

            // Segundo loop de leitura ate receber um digito valido
            while(1) {
                scanf("%d", &opt);

                if(opt != 0 && opt != 1) {
                    printf("\nOpcao nao encontrada. Tente novamente: ");
                    fgets(linha, 1000, stdin);
                }
                else break;
            }
            if(!opt) return -1;
            else getchar();
        }
        else break;
    }
    while(fgets(linha, 100, arquivo)) {
        linha[strcspn(linha, "\n")] = 0;
        if(linha[0] == '\0') continue;

        // Le as variaveis da gramatica
        if(strncmp(linha, "variaveis:", 10) == 0) {
            strcpy(gramatica->variaveis.vetor, linha + 10);
            gramatica->variaveis.controlador = strlen(gramatica->variaveis.vetor) - 1;

            for(int i = gramatica->variaveis.controlador; i >= 0; i--){
                // Remove as virgulas do vetor
                if(gramatica->variaveis.vetor[i] == ',') {
                    for(int j = i; j <= gramatica->variaveis.controlador; j++)
                        gramatica->variaveis.vetor[j] = gramatica->variaveis.vetor[j + 1];

                    gramatica->variaveis.controlador = strlen(gramatica->variaveis.vetor) - 1;
                }
            }
        }
        // Le a variavel inicial da gramatica
        else if(strncmp(linha, "inicial:", 8) == 0) gramatica->inicial = linha[8];

        // Le os terminais da gramatica
        else if(strncmp(linha, "terminais:", 10) == 0) {
            strcpy(gramatica->terminais.vetor, linha + 10);
            gramatica->terminais.controlador = strlen(gramatica->terminais.vetor) - 1;

            for(int i = gramatica->terminais.controlador; i >= 0; i--){
                // Remove as vírgulas do vetor
                if(gramatica->terminais.vetor[i] == ',') {
                    for(int j = i; j <= gramatica->terminais.controlador; j++)
                        gramatica->terminais.vetor[j] = gramatica->terminais.vetor[j + 1];

                    gramatica->terminais.controlador = strlen(gramatica->terminais.vetor) - 1;
                }
            }
        }

        // Ativa o modo que le as producoes, a partir da proxima linha
        else if(strncmp(linha, "producoes", 9) == 0) modoLerProducoes = 1;

        // Recusa o arquivo se uma producao esta em outro formato
        else if((modoLerProducoes == 1) && (linha[1] != ':')) {
            printf("\033[31m\nO arquivo possui producao invalida!\033[0m\n");
            return 1;
        }
        // Le as producoes ate o final do arquivo
        else if(modoLerProducoes) adicionar_producao(gramatica, linha);
    }
    fclose(arquivo);
    return 0;
}

void modo_detalhado(Gramatica *gramatica) {
    clear();

    // 'Variaveis' e 'valida' sao flags de execucao
    int opcao, variaveis = 0, valida;

    char simbolo;           // Armazena sempre o nao-terminal mais a esquerda
    char escolha[20];       // Armazena a producao escolhida pelo usuario naquela iteracao
    char resultado[200];    // Armazena a cadeia depois da ultima derivacao
    char string[1000];      // Armazena todo o processo de derivacao, do inicial ate a cadeia final

    resultado[0] = '\0';
    string[0] = '\0';
    simbolo = gramatica->inicial;

    sprintf(string, "%c", simbolo); // Comeca montando com a variavel inicial.
    Producoes *aux;

    // Loop interativo de montagem. Termina se for colocado um epsilon ou uma producao que nao leva a nada.
    do {
        // Reinicia aux para pegar todas as producoes
        aux = gramatica->producoes;
        valida = 0;

        // Mostra as possibilidades de escrita
        printf("\n");
        while(aux) {
            // Mostra na tela todas as producoes cuja variavel de origem for o simbolo atual
            if(aux->variavel == simbolo) {
                printf("\033[33m%c: %s\033[0m", aux->variavel, aux->producao);

                if(aux->estadoArmadilha) printf(" (armadilha)\n\n");
                
                else printf("\n\n");
            }
            aux = aux->proximo;
        }
        // Deixa ao usuario escolher uma producao
        printf("Digite uma producao: ");
        scanf("%s", escolha);
        printf("\n");
        aux = gramatica->producoes;

        while(aux) {
            // Encontra a producao escolhida pelo usuario na lista de producoes
            if(strcmp(aux->producao, escolha) == 0 && aux->variavel == simbolo) {
                // Se a producoes levar a um loop, encerra a execucao do modo detalhado
                if(aux->estadoArmadilha) {
                    printf("Voce entrou em um loop! Voltando ao menu...\n");
                    return;
                }
            }
            aux = aux->proximo;
        }
        // Verifica se a string e valida e seleciona o tratamento correto.
        aux = gramatica->producoes;
        while(aux) {
            // Verifica se a variavel de origem da producao e igual ao simbolo atual
            if(aux->variavel == simbolo) {
                // Caso a producao dessa variavel escolhida pelo usuario tenha sido 'epsilon', faz o tratamento
                if((strcmp(aux->producao, "epsilon") == 0) && (strcmp(escolha, "epsilon") == 0)) {
                    valida = 1;

                    // Se essa nao for a primeira iteracao, some com a variavel em questao e arruma o resultado
                    if(resultado[0] != '\0') {
                        for(int i = 0; i < (int) strlen(resultado); i++) {
                            // Percorre ate achar o caractere que corresponde ao simbolo em questao
                            if((resultado[i] == simbolo)) {
                                for(int j = i; j < (int) strlen(resultado); j++)
                                    resultado[j] = resultado[j + 1]; // Move tudo a direita desse simbolo uma posicao a esquerda, sumindo com o simbolo
                                
                                break;
                            }
                        }
                    }
                    break;
                }
                // Caso a producao da variavel seja alguma outra, faz outro tratamento
                if((strcmp(aux->producao, escolha) == 0)) {
                    valida = 1;

                    // Se essa for a primeira iteracao, apenas copia a derivacao escolhida no resultado
                    if(resultado[0] == '\0') strcpy(resultado, escolha);

                    // Senao, substitui a variavel pela derivacao no resultado
                    else {
                        for(int i = 0; i < (int) strlen(resultado); i++) {
                            // Percorre a cadeia ate encontrar a variavel em questao
                            if((resultado[i] == simbolo)) {
                                // Move o restante dos caracteres para a esquerda de acordo com o tamanho da derivacao escolhida
                                if((int) strlen(escolha) > 1) {
                                    for(int j = (int) strlen(resultado); j >= i; j--)
                                        resultado[j + ((int) strlen(escolha) - 1)] = resultado[j];
                                }
                                // Preenche o espaco no meio com os caracteres da derivacao escolhida pelo usuario
                                for(int j = 0; j < (int) strlen(escolha); j++)
                                    resultado[i + j] = escolha[j];
                                
                                break;
                            }
                        }
                    }
                    break;
                }
            }
            aux = aux->proximo;
        }
       // Se a escolha foi invalida, permite que o usuario tente novamente ou volte ao menu
        if (valida == 0) {
            printf("\033[31m\nProducao escolhida invalida!\033[0m\n");
            printf("\033[33m\n(0) - Voltar ao Menu\033[0m\n");
            printf("\033[33m\n(1) - Tentar de novo\033[0m\n\n");
            printf("Digite uma opcao: ");
            scanf("%d", &opcao);

            if(!opcao) return;
            else variaveis = 1;
        }
        else {
            // Monta a string completa
            strcat(string, " -> ");
            strcat(string, resultado);
            printf("\033[32m%s\033[m\n", string);

            variaveis = 0;

            // Verifica se ainda tem variaveis para serem derivadas e guarda elas em um vetor
            for(int j = 0; j < (int) strlen(resultado); j++) {
                for(int i = 0; i <= gramatica->variaveis.controlador; i++) {
                    if(resultado[j] == gramatica->variaveis.vetor[i]) {
                        variaveis = 1;
                        // Atualiza 'simbolo' para que ele guarde o nao terminal mais a esquerda
                        simbolo = resultado[j];
                        break;
                    }
                }
                if(variaveis) break;
            }
        }
    }
    while(variaveis >= 1); // Repete o laco enquanto a flag 'variaveis' for verdadeira
    
    printf("\033[32m\nCadeia formada com sucesso!\033[0m\n\n");
}

void modo_rapido(Gramatica *gramatica, char simbolo, char *producao, char *resultado, char *string, int iteracoes) {
    // Se a flag estiver ativada, retorna todas as chamadas
    if(flag || iteracoes <= 0) return;

    int invalida = 0;
    Producoes *aux;

    char copiaProducao[20], copiaResultado[200], copiaString[1000];

    // Cria copias das variaveis recebidas
    strcpy(copiaProducao, producao);
    strcpy(copiaResultado, resultado);
    strcpy(copiaString, string);

    // Verifica se a string e valida e seleciona o tratamento correto
    aux = gramatica->producoes;
    while(aux) {
        // Encontra a variavel de origem do simbolo atual
        if(aux->variavel == simbolo) {
            // Se for 'epsilon', remove a variavel e arruma a string final
            if((strcmp(aux->producao, "epsilon") == 0) && (strcmp(copiaProducao, "epsilon") == 0)) {
                if(copiaResultado[0] != '\0') {
                    for(int i = 0; i < (int)strlen(copiaResultado); i++) {
                        if((copiaResultado[i] == simbolo)) {
                            for(int j = i; j < (int)strlen(copiaResultado); j++)
                                copiaResultado[j] = copiaResultado[j + 1];
                            break;
                        }
                    }
                }
                break;
            }

            // Se for outra, substitui a variavel pela producao e arruma a string
            if((strcmp(aux->producao, copiaProducao) == 0)) {
                if(copiaResultado[0] == '\0') strcpy(copiaResultado, copiaProducao);
                
                else {
                    for(int i = 0; i < (int)strlen(copiaResultado); i++) {
                        if((copiaResultado[i] == simbolo)) {
                            if((int)strlen(copiaProducao) > 1) {
                                for(int j = (int)strlen(copiaResultado); j >= i; j--)
                                    copiaResultado[j + ((int)strlen(copiaProducao) - 1)] = copiaResultado[j];
                            }

                            for(int j = 0; j < (int)strlen(copiaProducao); j++)
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
    strcat(copiaString, " -> ");
    strcat(copiaString, copiaResultado);

    // Verifica se ha alguma variavel para ser derivada
    for(int i = 0; i < (int)strlen(copiaResultado); i++) {
        for(int j = 0; j <= gramatica->variaveis.controlador; j++) {
            if(copiaResultado[i] == gramatica->variaveis.vetor[j]) {
                simbolo = copiaResultado[i];
                invalida = 1;
                break;
            }
        }
        if(invalida) break;
    }
    // Se a producao for valida, printa uma derivacao possivel
    if(!invalida) {
        int opcao;

        printf("\033[32m%s\033[m\n", copiaString);
        printf("\nDeseja obter uma nova cadeia? ('0' se nao, '1' se sim) - ");
        scanf("%d", &opcao);
        printf("\n");

        // Se o usuario quiser uma nova cadeia, chama mais uma recursao
        if(opcao) return;

        // Senao, muda o valor de flag para 1, encerrando a recursao
        else if(!opcao) {
            flag = 1;
            clear();
        }
    }

    // Verifica se ainda tem variaveis para serem derivadas e guarda elas em um vetor
    aux = gramatica->producoes;
    while(aux) {
        if(aux->variavel == simbolo) {
            if(aux->estadoArmadilha) return;
            modo_rapido(gramatica, simbolo, aux->producao, copiaResultado, copiaString, iteracoes - 1);
        }
        aux = aux->proximo;
    }
}

// Limpa o terminal
void clear() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Texto de abertura
void menu_abertura() {
    printf("\033[33m  _____                        _                      _             _____  _       _____ \n\033[0m");
    printf("\033[33m / ____|                      | |                    | |           / ____|| |     / ____|\n\033[0m");
    printf("\033[33m| |  __   ___  _ __  __ _   __| |  ___   _ __      __| |  ___     | |  __ | |    | |     \n\033[0m");
    printf("\033[33m| | |_ | / _ \\| '__|/ _` | / _` | / _ \\ | '__|    / _` | / _ \\    | | |_ || |    | |     \n\033[0m");
    printf("\033[33m| |__| ||  __/| |  | (_| || (_| || (_) || |      | (_| ||  __/    | |__| || |____| |____ \n\033[0m");
    printf("\033[33m \\_____| \\___||_|   \\__,_| \\__,_| \\___/ |_|       \\__,_| \\___|     \\_____||______|\\_____|\n\n\033[0m");
}

// Painel de avisos
void avisos() {
    printf("\nEste codigo nao suporta qualquer tamanho de vetores e pode ter problemas se executar gramaticas muito grandes\n");
    printf("\nAo utilizar esse programa, tente seguir os avisos abaixo:\n");


    printf("\033[31m\n1. Recomenda-se gramaticas de, no maximo, 20 variaveis e 20 terminais\n\033[0m");
    printf("\033[31m\n2. Producoes muito grandes podem exceder o limite de memoria estabelecido, gerando derivaoces incompletas\n\033[0m");
    printf("\033[31m\n3. Recomenda-se cuidado a producoes de profundidade maior ou igual a 10\n\033[0m");

    printf("\033[33m\nGrupo: Davi de Lacerda Teixeira (20230012251) e Joao Victor Fernandes da Silveira (20230012298) \n\033[0m");
}



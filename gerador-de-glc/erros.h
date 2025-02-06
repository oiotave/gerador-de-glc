#ifndef VERIFICADORES_H_INCLUDED
#define VERIFICADORES_H_INCLUDED

#include "structs.h"

// Verifica se algum parametro deixou de ser passado no arquivo
int verifica_arquivo(Gramatica*);

// Verifica casos de loops simples
int  verifica_loops(Gramatica*);

// Verifica casos de armadilha mais complexas
int verifica_armadilha(Gramatica*);

// Verifica potenciais erros na escrita do arquivo
int verifica_producoes(Gramatica*);

// Verifica a existencia de estados armadilha
void estado_armadilha(Gramatica*, char, char*, char*, int, int*);

#endif // VERIFICADORES_H_INCLUDED

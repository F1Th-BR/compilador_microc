#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "tokens.h"

typedef struct {
    FILE *fonte;    // Aponta para o arquivo aberto ou para o stdin
    int linha;      // Contador da linha atual
} Scanner;

// Inicializa o estado do scanner com o fluxo de entrada
void scanner_init(Scanner *scanner, FILE *input);

// Retorna o próximo token válido encontrado no fluxo
Token prox_token(Scanner *scanner);

#endif
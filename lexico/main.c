#include <stdio.h>
#include "tokens.h"
#include "scanner.h"

// Mapeia o valor do enum para a string exata do nome do token
const char* token_para_string(TokenType tipo) {
    switch (tipo) {
        case TK_ID:      return "TK_ID";
        case TK_NUM:     return "TK_NUM";
        case TK_FLOAT:   return "TK_FLOAT";
        case TK_STRING:  return "TK_STRING";
        case TK_PLUS:    return "TK_PLUS";
        case TK_MINUS:   return "TK_MINUS";
        case TK_TIMES:   return "TK_TIMES";
        case TK_DIV:     return "TK_DIV";
        case TK_MOD:     return "TK_MOD";
        case TK_ASSIGN:  return "TK_ASSIGN";
        case TK_EQ:      return "TK_EQ";
        case TK_LT:      return "TK_LT";
        case TK_LEQ:     return "TK_LEQ";
        case TK_GT:      return "TK_BT";
        case TK_GEQ:     return "TK_BEQ";
        case TK_SEMI:    return "TK_SEMI";
        case TK_SCOM:    return "TK_SCOM";
        case TK_MCOM:    return "TK_MCOM";
        case TK_LPAREN:  return "TK_LPAREN";
        case TK_RPAREN:  return "TK_RPAREN";
        case TK_IF:      return "TK_IF";
        case TK_ELSE:    return "TK_ELSE";
        case TK_FOR:     return "TK_FOR";
        case TK_WHILE:   return "TK_WHILE";
        case TK_EOF:     return "TK_EOF";
        default:         return "TK_ERROR";
    }
}

int main(int argc, char *argv[]) {
    FILE *entrada = stdin;

    if (argc > 1) {
        entrada = fopen(argv[1], "r");
        if (!entrada) {
            fprintf(stderr, "Erro ao abrir o arquivo '%s'\n", argv[1]);
            return 1;
        }
    }

    Scanner scanner;
    scanner_init(&scanner, entrada);

    Token token;
    int primeiro = 1;

    do {
        token = prox_token(&scanner);

        // Imprime o token (inclui comentários, TK_EOF e TK_ERROR)
        if (!primeiro) printf(" ");
        if (token.tipo == TK_ERROR) {
            printf("(TK_ERROR,%s,linha: %d)", token.lexema, token.linha);
        } else {
            printf("(%s,%s)", token_para_string(token.tipo), token.lexema);
        }
        primeiro = 0;


    } while (token.tipo != TK_EOF);

    printf("\n");

    if (entrada != stdin) {
        fclose(entrada);
    }

    return 0;
}
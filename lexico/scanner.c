#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"


// Funções auxiliares

// Lê o caractere atual e avança no arquivo
static char avanca(Scanner *s) {
    char c = fgetc(s->fonte);
    if (c == '\n')
        s->linha++;

    return c;
}

// Olha o próximo caractere sem consumí-lo
static char prox_char(Scanner *s) {
    char c = fgetc(s->fonte);
    ungetc(c, s->fonte);
    return c;
}

/*  Função para verificar se um token
marcado como identificador (ID) é,
de fato, um identificador ou uma
palavra reservada.

@param lexema => o lexema a ser avaliado 
@return TokenType => um token de palavra
reservada ou TK_ID
*/
static TokenType ver_palavra_res(const char *lexema) {
    // Verifica cada tipo de palavra reservada (tabela hash pendente)
    if(strcmp(lexema, "if") == 0) return TK_IF;
    if(strcmp(lexema, "else") == 0) return TK_ELSE;
    if(strcmp(lexema, "for") == 0) return TK_FOR;
    if(strcmp(lexema, "while") == 0) return TK_WHILE;
    
    // Se tudo falhar, cai aqui
    return TK_ID;    
}


// Funções públicas

// Inicializador do scanner
void scanner_init(Scanner *scanner, FILE *input) {
    scanner->fonte = input;
    scanner->linha = 1;
}

// TODO: Achar uma forma de otimizar isso aqui. Está horroroso e não é o ideal, mas funciona.
Token prox_token(Scanner *scanner) {
    Token token;

    while (1) {
        token.lexema[0] = '\0';

        // 1. Ignora espaços em branco
        char c = avanca(scanner);
        while (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            c = avanca(scanner);
        }

        token.linha = scanner->linha; // Armazena a linha onde o token se encontra

        // 2. EOF
        if (c == EOF) {
            token.tipo = TK_EOF;
            strcpy(token.lexema, "");
            return token;
        }

        // 3. Trata / e Comentários (Descarte)
        if (c == '/') {
            char proximo = prox_char(scanner);

            // Comentário de Linha: //
            if (proximo == '/') {
                avanca(scanner); // Consome o segundo '/'
                while (prox_char(scanner) != '\n' && prox_char(scanner) != EOF) {
                    avanca(scanner); // Apenas avança descartando o conteúdo
                }
                continue; // Reinicia o laço para pegar o próximo token válido
            } else if (proximo == '*') { // Comentário de Bloco: /* ... */
                avanca(scanner); // Consome o '*'
                int erro_eof = 0;
                int fim_comentario = 0;

                while (!erro_eof && !fim_comentario) {
                    char ch = avanca(scanner);

                    if (ch == EOF) {
                        erro_eof = 1;
                    } else if (ch == '*' && prox_char(scanner) == '/') {
                        avanca(scanner); // Consome o '/' final
                        fim_comentario = 1;
                    }
                }

                if (erro_eof) {
                    token.tipo = TK_ERROR;
                    strcpy(token.lexema, "/*");
                    return token;
                } else {
                    continue; // Comentário foi fechado
                }

            }

            // Operador de Divisão simples: /
            token.lexema[0] = '/';
            token.lexema[1] = '\0';
            token.tipo = TK_DIV;
            return token;
        }

        // 4. Identificadores e Palavras Reservadas
        if (isalpha(c) || c == '_') {
            int i = 0;
            token.lexema[i++] = c;

            char proximo = prox_char(scanner);
            while ((isalnum(proximo) || proximo == '_') && i < 63) {
                token.lexema[i++] = avanca(scanner);
                proximo = prox_char(scanner);
            }

            // Verificação de palavra reservada
            token.lexema[i] = '\0';
            token.tipo = ver_palavra_res(token.lexema);
            return token;
        }

        // 5. Números Inteiros e Floats
        // Nota: Provavelmente eu compliquei mais do que era necessário. Mas serviu como aprendizado.
        if (isdigit(c)) {
            int i = 0;
            int eh_float = 0;
            token.lexema[i++] = c;

            while (isdigit(prox_char(scanner)) && i < 63) {
                token.lexema[i++] = avanca(scanner);
            }

            // Se é número, e tem ponto, é real
            if (prox_char(scanner) == '.') {
                eh_float = 1;
                token.lexema[i++] = avanca(scanner);

                while (isdigit(prox_char(scanner)) && i < 63) {
                    token.lexema[i++] = avanca(scanner);
                }
            }

            // Se tem número, e tem letra (ou _ ) após um número, erro léxico
            if (isalpha(prox_char(scanner)) || prox_char(scanner) == '_') {
                while ((isalnum(prox_char(scanner)) || prox_char(scanner) == '_') && i < 63) {
                    token.lexema[i++] = avanca(scanner);
                }
                token.lexema[i] = '\0';
                token.tipo = TK_ERROR;
                return token;
            }

            // verificação final de tipo int/float
            token.lexema[i] = '\0';
            if (eh_float) {
                token.tipo = TK_FLOAT;
                token.valor_num.f = (float)atof(token.lexema);
            } else {
                token.tipo = TK_NUM;
                token.valor_num.i = atoi(token.lexema);
            }
            return token;
        }

        // 6. Operadores Restantes e Pontuação
        token.lexema[0] = c;
        token.lexema[1] = '\0';

        switch (c) {
            case '=':
                if (prox_char(scanner) == '=') {
                    token.lexema[1] = avanca(scanner);
                    token.lexema[2] = '\0';
                    token.tipo = TK_EQ;
                } else {
                    token.tipo = TK_ASSIGN;
                }
                return token;

            case '<':
                if (prox_char(scanner) == '=') {
                    token.lexema[1] = avanca(scanner);
                    token.lexema[2] = '\0';
                    token.tipo = TK_LEQ;
                } else {
                    token.tipo = TK_LT;
                }
                return token;

            case '>':
                if (prox_char(scanner) == '=') {
                    token.lexema[1] = avanca(scanner);
                    token.lexema[2] = '\0';
                    token.tipo = TK_GEQ;
                } else {
                    token.tipo = TK_GT;
                }
                return token;

            case '"': {
                int i = 0;
                token.lexema[i++] = c;

                while (prox_char(scanner) != '"' && prox_char(scanner) != '\n' && prox_char(scanner) != EOF) {
                    if (i < 63) token.lexema[i++] = avanca(scanner);
                    else avanca(scanner);
                }

                if (prox_char(scanner) == '"') {
                    if (i < 63) token.lexema[i++] = avanca(scanner);
                    else avanca(scanner);
                    token.lexema[i] = '\0';
                    token.tipo = TK_STRING;
                    return token;
                }

                token.lexema[i] = '\0';
                token.tipo = TK_ERROR;
                return token;
            }

            case '+': token.tipo = TK_PLUS; return token;
            case '-': token.tipo = TK_MINUS; return token;
            case '*': token.tipo = TK_TIMES; return token;
            case '%': token.tipo = TK_MOD; return token;
            case ';': token.tipo = TK_SEMI; return token;
            case '(': token.tipo = TK_LPAREN; return token;
            case ')': token.tipo = TK_RPAREN; return token;

            default:
                token.tipo = TK_ERROR;
                return token;
        }
    }
}


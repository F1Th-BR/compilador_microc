#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    // Identificadores e Literais
    TK_ID,      // [_a-zA-z][_a-zA-Z0-9]*
    TK_NUM,     // [0-9]+
    TK_FLOAT,   // [0-9](.[0-9])?([0-9]+)?
    TK_STRING,   // ["]
        
    // Operadores Aritméticos
    TK_PLUS,    // +
    TK_MINUS,   // -
    TK_TIMES,   // *
    TK_DIV,     // /
    TK_MOD,     // %

    // Operador Relacionais e de Atribuição
    TK_ASSIGN,  // =
    TK_EQ,      // ==
    TK_LT,      // <
    TK_LEQ,     // <=
    TK_GT,      // >
    TK_GEQ,     // >=
    
    // Pontuação e Comentários
    TK_SEMI,    // ;
    TK_SCOM,    // //
    TK_MCOM,    // /* */
    TK_LPAREN,  // (
    TK_RPAREN,  // )

    // Palavras Reservadas
    TK_IF,
    TK_ELSE,
    TK_FOR,
    TK_WHILE,

    // EOF e Erros
    TK_EOF,
    TK_ERROR
} TokenType;

typedef struct {
    TokenType tipo;
    char lexema[64];
    int linha;
    union {
        int i;
        float f;
    } valor_num;
} Token;

#endif
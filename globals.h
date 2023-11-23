#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXRESERVED 6

typedef enum
{
    ENDFILE,
    ERROR,

    ELSE,
    IF,
    INT,
    RETURN,
    VOID,
    WHILE,

    PLUS,
    MINUS,
    TIMES,
    OVER,
    LT,
    LTE,
    GT,
    GTE,
    EQ,
    NEQ,
    ASSIGN,
    SEMI,
    COMMA,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,

    ID,
    NUM
} TokenType;

extern FILE *source_file;
extern int lineno;
%{
#include "globals.h"
#include "scan.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
int yyparse(void);
void yyerror(const char *);

%}

%token DIGIT
%token ENDFILE
%token ERROR
%token ELSE
%token IF
%token INT
%token RETURN
%token VOID
%token WHILE
%token RESERVEDWORD
%token PLUS
%token MINUS
%token TIMES
%token OVER
%token LT
%token LTE
%token GT
%token GTE
%token EQ
%token NEQ
%token ASSIGN
%token SEMI
%token COMMA
%token LPAREN
%token RPAREN
%token LBRACKET
%token RBRACKET
%token LBRACE
%token RBRACE

%token ID
%token NUM

%%


%%

int yylex()
{
    //analisador lexico
}

programa : declaracao-lista

declaracao-lista : declaracao-lista declaracao | declaracao

declaracao : var-declaracao | fun-declaracao

var-declaracao : tipo-especificador ID SEMI | tipo-especificador ID LBRACKET NUM RBRACKET SEMI

tipo-especificador : INT | VOID

fun-declaracao : tipo-especificador ID LPAREN params RPAREN composto-decl

params : param-lista | VOID

param-lista : param-lista COMMA param | param

param : tipo-especificador ID | tipo-especificador ID LBRACKET RBRACKET

composto-decl : LBRACE local-declaracoes statement-lista RBRACE

local-declaracoes : local-declaracoes var-declaracao | vazio

statement-lista : statement-lista statement | vazio

statement : expressao-decl | composto-decl | selecao-decl | iteracao-decl | retorno-decl

expressao-decl : expressao SEMI | SEMI

selecao-decl : IF LPAREN expressao RPAREN statement | IF LPAREN expressao RPAREN ELSE statement

iteracao-decl : WHILE LPAREN expressao RPAREN statement

retorno-decl : RETURN SEMI | RETURN expressao SEMI

expressao : var = expressao | simples-expressao

var : ID | ID LBRACKET expressao RBRACKET

simples-expressao : soma-expressao relacional soma-expressao | soma-expressao

relacional : LTE | LT | GT | GTE | EQ | NEQ

soma-expressao : soma-expressao soma termo | termo

soma : PLUS | MINUS

termo : termo mult fator | fator

mult : TIMES | OVER

fator : LBRACKET expressao RBRACKET | var | ativacao | NUM

ativacao : ID LBRACKET args RBRACKET

args : arg-lista | vazio

arg-lista : arg-lista SEMI expressao | expressao 
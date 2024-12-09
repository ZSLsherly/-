%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%option noyywrap

// �ؼ��ֶ���
%keyword main if then else while do repeat until for from to step switch of case default return
%type {char*} keyword

// ר�÷��Ŷ���
%{
#define ASSIGN ":="
#define PLUS "+"
#define MINUS "-"
#define MUL "*"
#define DIV "/"
#define LT "<"
#define LE "<="
#define GT ">"
#define GE ">="
#define NEQ "!="
#define COMMA ","
#define SEMICOLON ";"
#define COLON ":"
#define LBRACE "{"
#define RBRACE "}"
#define LBRACKET "["
#define RBRACKET "]"
#define LPAREN "("
#define RPAREN ")"
%}

// ������ĸ�����ֵ�ƥ��
letter    [a-zA-Z]
digit     [0-9]
id        {letter}({letter}|{digit})*
num       {digit}+

// �ʷ���������
%%
"main"            { printf("KEYWORD: main\n"); }
"if"              { printf("KEYWORD: if\n"); }
"then"            { printf("KEYWORD: then\n"); }
"else"            { printf("KEYWORD: else\n"); }
"while"           { printf("KEYWORD: while\n"); }
"do"              { printf("KEYWORD: do\n"); }
"repeat"          { printf("KEYWORD: repeat\n"); }
"until"           { printf("KEYWORD: until\n"); }
"for"             { printf("KEYWORD: for\n"); }
"from"            { printf("KEYWORD: from\n"); }
"to"              { printf("KEYWORD: to\n"); }
"step"            { printf("KEYWORD: step\n"); }
"switch"          { printf("KEYWORD: switch\n"); }
"of"              { printf("KEYWORD: of\n"); }
"case"            { printf("KEYWORD: case\n"); }
"default"         { printf("KEYWORD: default\n"); }
"return"          { printf("KEYWORD: return\n"); }

":="              { printf("ASSIGN: :=\n"); }
"+"               { printf("OPERATOR: +\n"); }
"-"               { printf("OPERATOR: -\n"); }
"*"               { printf("OPERATOR: *\n"); }
"/"               { printf("OPERATOR: /\n"); }
"<"               { printf("OPERATOR: <\n"); }
"<="              { printf("OPERATOR: <=\n"); }
">"               { printf("OPERATOR: >\n"); }
">="              { printf("OPERATOR: >=\n"); }
"!="              { printf("OPERATOR: !=\n"); }
","               { printf("SEPARATOR: ,\n"); }
";"               { printf("SEPARATOR: ;\n"); }
":"               { printf("SEPARATOR: :\n"); }
"{"               { printf("SEPARATOR: {\n"); }
"}"               { printf("SEPARATOR: }\n"); }
"["               { printf("SEPARATOR: [\n"); }
"]"               { printf("SEPARATOR: ]\n"); }
"("               { printf("SEPARATOR: (\n"); }
")"               { printf("SEPARATOR: )\n"); }

{id}              { printf("ID: %s\n", yytext); }
{num}             { printf("NUM: %s\n", yytext); }

[ \t\n]+          { /* ���Կհס��Ʊ����ͻ��з� */ }

.                 { printf("UNKNOWN: %s\n", yytext); }

%%

int main() {
    yylex();  // �����ʷ�����
    return 0;
}

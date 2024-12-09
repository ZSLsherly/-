%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%option noyywrap

// 关键字定义
%keyword main if then else while do repeat until for from to step switch of case default return
%type {char*} keyword

// 专用符号定义
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

// 定义字母和数字的匹配
letter    [a-zA-Z]
digit     [0-9]
id        {letter}({letter}|{digit})*
num       {digit}+

// 词法分析规则
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

[ \t\n]+          { /* 忽略空白、制表符和换行符 */ }

.                 { printf("UNKNOWN: %s\n", yytext); }

%%

int main() {
    yylex();  // 启动词法分析
    return 0;
}


%{
#include <stdio.h>
#include <stdlib.h>
int count = 0;
%}

digit       [0-9]
letter      [a-zA-Z]
id          ({letter}|_){letter}*
num         {digit}+(\.{digit}+)?([eE][+-]?{digit}+)?
operator    :=|\+|\-|\*|\/|<|<=|>|>=|!=
delim       [ \t\n\r]
whitespace  {delim}+
str         \"[^"]*\"
semicolon   [\;]
other       .
reservedWord main|if|then|else|while|do|repeat|until|for|from|to|step|switch|of|case|default|return|integer|real|char|bool|and|or|not|mod|read|write

%%

{reservedWord}  {count++; printf("%d\t(reserved word,\'%s\')\n", count, yytext);}
{id}            {count++; printf("%d\t(id,\'%s\')\n", count, yytext);}
{num}           {count++; printf("%d\t(num,\'%s\')\n", count, yytext);}
{operator}      {count++; printf("%d\t(op,\'%s\')\n", count, yytext);}
{whitespace}    { /* ignore */ }
{str}           {count++; printf("%d\t(string,\'%s\')\n", count, yytext);}
"("             {count++; printf("%d\t(left paren,\'%s\')\n", count, yytext);}
")"             {count++; printf("%d\t(right paren,\'%s\')\n", count, yytext);}
"{"             {count++; printf("%d\t(left brace,\'%s\')\n", count, yytext);}
"}"             {count++; printf("%d\t(right brace,\'%s\')\n", count, yytext);}
"["             {count++; printf("%d\t(left bracket,\'%s\')\n", count, yytext);}
"]"             {count++; printf("%d\t(right bracket,\'%s\')\n", count, yytext);}
","             {count++; printf("%d\t(comma,\'%s\')\n", count, yytext);}
";"             {count++; printf("%d\t(semicolon,\'%s\')\n", count, yytext);}
":"             {count++; printf("%d\t(colon,\'%s\')\n", count, yytext);}
"#".*           {count++; printf("%d\t(head,\'%s\')\n", count, yytext);}
{other}         {printf("unknown character: \'%s\'\n", yytext);}

%%

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("File opening failed");
        return 1;
    }
    yylex();
    fclose(yyin);
    return 0;
}

int yywrap() {
    return 1;
}


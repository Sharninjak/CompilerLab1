%{
    #include "node.h"
    #include "syntax.tab.h"
    extern unsigned lexError;
    int lines = 0;
    int yycolumn = 1;
    // YY_USER_ACTION宏代表在执行每一个语义动作之前需要先被执行的一段代码
    #define YY_USER_ACTION \
        yylloc.first_line=yylloc.last_line=yylineno; \
        yylloc.first_column=yycolumn; \
        yylloc.last_column=yycolumn+yyleng-1; \
        yycolumn+=yyleng;
%}

%option yylineno

ws          [ \t]
digit       [0-9]
letter      [_a-zA-Z]
ld          [0-9_a-zA-Z]
RELOP 	    >|<|>=|<=|==|!=
IF          if
ELSE        else
WHILE       while
TYPE        int|float
STRUCT      struct
RETURN      return
PLUS        "+"
MINUS       "-"
STAR        "*"
DIV         "/"
AND         "&&"
OR          "||"
NOT         "!"
DOT         "."
SEMI        ";"
COMMA       ","
ASSIGNOP    "="
LP          "("
RP          ")"
LB          "["
RB          "]"
LC          "{"
RC          "}"
ID          {letter}{ld}*
INT         0|([1-9]+[0-9]*)
OCT         0[0-7]+
HEX         0[xX][0-9a-fA-F]+
FLOAT       ({digit}+"."{digit}+)|({digit}*"."{digit}+[eE][+-]?{digit}+)|({digit}+"."{digit}*[eE][+-]?{digit}+)
COMMENT     "//"


%%
{ws}+           {;}
\n|\r           { yycolumn = 1; }
{IF}            { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "IF", yytext); return IF; }
{ELSE}          { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "ELSE", yytext); return ELSE; }
{WHILE}         { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "WHILE", yytext); return WHILE; }
{TYPE}          { yylval.node = newTokenNode(yylineno, TOKEN_TYPE, "TYPE", yytext); return TYPE; }
{STRUCT}        { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "STRUCT", yytext); return STRUCT; }
{RETURN}        { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "RETURN", yytext); return RETURN; }
{RELOP}         { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "RELOP", yytext); return RELOP; }
{PLUS}          { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "PLUS", yytext); return PLUS; }
{MINUS}         { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "MINUS", yytext); return MINUS; }
{STAR}          { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "STAR", yytext); return STAR; }
{DIV}           { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "DIV", yytext); return DIV; }
{AND}           { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "AND", yytext); return AND; }
{OR}            { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "OR", yytext); return OR; }
{NOT}           { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "NOT", yytext); return NOT; }
{DOT}           { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "DOT", yytext); return DOT; }
{SEMI}          { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "SEMI", yytext); return SEMI; }
{COMMA}         { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "COMMA", yytext); return COMMA; }
{ASSIGNOP}      { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "ASSIGNOP", yytext); return ASSIGNOP; }
{LP}            { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "LP", yytext); return LP; }
{RP}            { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "RP", yytext); return RP; }
{LB}            { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "LB", yytext); return LB; }
{RB}            { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "RB", yytext); return RB; }
{LC}            { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "LC", yytext); return LC; }
{RC}            { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "RC", yytext); return RC; }
{ID}            { yylval.node = newTokenNode(yylineno, TOKEN_ID, "ID", yytext); return ID;}
{INT}           { yylval.node = newTokenNode(yylineno, TOKEN_INT, "INT", yytext); return INT;}
{OCT}           { yylval.node = newTokenNode(yylineno, TOKEN_INT, "INT", yytext); return INT;}
{HEX}           { yylval.node = newTokenNode(yylineno, TOKEN_INT, "INT", yytext); return INT;}
{FLOAT}         { yylval.node = newTokenNode(yylineno, TOKEN_FLOAT, "FLOAT", yytext); return FLOAT;}
"/*"            {
                    int line = yylineno;
                    char c = input();
                    char previous;
                    int flag = 0; // have not found */
                    while(c != 0)
                    {
                        previous = c;
                        c = input();
                        if(previous == '*' && c == '/'){
                            flag = 1;
                            break;
                        }
                    }
                    if(flag == 0){
                        lexError = TRUE;
                        printf("Error Type B at Line %d: /* can not match a */.\n", line);
                    }
                }
"*/"            { 
                    lexError = TRUE;
                    printf("Error Type B at Line %d: syntax error.\n", yylineno);
                }
{COMMENT}       { 
                    char c = input();
                    while(c != '\n'){
                        c = input();
                    };
                }

0([0-7]*[89][0-7]*)*        { lexError = TRUE; printf("Error type A at Line %d: Illegal octal \"%s\".\n", yylineno, yytext); }
0[xX]([0-9a-fA-F]*[g-zG-Z][0-9a-fA-F]*)* { lexError = TRUE; printf("Error type A at Line %d: Illegal hexadecimal \"%s\".\n", yylineno, yytext); }

.                           { lexError = TRUE; printf("Error type A at Line %d: Mysterious character \'%s\'.\n", yylineno, yytext); }





%%

// int main(int argc, char** argv) {
// if (argc > 1) {
// if (!(yyin = fopen(argv[1], "r"))) {
// perror(argv[1]);
// return 1;
// }
// }
// yylex();
// printf("%d\n",lines);
// return 0;
// }

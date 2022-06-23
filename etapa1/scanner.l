%{
#include <stdio.h>

int lineNumber = 1;
int running = 1;
%}

%x COMMENT

%%


int                     { fprintf(stderr, "KW_INT\n"); return 1; }
if                      { fprintf(stderr, "KW_IF\n"); return 2; }
[-\+\\*;\{\}\[\]\(\)]   { return 3; } // return yytext[0]; 
[a-zA-Z]+               { fprintf(stderr, "IDENTIFIER %s\n", yytext); return 4; }
[0-9]+                  { fprintf(stderr, "LIT_INT %s\n", yytext); return 5; }

[ \t]                   {}
"//".*                  {} // comment
"/-"                    { BEGIN(COMMENT); }
[\n]                    { ++ lineNumber; }
.                       { return 10; }

<COMMENT>"-/"           { BEGIN(INITIAL); }
<COMMENT>.              {}
<COMMENT>\n             { ++ lineNumber; }


%%
#include "main.c"

int yywrap()
{
    fprintf(stderr, "Number of lines in the document: %d\n", lineNumber);
    running = 0;
    return 1;
}

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


int main (int argc, char *argv[]) {
    int tok;

    if(argc < 2){
        fprintf(stderr, "Call: etapa input_file_name\n");
        exit(1);
    }

    if((yyin = fopen(argv[1], "r")) == 0 ){
        fprintf(stderr, "File %s not found!\n", argv[1]);
        exit(2);
    }

    while(running){
        tok = yylex();
        if(running == 0)
            break;
        
        switch(tok) {
            case 1:
                fprintf(stderr, "case 1\n");
                break;
            case 2:
                fprintf(stderr, "case 2\n");
                break;
            case 3:
                fprintf(stderr, "case 3\n");
                break;
            case 4:
                fprintf(stderr, "case 4\n");
                break;
            case 5:
                fprintf(stderr, "case 5\n");
                break;
            case 6:
                fprintf(stderr, "case 6\n");
                break;
            default:
                fprintf(stderr, "ERROR: invalid token!\n");
        }
    }
    exit(0);
}

int yywrap()
{
    fprintf(stderr, "Number of lines in the document: %d\n", lineNumber);
    running = 0;
    return 1;
}

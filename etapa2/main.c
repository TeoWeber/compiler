// main.c - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#include <stdio.h>
#include <stdlib.h>

#include "lex.yy.h"

extern int yylex(void);
extern int yylex_destroy(void);
extern int yyparse();

extern FILE *yyin;
extern char *yytext;

// TOOLS Functions
int getLineNumber(void);
int isRunning(void);
void initMe(void);

// HASH Functions
void hashPrint(void);

int main (int argc, char **argv) {
    int tok;

    if(argc < 2) {
        fprintf(stderr, "Call: etapa input_file_name\n");
        exit(1);
    }

    if((yyin = fopen(argv[1], "r")) == 0 ) {
        fprintf(stderr, "File %s not found!\n", argv[1]);
        exit(2);
    }

    initMe();

    yyparse();

    hashPrint();

    fclose(yyin);
    yylex_destroy();

    fprintf(stderr, "Program has %d lines.\n", getLineNumber());
    fprintf(stderr, "Compilation successful!\n");
    exit(0);
}
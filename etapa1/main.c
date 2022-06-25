#include <stdlib.h>
#include "tokens.h"
#include "utils.h"

extern int yylex(void);
extern int yylex_destroy(void);

extern FILE *yyin;
extern char *yytext;

int main (int argc, char **argv) {
    int tok;

    if(argc < 2){
        fprintf(stderr, "Call: etapa input_file_name\n");
        exit(1);
    }

    if((yyin = fopen(argv[1], "r")) == 0 ){
        fprintf(stderr, "File %s not found!\n", argv[1]);
        exit(2);
    }

    initMe();

    while(isRunning()){
        tok = yylex();
        if(isRunning() == 0)
            break;
        
        switch(tok) {
            case KW_CHAR:
                fprintf(stderr, "KW_CHAR at line %d\n", getLineNumber());
                break;
            case KW_INT:
                fprintf(stderr, "KW_INT at line %d\n", getLineNumber());
                break;
            case KW_FLOAT:
                fprintf(stderr, "KW_FLOAT at line %d\n", getLineNumber());
                break;
            case KW_IF:
                fprintf(stderr, "KW_IF at line %d\n", getLineNumber());
                break;
            case KW_ELSE:
                fprintf(stderr, "KW_ELSE at line %d\n", getLineNumber());
                break;
            case KW_WHILE:
                fprintf(stderr, "KW_WHILE at line %d\n", getLineNumber());
                break;
            case KW_READ:
                fprintf(stderr, "KW_READ at line %d\n", getLineNumber());
                break;
            case KW_PRINT:
                fprintf(stderr, "KW_PRINT at line %d\n", getLineNumber());
                break;
            case KW_RETURN:
                fprintf(stderr, "KW_RETURN at line %d\n", getLineNumber());
                break;
            case ASSIGNMENT:
                fprintf(stderr, "ASSIGNMENT at line %d\n", getLineNumber());
                break;
            case OPERATOR_LE:
                fprintf(stderr, "OPERATOR_LE at line %d\n", getLineNumber());
                break;
            case OPERATOR_GE:
                fprintf(stderr, "OPERATOR_GE at line %d\n", getLineNumber());
                break;
            case OPERATOR_EQ:
                fprintf(stderr, "OPERATOR_EQ at line %d\n", getLineNumber());
                break;
            case OPERATOR_DIF:
                fprintf(stderr, "OPERATOR_DIF at line %d\n", getLineNumber());
                break;
            case TK_IDENTIFIER:
                fprintf(stderr, "TK_IDENTIFIER at line %d\n", getLineNumber());
                break;
            case LIT_INTEGER:
                fprintf(stderr, "LIT_INTEGER at line %d\n", getLineNumber());
                break;
            case LIT_FLOAT:
                fprintf(stderr, "LIT_FLOAT at line %d\n", getLineNumber());
                break;
            case LIT_CHAR:
                fprintf(stderr, "LIT_CHAR at line %d\n", getLineNumber());
                break;
            case LIT_STRING:
                fprintf(stderr, "LIT_STRING at line %d\n", getLineNumber());
                break;
            case ';':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case '+':
            case '-':
            case '.':
            case '/':
            case '<':
            case '>':
            case '&':
            case '|':
            case '~':
                fprintf(stderr, "Special Character: %c at line %d\n", tok, getLineNumber());
                break;
            default:
                fprintf(stderr, "ERROR: invalid token at line %d!\n", getLineNumber());
        }
    }

    yylex_destroy();
    exit(0);
}
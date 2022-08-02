// parser.y - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

%token KW_CHAR      
%token KW_INT       
%token KW_FLOAT     

%token KW_IF        
%token KW_ELSE      
%token KW_WHILE     
%token KW_READ      
%token KW_PRINT     
%token KW_RETURN    

%token ASSIGNMENT
%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF

%token TK_IDENTIFIER

%token LIT_INTEGER  
%token LIT_FLOAT    
%token LIT_CHAR     
%token LIT_STRING   

%token TOKEN_ERROR  

%left '|' '&'
%left '<' '>' OPERATOR_EQ OPERATOR_LE OPERATOR_GE OPERATOR_DIF
%left '+' '-'
%left '.' '/'
%right '~'

%right "flow" KW_ELSE
%right "identifier" '('

%{
    #include <stdio.h>
    #include <stdlib.h>

    int yylex();
    int yyerror();
    extern int getLineNumber(void);
%}

%%


program: decl
    ;

decl: varDecl decl
    | funcDecl decl
    |
    ;

varDecl: type TK_IDENTIFIER '(' literal ')' ';'
    | type TK_IDENTIFIER '[' LIT_INTEGER ']' vecInitValue ';'
    ;

type: KW_INT
    | KW_FLOAT
    | KW_CHAR
    ;

literal: LIT_INTEGER
    | LIT_FLOAT
    | LIT_CHAR
    ;

vecInitValue: literal vecInitValue
    |
    ;

funcDecl: header cmdBlock
    ;

header: type TK_IDENTIFIER '(' paramList ')'
    ;

paramList: type TK_IDENTIFIER paramList
    |
    ;

cmdBlock: '{' cmdList '}'
    ;

cmdList: cmd ';' cmdList
    | cmd
    ;

cmd: read
    | print
    | return
    | attrib
    | flow
    | cmdBlock
    |
    ;

attrib: TK_IDENTIFIER ASSIGNMENT expr
    | TK_IDENTIFIER '[' expr ']' ASSIGNMENT expr
    ;

read: KW_READ TK_IDENTIFIER
    | KW_READ TK_IDENTIFIER '[' expr ']'
    ;

print: KW_PRINT printList
    ;

printList: LIT_STRING printList
    | expr printList
    |
    ;

return: KW_RETURN expr
    ;

expr: TK_IDENTIFIER                         %prec "identifier"
    | TK_IDENTIFIER '[' expr ']'
    | literal
    | '(' expr ')'
    | expr '+' expr
    | expr '-' expr
    | expr '.' expr
    | expr '/' expr
    | expr '<' expr
    | expr '>' expr
    | expr OPERATOR_LE expr
    | expr OPERATOR_GE expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_DIF expr
    | expr '&' expr
    | expr '|' expr
    | '~' expr
    | funcCall
    ;

funcCall: TK_IDENTIFIER '(' argList ')'
    ;

argList: expr argList
    |
    ;

flow: KW_IF '(' expr ')' cmd                %prec "flow"
    | KW_IF '(' expr ')' cmd KW_ELSE cmd
    | KW_WHILE '(' expr ')' cmd
    ;


%%

int yyerror ()
{
  fprintf(stderr,"Syntax error at line %d.\n", getLineNumber());
  exit(3);
}
